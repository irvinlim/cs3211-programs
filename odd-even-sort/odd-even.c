/**
 * Parallel-version of odd-even transposition sort,
 * CS3211 Lecture 4, Slide 33.
 * 
 * Implements the original odd-even sorting algorithm by Habermann (1972).
 * For a list of n elements, requires n processes to work on each element.
 * 
 * Uses Open MPI for message passing.
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

#define DEBUG false
#define DEBUGRANK 0

int arr[] = { 3, 2, 3, 8, 5, 6, 4, 1 };

static void exchange(int* arr, int src_rank, int tgt_rank)
{
    // Send arr[src_rank] to tgt_rank, 
    // receive number from tgt_rank and place in arr[tgt_rank].
    MPI_Sendrecv(&arr[src_rank], 1, MPI_INT, tgt_rank, 0,
        &arr[tgt_rank], 1, MPI_INT, tgt_rank, 0,
        MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

void compare_exchange_min(int* arr, int len, int src_rank, int tgt_rank)
{
    if (tgt_rank >= 0 && tgt_rank < len) {
        // Perform an exchange with the target before comparing.
        exchange(arr, src_rank, tgt_rank);

        // Compare and swap if larger.
        if (arr[src_rank] > arr[tgt_rank])
            swap(arr, src_rank, tgt_rank);
    }
}

void compare_exchange_max(int* arr, int len, int src_rank, int tgt_rank)
{
    if (tgt_rank >= 0 && tgt_rank < len) {
        // Perform an exchange with the target before comparing.
        exchange(arr, src_rank, tgt_rank);

        // Compare and swap if smaller.
        if (arr[src_rank] < arr[tgt_rank])
            swap(arr, src_rank, tgt_rank);
    }
}

int main(int argc, char** argv)
{
    int rank;
    int size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int len = sizeof(arr) / sizeof(arr[0]);
    int* result = calloc(len, sizeof(arr[0]));

    // Sanity check for the number of MPI processes.
    if (size != len) {
        if (rank == 0)
            fprintf(stderr, "ERROR: Number of processes has to be at least the length of the array = %d.\n", len);

        exit(1);
    }

    // Parallel section
    for (int i = 0; i < len; i++) {
#if DEBUG
        // Print array for process.
        if (rank == DEBUGRANK)
            debug_print(i, rank, len, arr);
        MPI_Barrier(MPI_COMM_WORLD);
#endif

        if (i % 2 == 0) {
            // If phase is even:
            if (rank % 2 == 0)
                compare_exchange_min(arr, len, rank, rank + 1);
            else
                compare_exchange_max(arr, len, rank, rank - 1);
        } else {
            // If phase is odd:
            if (rank % 2 == 1)
                compare_exchange_min(arr, len, rank, rank + 1);
            else
                compare_exchange_max(arr, len, rank, rank - 1);
        }
    }

    // Gather the numbers in each processor in their relevant position.
    MPI_Gather(&arr[rank], 1, MPI_INT, result, 1, MPI_INT, DEBUGRANK, MPI_COMM_WORLD);
    if (rank == DEBUGRANK)
        print(len, result);

    MPI_Finalize();
    return 0;
}

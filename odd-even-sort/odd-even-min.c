/**
 * Same as odd-even.c, except less verbose, which may be better for print.
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

int arr[] = { 3, 2, 3, 8, 5, 6, 4, 1 };

void sendrecv(int* arr, int src_rank, int tgt_rank)
{
    // Send and receive from tgt_rank, place in arr[tgt_rank].
    MPI_Sendrecv(&arr[src_rank], 1, MPI_INT, tgt_rank, 0,
                 &arr[tgt_rank], 1, MPI_INT, tgt_rank, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

void compare_exchange_min(int* arr, int len, int src_rank, int tgt_rank)
{
    if (tgt_rank >= 0 && tgt_rank < len) {
        sendrecv(arr, src_rank, tgt_rank);
        if (arr[src_rank] > arr[tgt_rank]) swap(arr, src_rank, tgt_rank);
    }
}

void compare_exchange_max(int* arr, int len, int src_rank, int tgt_rank)
{
    if (tgt_rank >= 0 && tgt_rank < len) {
        sendrecv(arr, src_rank, tgt_rank);
        if (arr[src_rank] < arr[tgt_rank]) swap(arr, src_rank, tgt_rank);
    }
}

int main(int argc, char** argv)
{
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int len = sizeof(arr) / sizeof(arr[0]);
    int* result = calloc(len, sizeof(arr[0]));

    // Parallel section
    for (int i = 0; i < len; i++) {
        if (i % 2 == 0) {
            if (rank % 2 == 0) compare_exchange_min(arr, len, rank, rank + 1);
            else compare_exchange_max(arr, len, rank, rank - 1);
        } else {
            if (rank % 2 == 1) compare_exchange_min(arr, len, rank, rank + 1);
            else compare_exchange_max(arr, len, rank, rank - 1);
        }
    }

    // Gather the final number from each process at the relevant index
    MPI_Gather(&arr[rank], 1, MPI_INT, result, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) print(len, result);

    MPI_Finalize();
    return 0;
}

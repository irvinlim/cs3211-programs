#include <stdio.h>
#include <stdlib.h>

#include "util.h"

void swap(int* arr, int a, int b)
{
    int temp;
    temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}

void print(int len, int* arr)
{
    for (int i = 0; i < len; i++)
        printf("%d ", arr[i]);

    printf("\n");
}

void debug_print(int phase, int rank, int len, int* arr)
{
    printf("[%d] Process %d: ", phase, rank);
    print(len, arr);
}

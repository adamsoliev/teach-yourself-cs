#include <assert.h>
#include <stdio.h>
#include "utils.h"
#include <stdlib.h>
#include <time.h>

#define DIM 15000

// 6.45 ◆◆◆◆
// In this assignment, you will apply the concepts you learned in Chapters 5 and 6
// to the problem of optimizing code for a memory-intensive application. Consider
// a procedure to copy and transpose the elements of an N × N matrix of type int.
// That is, for source matrix S and destination matrix D, we want to copy each
// element si,j to dj,i . This code can be written with a simple loop,
void transpose(int *dst, int *src, int dim)
{
    int i, j;
    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
        {
            dst[j * dim + i] = src[i * dim + j];
        }
}
// where the arguments to the procedure are pointers to the destination (dst) and
// source (src) matrices, as well as the matrix size N (dim). Your job is to devise a
// transpose routine that runs as fast as possible.

void transpose_optimized(int *dst, int *src, int dim)
{
    int i, j;
    for (i = 0; i < dim; i += 8)
    {
        for (j = 0; j < dim; j++)
        {
            dst[j * dim + i] = src[i * dim + j];
            dst[j * dim + i + 1] = src[(i + 1) * dim + j];
            dst[j * dim + i + 2] = src[(i + 2) * dim + j];
            dst[j * dim + i + 3] = src[(i + 3) * dim + j];
            dst[j * dim + i + 4] = src[(i + 4) * dim + j];
            dst[j * dim + i + 5] = src[(i + 5) * dim + j];
            dst[j * dim + i + 6] = src[(i + 6) * dim + j];
            dst[j * dim + i + 7] = src[(i + 7) * dim + j];
        }
    }
}

int main(void)
{
    // Seed the random number generator with the current time
    srand(time(NULL));

    // Create the source and destination matrices
    // int src[DIM][DIM];
    // int dst[DIM][DIM];

    // Had to allocate using malloc to fix Segmentation fault error when I increased DIM to 10k
    // Allocate memory for the matrices using malloc
    int *src = malloc(DIM * DIM * sizeof(int));
    int *dst = malloc(DIM * DIM * sizeof(int));

    // Initialize the source matrix with random values
    // int i, j;
    // for (i = 0; i < DIM; i++)
    // {
    //     for (j = 0; j < DIM; j++)
    //     {
    //         src[i][j] = rand();
    //     }
    // }

    // Initialize the source matrix with random values
    int i, j;
    for (i = 0; i < DIM; i++)
    {
        for (j = 0; j < DIM; j++)
        {
            src[i * DIM + j] = rand();
        }
    }

    // Measure the execution time of the original transpose function
    clock_t start, end;
    start = clock();
    transpose((int *)dst, (int *)src, DIM);
    end = clock();
    double time_elapsed_original = (double)(end - start) / CLOCKS_PER_SEC;

    // Check that the transpose was successful
    // for (i = 0; i < DIM; i++)
    // {
    //     for (j = 0; j < DIM; j++)
    //     {
    //         assert(dst[i][j] == src[j][i]);
    //     }
    // }

    // Check that the transpose was successful
    for (i = 0; i < DIM; i++)
    {
        for (j = 0; j < DIM; j++)
        {
            assert(dst[i * DIM + j] == src[j * DIM + i]);
        }
    }

    // Measure the execution time of the optimized transpose function
    start = clock();
    transpose_optimized((int *)dst, (int *)src, DIM);
    end = clock();
    double time_elapsed_optimized = (double)(end - start) / CLOCKS_PER_SEC;

    // Check that the transpose was successful
    // for (i = 0; i < DIM; i++)
    // {
    //     for (j = 0; j < DIM; j++)
    //     {
    //         assert(dst[i][j] == src[j][i]);
    //     }
    // }

    for (i = 0; i < DIM; i++)
    {
        for (j = 0; j < DIM; j++)
        {
            assert(dst[i * DIM + j] == src[j * DIM + i]);
        }
    }

    // Free the memory allocated for the matrices
    free(src);
    free(dst);

    // Print the execution time statistics
    printf("Original transpose function execution time: %f seconds\n", time_elapsed_original);
    printf("Optimized transpose function execution time: %f seconds\n", time_elapsed_optimized);
    printf("Optimized transpose function is %.2fx faster than original\n", time_elapsed_original / time_elapsed_optimized);

    return 0;
}

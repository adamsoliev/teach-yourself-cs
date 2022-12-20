#include <assert.h>
#include <stdio.h>
#include "utils.h"

// Reference
// https://dreamanddead.github.io/CSAPP-3e-Solutions/chapter3/

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
            printf("%d\n", j * dim + i);
            dst[j * dim + i] = src[i * dim + j];
        }
}
// where the arguments to the procedure are pointers to the destination (dst) and
// source (src) matrices, as well as the matrix size N (dim). Your job is to devise a
// transpose routine that runs as fast as possible.
/*
a[3][3]

a[0][1] 0
a[0][1] 4
a[0][2] 8
a[1][1] 12
a[1][1] 16
a[1][2] 20
a[2][1] 24
a[2][1] 28
a[2][2] 32
*/ 

int main(int argc, char *argv[])
{
    int a[10][10] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    int b[10][10] = {
        {23, 43, 12, 43, 65, 7, 34, 76, 25, 93},
        {23, 43, 12, 43, 65, 7, 34, 76, 25, 93},
        {23, 43, 12, 43, 65, 7, 34, 76, 25, 93},
        {23, 43, 12, 43, 65, 7, 34, 76, 25, 93},
        {23, 43, 12, 43, 65, 7, 34, 76, 25, 93},
        {23, 43, 12, 43, 65, 7, 34, 76, 25, 93},
        {23, 43, 12, 43, 65, 7, 34, 76, 25, 93},
        {23, 43, 12, 43, 65, 7, 34, 76, 25, 93},
        {23, 43, 12, 43, 65, 7, 34, 76, 25, 93},
        {23, 43, 12, 43, 65, 7, 34, 76, 25, 93}};

    transpose(&b[0][0], &a[0][0], 10);
    assert(b[4][5] == 0);
    // for (int i = 0; i < 10; i++)
    // {
    //     for (int j = 0; j < 10; j++)
    //     {
    //         printf("%d", b[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("a[4][5]: %d\n", a[4][5]);
    // assert(a[4][5] == 7);
}
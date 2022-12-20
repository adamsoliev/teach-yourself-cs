#include <assert.h>
#include <stdio.h>
#include "utils.h"

// Reference
// https://dreamanddead.github.io/CSAPP-3e-Solutions/chapter3/

// 5.18 ◆◆◆
// We considered the task of polynomial evaluation in Practice Problems 5.5 and 5.6,
// with both a direct evaluation and an evaluation by Horner’s method. Try to write
// faster versions of the function using the optimization techniques we have explored,
// including loop unrolling, parallel accumulation, and reassociation. You will find
// many different ways of mixing together Horner’s scheme and direct evaluation
// with these optimization techniques.
// double poly(double a[], double x, long degree)
// {
//     long i;
//     double result = a[0];
//     double xpwr = x; /* Equals x^i at start of loop */
//     for (i = 1; i <= degree; i++)
//     {
//         result += a[i] * xpwr;
//         xpwr = x * xpwr;
//     }
//     return result;
// }
// Ideally, you should be able to reach a CPE close to the throughput limit of
// your machine. Our best version achieves a CPE of 1.07 on our reference machine.

/* calculate a0 + a1*x + a2*x^2 + ... + an*x^n */
double poly(double a[], double x, long degree)
{
    long i;
    double result = a[0];
    double xpwr = x;
    for (i = 1; i <= degree; i++)
    {
        result += a[i] * xpwr;
        xpwr = x * xpwr;
    }
    return result;
}

double poly_6_3a(double a[], double x, long degree)
{
    long i = 1;
    double result = a[0];
    double result1 = 0;
    double result2 = 0;

    double xpwr = x;
    double xpwr1 = x * x * x;
    double xpwr2 = x * x * x * x * x;

    double xpwr_step = x * x * x * x * x * x;
    for (; i <= degree - 6; i += 6)
    {
        result = result + (a[i] * xpwr + a[i + 1] * xpwr * x);
        result1 = result1 + (a[i + 2] * xpwr1 + a[i + 3] * xpwr1 * x);
        result2 = result2 + (a[i + 4] * xpwr2 + a[i + 5] * xpwr2 * x);

        xpwr *= xpwr_step;
        xpwr1 *= xpwr_step;
        xpwr2 *= xpwr_step;
    }

    for (; i <= degree; i++)
    {
        result = result + a[i] * xpwr;
        xpwr *= x;
    }
    return result + result1 + result2;
}

/* apply horner's method */
double polyh(double a[], double x, long degree)
{
    long i;
    double result = a[degree];
    for (i = degree - 1; i >= 0; i--)
    {
        result = a[i] + x * result;
    }
    return result;
}

int main(int argc, char *argv[])
{
    // 5.18 ◆◆◆
    double a[10 + 1] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    double x = 2;
    long degree = 10;

    assert(poly(a, x, degree) == 2 + 4 + 8 + 16 + 32 + (2 + 4 + 8 + 16 + 32) * 32);
    assert(poly_6_3a(a, x, degree) == 2 + 4 + 8 + 16 + 32 + (2 + 4 + 8 + 16 + 32) * 32);
    assert(poly(a, x, degree) == polyh(a, x, degree));
    return 0;
}

#include <assert.h>
#include <stdio.h>
#include "utils.h"

// Reference
// https://dreamanddead.github.io/CSAPP-3e-Solutions/chapter2/

// 2.62 ◆◆◆
// Write a function int_shifts_are_arithmetic() that yields 1 when run on a
// machine that uses arithmetic right shifts for data type int and yields 0 otherwise.
// Your code should work on a machine with any word size. Test your code on several
// machines.
int int_sifts_are_arithmetic()
{
  int num = -1;               // 0xFFFFFFFF
  return !(num ^ (num >> 1)); // uses a ^ a = 0 property
}

// 2.65 ◆◆◆◆
// Write code to implement the following function:
// /* Return 1 when x contains an odd number of 1s; 0 otherwise.
// Assume w=32 */
// int odd_ones(unsigned x);
// Your function should follow the bit-level integer coding rules (page 164),
// except that you may assume that data type int has w = 32 bits.
// Your code should contain a total of at most 12 arithmetic, bitwise, and logical
// operations.
int odd_ones(unsigned x)
{
  // 0xffffffff
  x ^= x >> 16;
  x ^= x >> 8;
  x ^= x >> 4;
  x ^= x >> 2;
  x ^= x >> 1;
  x &= 0x1;
  return x;
}

// 2.66 ◆◆◆
// Write code to implement the following function:
// /*
// * Generate mask indicating leftmost 1 in x. Assume w=32.
// * For example, 0xFF00 -> 0x8000, and 0x6600 --> 0x4000.
// * If x = 0, then return 0.
// */
// int leftmost_one(unsigned x);
// Your function should follow the bit-level integer coding rules (page 164),
// except that you may assume that data type int has w = 32 bits.
// Your code should contain a total of at most 15 arithmetic, bitwise, and logical
// operations.
// Hint: First transform x into a bit vector of the form [0 . . . 011 . . . 1].
int leftmost_one(unsigned x)
{
  /*
   * first, generate a mask that all bits after leftmost one are one
   * e.g. 0xFF00 -> 0xFFFF, and 0x6000 -> 0x7FFF
   * If x = 0, get 0
   */
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
  /*
   * then, do (mask >> 1) + (mask && 1), in which mask && 1 deals with case x = 0, reserve leftmost bit one
   * that's we want
   */
  return (x >> 1) + (x && 1);
}

// 2.94 ◆◆◆
// Following the bit-level floating-point coding rules, implement the function with
// the following prototype:
// /* Compute 2*f. If f is NaN, then return f. */
// float_bits float_twice(float_bits f);
// For floating-point number f , this function computes 2.0 . f . If f is NaN, your
// function should simply return f .
// Test your function by evaluating it for all 232 values of argument f and com-
// paring the result to what would be obtained using your machine’s floating-point
// operations.
typedef unsigned float_bits;
float_bits float_twice(float_bits f)
{
  unsigned sig = f >> 31;
  unsigned exp = f >> 23 & 0xFF;
  unsigned frac = f & 0x7FFFFF;

  int is_NAN_or_oo = (exp == 0xFF);
  if (is_NAN_or_oo)
    return f;
  if (exp == 0)
  {
    /* Denormalized */
    frac <<= 1;
  }
  else if (exp == 0xFF - 1)
  {
    /* twice to oo */
    exp = 0xFF;
    frac = 0;
  }
  else
  {
    /* Normalized */
    exp += 1;
  }
  return sig << 31 | exp << 23 | frac;
}

// 2.95 ◆◆◆
// Following the bit-level floating-point coding rules, implement the function with
// the following prototype:
// /* Compute 0.5*f. If f is NaN, then return f. */
// float_bits float_half(float_bits f);
// For floating-point number f , this function computes 0.5 . f . If f is NaN, your
// function should simply return f .
// Test your function by evaluating it for all 232 values of argument f and com-
// paring the result to what would be obtained using your machine’s floating-point
// operations.
float_bits float_half(float_bits f)
{
  unsigned sig = f >> 31;
  unsigned rest = f & 0x7FFFFFFF;
  unsigned exp = f >> 23 & 0xFF;
  unsigned frac = f & 0x7FFFFF;

  int is_NAN_or_oo = (exp == 0xFF);
  if (is_NAN_or_oo)
  {
    return f;
  }

  /*
   * round to even, we care about last 2 bits of frac
   * 00 => 0 just >>1
   * 01 => 0 (round to even) just >>1
   * 10 => 1 just >>1
   * 11 => 1 + 1 (round to even) just >>1 and plus 1
   */
  int addition = (frac & 0x3) == 0x3;

  if (exp == 0)
  {
    frac >>= 1;
    frac += addition;
  }
  else if (exp == 1)
  {
    rest >>= 1;
    rest += addition;
    exp = rest >> 23 & 0xFF;
    frac = rest & 0x7FFFFF;
  }
  else
  {
    exp -= 1;
  }
  return sig << 31 | exp << 23 | frac;
}

// 2.96 ◆◆◆◆
// Following the bit-level floating-point coding rules, implement the function with
// the following prototype:
// /*
// * Compute (int) f.
// * If conversion causes overflow or f is NaN, return 0x80000000
// */
// int float_f2i(float_bits f);
// For floating-point number f , this function computes (int) f . Your function
// should round toward zero. If f cannot be represented as an integer (e.g., it is out
// of range, or it is NaN), then the function should return 0x80000000.
// Test your function by evaluating it for all 232 values of argument f and com-
// paring the result to what would be obtained using your machine’s floating-point
// operations.
int float_f2i(float_bits f)
{
  unsigned sig = f >> 31;
  unsigned exp = f >> 23 & 0xFF;
  unsigned frac = f & 0x7FFFFF;
  unsigned bias = 0x7F;

  int num;

  unsigned E;
  unsigned M;

  /*
   * consider positive numbers
   *
   * 0 00000000 00000000000000000000000
   *   ===>
   * 0 01111111 00000000000000000000000
   *   0 <= f < 1
   * get integer 0
   *
   * 0 01111111 00000000000000000000000
   *   ===>
   * 0 (01111111+31) 00000000000000000000000
   *   1 <= f < 2^31
   * integer round to 0
   *
   * 0 (01111111+31) 00000000000000000000000
   *   ===>
   * greater
   *   2^31 <= f < oo
   * return 0x80000000
   */
  if (exp >= 0 && exp < 0 + bias)
  {
    /* number less than 1 */
    num = 0;
  }
  else if (exp >= 31 + bias)
  {
    /* number overflow */
    /* or f < 0 and (int)f == INT_MIN */
    num = 0x80000000;
  }
  else
  {
    E = exp - bias;
    M = frac | 0x800000;
    if (E > 23)
    {
      num = M << (E - 23);
    }
    else
    {
      /* whether sig is 1 or 0, round to zero */
      num = M >> (23 - E);
    }
  }

  return sig ? -num : num;
}

int main(int argc, char *argv[])
{
  // 2.62 ◆◆◆
  // assert(int_sifts_are_arithmetic());

  // 2.65 ◆◆◆◆
  // assert(odd_ones(0x10101011));
  // assert(!odd_ones(0x01010101));

  // 2.66 ◆◆◆
  // assert(leftmost_one(0xFF00) == 0x8000);
  // assert(leftmost_one(0x6600) == 0x4000);
  // assert(leftmost_one(0x0) == 0x0);

  return 0;
}

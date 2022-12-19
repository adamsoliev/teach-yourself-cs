
#include <assert.h>
#include <stdio.h>
#include "utils.h"

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


int main(int argc, char *argv[])
{
  // 2.62 ◆◆◆
  // assert(int_sifts_are_arithmetic());

  // 2.65 ◆◆◆◆
  // assert(odd_ones(0x10101011));
  // assert(!odd_ones(0x01010101));
  return 0;
}

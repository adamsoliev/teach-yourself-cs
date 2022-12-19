
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

int main(int argc, char *argv[])
{
  // 2.62 ◆◆◆
  assert(int_sifts_are_arithmetic());

  return 0;
}

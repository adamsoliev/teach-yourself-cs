
#include <assert.h>
#include <stdio.h>

// utils
void _print(int a, char* str) {
  // message
  if (a > 0) {
    printf("[PASSED] - %s", str);
  } else if (a == 0) {
    printf("[FAILED] - %s", str);
  }
  printf("\n");
}


//////////////////////////////////////////////////////////////////////////////
typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len) {
  int i;
  for (i = 0; i < len; i++) 
    printf(" %.2x", start[i]);
  printf("\n");
}


void show_int(int x) {
  show_bytes((byte_pointer) &x, sizeof(int));
}

void show_float(float x) {
  show_bytes((byte_pointer) &x, sizeof(float));
}

void show_pointer(void *x) {
  show_bytes((byte_pointer) &x, sizeof(void *));
}


void test__show_bytes(int val) {
  printf("Showing int, float and *int bytes of %d\n", val);
  int ival = val;
  float fval = (float) ival;
  int *pval = &ival;
  show_int(ival);
  show_float(fval);
  show_pointer(pval);
}
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// 2.10
void inplace_swap(int *x, int *y) {
  // TODO: seems can't handle 0
  
  *y = *x ^ *y;
  *x = *x ^ *y;
  *y = *x ^ *y;
}

void test__inplace_swap() {
  // initialize variables
  int a = -23, b = 23, c = 9, d = 23492;

  // test
  inplace_swap(&a, &b);
  assert(a == 23);
  assert(b == -23);

  inplace_swap(&c, &d);
  assert(c == 23492);
  assert(d == 9);

  _print(1, "test__inplace_swap()");
}

// 2.11
void reverse_array(int a[], int cnt) {
  int first, last;
  for (first = 0, last = cnt - 1; first < last; first++, last--) {
    inplace_swap(&a[first], &a[last]);
  }
}

void test__reverse_array() {
  // initialize variables
  int a[4] = {-2, 4, 8, 1}; 
  int b[5] = {-2, 4, 8, 1, 9}; 
  
  // test
  reverse_array(a, 4);
  assert(a[0] == 1);
  assert(a[2] == 4);
  assert(a[3] == -2);

  reverse_array(b, 5);
  assert(b[0] == 9);
  assert(b[2] == 8);
  assert(b[4] == -2);

  _print(1, "test__reverse_array()");
}

// 2.12
void custom_bit_mask(int x) {
  printf("*** 2.12 ***\n");
  printf("x: %d\n", x);
  show_int(x);
 
  // A
  int a = x & 0xFF;
  printf("x & 0xFF: ");
  show_int(a);

  // B
  int b = x ^ (~0xFF);
  printf("x ^ (~0xFF): ");
  show_int(b);

  // C
  int c = x | 0xFF;
  printf("x | 0xFF: ");
  show_int(c);
}

// 2.13
int bis(int x, int m) { // bit set
  return x | m;
}
int bic(int x, int m) { // bit clear
  return x & (~m);
}
int bool_or(int x, int y) {
  return bis(x, y);
}

int bool_xor(int x, int y) {
  // x ^ y = (x & ~y) | (~x & y)
  return bis(bic(x, y), bic(y, x));
}
//////////////////////////////////////////////////////////////////////////////

// 2.58
int is_little_endian() {
  int test_num = 0xff;
  byte_pointer byte_start = (byte_pointer) &test_num;
  if (byte_start[0] == 0xff) return 1;
  return 0;
}

// 2.59
int leastXrestY(int x, int y) {
  // x = 0x89ABCDEF
  // y = 0x76543210
  // res = 0x765432EF
  int mask = 0xff;
  int res = (x & mask) | (y & ~mask);
  return res;
}

// 2.60
unsigned replace_byte(unsigned x, int i, unsigned char b) {
  // replace_byte(0x12345678, 2, 0xAB) --> 0x12AB5678
  // replace_byte(0x12345678, 0, 0xAB) --> 0x123456AB
  if (i < 0) {
    printf("error: i is negetive\n");
    return x;
  }
  if (i > sizeof(unsigned)-1) {
    printf("error: too big i");
    return x;
  }
  unsigned mask = ((unsigned) 0xff) << (i << 3);
  unsigned pos_byte = ((unsigned) b) << (i << 3);
  return (x & ~mask) | pos_byte;
}

// get the most significant byte
int get_msb(int x) { 
  // int x = 0xFF345678
  /* Shift by w-8 */
  int shift_val = (sizeof(int)-1)<<3;  // sizeof(int) = 4
  /* Arithmetic shift */
  int xright = x >> shift_val;   // 0xFFFFFFFF
  /* Zero all but LSB */
  return xright & 0xFF; // 0xFF000000 
}


// TODO: complete this
int get_lsb(int x) {
  return 0;
}

// 2.61
int A(int x) {
  return !~x;
}

int B(int x) {
  return !x;
}

int C(int x) {
  return A(x | ~0xff);
}

int D(int x) {
  return B((x >> ((sizeof(int) - 1) << 3)) & 0xff);
}


// 2.62
int int_shifts_are_arithmetic() {
  int num = -1;
  return !(num ^ (num >> 1));
}


int main() {

  // 2.58
  // assert(is_little_endian());

  // 2.59
  // int res = leastXrestY(0x89ABCDEF, 0x76543210);
  // assert(res == 0x765432EF);

  // 2.60
  /*
  unsigned rep_0 = replace_byte(0x12345678, 0, 0xAB);
  unsigned rep_3 = replace_byte(0x12345678, 3, 0xAB);
  assert(rep_0 == 0x123456AB);
  assert(rep_3 == 0xAB345678);
  */

  // 2.61
  /*
  int all_bit_one = ~0;
  int all_bit_zero = 0;

  assert(A(all_bit_one));
  assert(!B(all_bit_one));
  assert(C(all_bit_one));
  assert(!D(all_bit_one));

  assert(!A(all_bit_zero));
  assert(B(all_bit_zero));
  assert(!C(all_bit_zero));
  assert(D(all_bit_zero));

  // test magic number 0x1234ff
  assert(!A(0x1234ff));
  assert(!B(0x1234ff));
  assert(C(0x1234ff));
  assert(D(0x1234ff));

  // test magic number 0x1234
  assert(!A(0x1234));
  assert(!B(0x1234));
  assert(!C(0x1234));
  assert(D(0x1234));
  */

  assert(int_shifts_are_arithmetic());

  return 0;
}

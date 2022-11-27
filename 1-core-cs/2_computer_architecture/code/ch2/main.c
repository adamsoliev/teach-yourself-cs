
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
  assert(a = 23);
  assert(b = -23);

  inplace_swap(&c, &d);
  assert(c = 23492);
  assert(d = 9);

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
  assert(a[0] = 1);
  assert(a[2] = 4);
  assert(a[3] = -2);

  reverse_array(b, 5);
  assert(b[0] = 9);
  assert(b[2] = 8);
  assert(b[4] = -2);

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

int main() {
  /*
  test__show_bytes(43);
  test__inplace_swap();
  test__reverse_array();
  */
  // custom_bit_mask(0x65C9F2AB);
  int b = 0x46;
  show_int(~b);
  show_int(!b);
  show_int(!!b);

  return 0;
}

#include <assert.h>
#include <stdio.h>


// ----------------------------- UTILS -----------------------------

// helper for show_type_sizes()
// str - message string (e.g., 'size of int')
//
void print2c_la_ra(char *str, size_t size) {
  printf("%-15s %2zu\n", str, size);
}

// prints size of a given C type 
void show_type_sizes() {
  print2c_la_ra("size of char:", sizeof(char));
  print2c_la_ra("size of short:", sizeof(short));
  print2c_la_ra("size of int:", sizeof(int));
  print2c_la_ra("size of long:", sizeof(long));
  print2c_la_ra("size of float:", sizeof(float));
  print2c_la_ra("size of double:", sizeof(double));
}
//////////////////////////////////////////////////////////////////////////////


// logger print
// a    - code for pass (x > 0) or fail (0)
// str  - message to print
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
// byte printer functions
//
typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len) {
  size_t i;
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

int main() {
  test__show_bytes(21928734);
  show_type_sizes();

  return 0;
}

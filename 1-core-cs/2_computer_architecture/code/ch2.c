
#include <assert.h>
#include <stdio.h>

// ----------------------------- UTILS -----------------------------
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

int main() {
  test__show_bytes(21928734);

  return 0;
}

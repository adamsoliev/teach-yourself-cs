#include <stdio.h>
#include <string.h>

typedef unsigned char *byte_pointer;
void show_bytes(byte_pointer start, size_t len) {
  int i;
  for (i = 0; i < len; i++)
    printf(" %.2x", start[i]);
  printf("\n");
}
void show_int(int x) {
  printf("int: \t");
  show_bytes((byte_pointer) &x, sizeof(int));
}
void show_float(float x) {
  printf("float: \t");
  show_bytes((byte_pointer) &x, sizeof(float));
}
void show_char(char x) {
  printf("char: \t");
  show_bytes((byte_pointer) &x, sizeof(char));
}
void show_double(double x) {
  printf("double: \t");
  show_bytes((byte_pointer) &x, sizeof(double));
}
void show_pointer(void *x) {
  printf("pointer: \t");
  show_bytes((byte_pointer) &x, sizeof(void *));
}

void print_first_256_hexes() {
  for (int i = 0; i < 256; i++) {
    printf("%d: %x\n", i, i);
  }
}


int main() {
  int a = 19088743;
  float b = 4979827342.0;
  char c = '<';
  double d = 32598374.322;
  byte_pointer e = "string";


  /*
  show_int(a);
  show_float(b);
  show_char(c);
  show_double(d);
  */
  show_bytes(e, strlen(e));

  return 0;
}

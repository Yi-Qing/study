#include <stdio.h>

void foo() {
  __asm__("mov r9, #7\n\t"
          "mov r8, #7\n\t"
          "nop");
}

int main(int argc, char **argv) {
  int i = 0;
  foo();
  for (i = 0; i < 10; i++) {
    asm("mov r9, #9");
  }

  return 0;
}

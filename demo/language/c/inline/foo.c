#include "foo.h"
#include <stdio.h>

inline void foo_fun(void) { printf("hello world\n"); }

void do_fun() {
  printf("aaaaa\n");
  foo_fun();
  printf("bbbbb\n");
}

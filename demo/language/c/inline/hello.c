#include "foo.h"

extern void foo_fun(void);

int main(int argc, char **argv) {
  do_fun();

  foo_fun();

  return 0;
}

#include <stdio.h>

int main() {
  int *p;
  const int ic = 7;
  p = (int *)&ic;
  *p = 5;
  printf("%d\n", ic);

  return 0;
}

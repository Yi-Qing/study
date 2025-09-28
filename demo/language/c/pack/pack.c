#include "pack.h"
#include <stdint.h>
#include <stdio.h>

#if 0
;
# pragma pack(push, 1)
#else
# pragma pack(1)
#endif
struct hello {
  int a;
  char b;
  short c;
};
#pragma pack()

struct china {
  int a;
  char b;
  short c;
};

int main(int argc, char **argv) {

  printf("%zu: %zu\n", _Alignof(struct hello), sizeof(struct hello));
  printf("%zu: %zu\n", _Alignof(struct world), sizeof(struct world));
  printf("%zu: %zu\n", _Alignof(world_t), sizeof(world_t));
  printf("%zu: %zu\n", _Alignof(struct china), sizeof(struct china));

  return 0;
}

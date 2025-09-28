#ifndef _PACK_H__
#define _PACK_H__

#include <stdint.h>

#pragma pack(1)
typedef struct world {
  char a;
  short b;
  intptr_t c;
} world_t;
#pragma pack()

#endif

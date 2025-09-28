#include "world.h"
#include "say.h"
#include <stdio.h>

void cdf_world_world()
{
    printf("ret: %d\n", cdf_world_say("world"));
    printf("ret: %d\n", cdf_world_say("世界"));
}

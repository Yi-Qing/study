#include <stdio.h>
#include "say.h"

int cdf_world_say(char* str)
{
    if (str == NULL) {
        printf("%s + %s: error parm\n", __FILE__, __func__);
        return -1;
    }

    return printf("world say: %s\n", str);
}


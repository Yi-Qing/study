#include <stdio.h>
#include "ver.h"
#include "buildCfg.h"

char* cdf_get_version(char* buf, int len)
{
    snprintf(buf, len, "%d.%d.%d.%d%c",
            DEMO_VERSION_MAJOR, DEMO_VERSION_MINOR,
            DEMO_VERSION_PATCH, DEMO_VERSION_TWEAK,
            '\0');  // Pay attention here
    buf[len - 1] = '\0';    // Therefore, the caller can directly use printf

    return buf;
}


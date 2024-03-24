#include <stdio.h>
#include "ver.h"
#include "hello.h"
#include "world.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    char buf[40] = { 0 };
    printf("Ver: %s\n", cdf_get_version(buf, 40));
    cdf_hello_hello();
    cdf_world_world();

    return 0;
}

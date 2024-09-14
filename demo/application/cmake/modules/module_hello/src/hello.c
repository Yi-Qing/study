#include "hello.h"
#include "say.h"
#include <stdio.h>

void cdf_hello_hello()
{
    printf("ret: %d\n", cdf_hello_say("hello"));
    printf("ret: %d\n", cdf_hello_say("你好"));
}

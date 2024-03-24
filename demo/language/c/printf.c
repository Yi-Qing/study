#include <stdio.h>

int main(int argc, char **argv)
{
    float a = 7.5f;
    double b = a;
    long c = 0x401e000000000000;
    printf("%f\n", c);
    printf("%lx\n", c);
    printf("%f\n", *(double *)&c);
    printf("%lx\n", *(long *)&b);
    printf("%lx\n", a);
    printf("%f, %f\n", a, b);
	return 0;
}


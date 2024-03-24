#include <stdio.h>

#define PATH "hello"

int main(int argc, char **argv)
{
    char *p = PATH" world";
    printf ("%s\n", p);
	return 0;
}


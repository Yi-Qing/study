#include <stdio.h>

#define FUN_MAX 7

int foo1(int a) {
	printf("%s\n", __func__);
	return a + 1;
}

int foo2(int a) {
	printf("%s\n", __func__);
	return a + 1;
}

int foo3(int a) {
	printf("%s\n", __func__);
	return a + 1;
}

int foo4(int a) {
	printf("%s\n", __func__);
	return a + 1;
}

int foo5(int a) {
	printf("%s\n", __func__);
	return a + 1;
}

int foo6(int a) {
	printf("%s\n", __func__);
	return a + 1;
}

int foo7(int a) {
	printf("%s\n", __func__);
	return a + 1;
}

static int (*fun[FUN_MAX])(int) = {
	foo1, 
	foo2, 
	foo3, 
	foo4, 
	foo5, 
	foo6, 
	foo7, 
};

int main(int argc, char **argv)
{
	int ret = -1;
	printf("size = %ld\n", sizeof(fun) / sizeof(fun[0]));

	for (int i = 0; i < 7; i++) {
		ret = fun[i](i);
		printf("i = %d, ret = %d\n", i, ret);
	}
	return 0;
}


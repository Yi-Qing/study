#include <stdio.h>

int max(short a,  short b)
{
	int sub = a - b;
	int i = sub >> 31;	// 判断 a - b 的正负

	sub = (sub ^ i) - i;	// 对 a - b 取绝对值
	return ((a + b + sub) >> 1);
}

int main(int argc, char **argv)
{
	int a = 200 << 4;
	int i = 0;

	for (int j = 0; j < 4096; j++) {
		i += !!(max(a, j) ^ a);
	}
	printf("%d\n", i);

	return 0;
}


#include <stdio.h>
#include <sys/time.h>

unsigned long long now_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

int max(short a,  short b)
{
	int sub = a - b;
	int i = sub >> 31;	// 判断 a - b 的正负

	sub = (sub ^ i) - i;	// 对 a - b 取绝对值
	return ((a + b + sub) >> 1);
}

int min(short a,  short b)
{
	int sub = a - b;
	int i = sub >> 31;	// 判断 a - b 的正负

	sub = (sub ^ i) - i;	// 对 a - b 取绝对值
	return ((a + b - sub) >> 1);
}

int main(int argc, char **argv)
{
	int x = 0;
	int n = 4096;
	unsigned short buf[4096];

	int sub = 0;
	int j = 0;
	unsigned long long t0 = now_time();
	for (int k = 0; k < 10000; k++) {
		for (int i = 0; i < 4096; i++){
			// x = max(x, buf[i]);
			// n = min(n, buf[i]);
			if (x < buf[i]) {
				x = buf[i];
			}
			if (n > buf[i]) {
				n = buf[i];
			}
			// printf("max: %d, min: %d\n", x, n);
		}
	}
	printf("%lldms\n", now_time() - t0);

	return 0;
}


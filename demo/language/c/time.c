#include <stdio.h>
#include <sys/time.h>

struct test1 {
	volatile int a;
	volatile int b;
};

struct test2 {
	volatile int a;
	volatile int c[7];
	volatile int b;
};

int main(int argc,  char *argv[])
{
	int epoch_tm = 0;
	struct timeval tm;
	int current_tm = 0;
	int i;
	struct test1 value1;
	struct test2 value2;
	int tmp;

	gettimeofday(&tm,  NULL);
	current_tm = tm.tv_sec * 1000000 + tm.tv_usec;

	for (i = 0; i < 1000000000; i++) {
		tmp = value1.a;
		value1.b = i;
	}

	gettimeofday(&tm,  NULL);
	epoch_tm = (tm.tv_sec * 1000000 + tm.tv_usec) -current_tm;
	printf("cache phony share cost [%d] usec\n", epoch_tm);

	gettimeofday(&tm,  NULL);
	current_tm = tm.tv_sec * 1000000 + tm.tv_usec;

	for (i = 0; i < 1000000000; i++) 
	{
		tmp = value2.a;
		value2.b = i;
	}

	gettimeofday(&tm,  NULL);
	epoch_tm = (tm.tv_sec * 1000000 + tm.tv_usec) - current_tm;
	printf("cache not share cost [%d] usec\n", epoch_tm);
}

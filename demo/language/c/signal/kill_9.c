#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void dill_sig(int num)
{
    printf("signal %d\n", num);
    exit(0);
}

int main(int argc, char **argv)
{
    printf("process id %d\n", getpid());
    signal(SIGTERM, dill_sig);

    while (1);

	return 0;
}


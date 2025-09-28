#include <stdio.h>
#include <ctype.h>
#include <string.h>

static unsigned char numTable[10] = {0};

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("usage: %s 1234567890\n", argv[0]);
		return -1;
	}

	char *p = (char *)argv[1];
	int tmp = -1;
	int size = strlen(p);
	printf("size = %d\n", size);

	for (int i = 0; i < size; i++) {
		if (isdigit(p[i])) {
			tmp = p[i];
			numTable[tmp - '0']++;
		} else {
			printf("\t%c is not digit\n", p[i]);
		}
	}

	for (int i = 0; i < 10; i++) {
		if (numTable[i] != 0) {
			printf("%d use %d\n", i, numTable[i]);
		}
	}
	return 0;
}


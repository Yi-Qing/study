#include <stdio.h>

//#define __attribute__(x)

struct unpack {
	char a;
	short b;
	int c;
	double d;
};

// #pragma pack(1)
typedef union {
	unsigned char buf[3];
	struct {
		unsigned char cmd;
		unsigned short dummy;
	} __attribute__((packed)) st;
} BASE ;

struct pack {
	char a;
	short b;
	struct unpack c;
} __attribute__((packed));
//#pragma pack()

static BASE base = { {0x30, 0x66, 0x66} };

int main(int argc, char **argv)
{
	BASE b;
	printf("%ld, %ld\n", sizeof (BASE), sizeof (b.st));
	printf("%02x, %04x\n", base.st.cmd, base.st.dummy);

	printf("%ld, %ld\n", sizeof (struct unpack), sizeof (struct pack));

	return 0;
}


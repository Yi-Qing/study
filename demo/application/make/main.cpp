#include <iostream>
#include "a.h"
#include "b.h"

int main(int argc, char **argv)
{
    std::cout << __func__ << std::endl;
    foo_a();
    foo_b();
	return 0;
}


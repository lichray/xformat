#include "include/xformat/ioformat.h"

#include <iostream>

int main()
{
	using namespace stdex;

	printf(std::cout, "hello, %s\n"_cfmt, "nice");
	printf(std::cout, "%s\n", 42);
}

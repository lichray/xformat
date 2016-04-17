#include "include/xformat/ioformat.h"

#include <iostream>

int main()
{
	using namespace stdex;

	auto x = "hello, %s\n"_cfmt;
	printf(std::cout, x, "nice");
}

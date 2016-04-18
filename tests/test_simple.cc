#include <xformat/ioformat.h>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sstream>

using namespace stdex;

TEST_CASE("simple")
{
	std::stringstream ss;

	REQUIRE(printf(ss, "hello, %s\n"_cfmt, "nice"));
	REQUIRE(ss.str() == "hello, nice\n");

	ss.str("");
	printf(ss, "%s\n", 42);
	REQUIRE(ss.str() == "42\n");
}

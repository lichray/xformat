#include <xformat/ioformat.h>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sstream>

using namespace stdex;

TEST_CASE("simple")
{
	std::stringstream ss;

	REQUIRE(printf(ss, ""_cfmt));
	REQUIRE(ss.str() == "");

	REQUIRE(printf(ss, "", 42, "meow"));
	REQUIRE(ss.str() == "");

	REQUIRE(printf(ss, "x"));
	REQUIRE(ss.str() == "x");

	REQUIRE(printf(ss, "-man"));
	REQUIRE(ss.str() == "x-man");

	ss.str({});
	REQUIRE(printf(ss, "%s", 42));
	REQUIRE(ss.str() == "42");

	ss.str({});
	REQUIRE(printf(ss, "%s%%", 42));
	REQUIRE(ss.str() == "42%");

	ss.str({});
	REQUIRE(printf(ss, "%%akka"));
	REQUIRE(ss.str() == "%akka");

	REQUIRE_THROWS_AS("%k"_cfmt, std::invalid_argument);
	REQUIRE_THROWS_AS("only1%"_cfmt, std::invalid_argument);
	REQUIRE_THROWS_AS(printf(ss, "%s\n"), std::out_of_range);
	REQUIRE_THROWS_AS(printf(ss, "%s %s %s", 1, 2), std::out_of_range);

	ss.str({});
	REQUIRE(printf(ss, "%s\n", 42));
	REQUIRE(ss.str() == "42\n");

	ss.str({});
	REQUIRE(printf(ss, "hello, %s\n"_cfmt, "world"));
	REQUIRE(ss.str() == "hello, world\n");

	ss.str({});
	REQUIRE(printf(ss, "%s, %s, %s-body", "hey", 'u', 3));
	REQUIRE(ss.str() == "hey, u, 3-body");

	ss.str({});
	int i = 3;
	printf(ss, "%s", std::ref(i));
	REQUIRE(ss.str() == "3");
}

TEST_CASE("limitations")
{
	REQUIRE_NOTHROW("  %s %s %s %s %s %s %s %s %s "_cfmt);
	REQUIRE_THROWS_AS("%s %s %s %s %s %s %s %s %s %s %s"_cfmt, std::length_error);
}

#include <xformat/ioformat.h>

#include "catch.hpp"

#include <string>
#include <stdio.h>
#include <assert.h>

using namespace stdex;

template <typename... T>
auto aprintf(T... v)
{
	std::string s(100, 'X');
	auto n = snprintf(&*s.begin(), s.size(), v...);
	assert(n >= 0);
	s.resize(size_t(n));
	return s;
}

TEST_CASE("printf guarantees")
{
	std::stringstream ss;

	ss.width(12);
	printf(ss, "mixing|%#+8.3ff", 0.12);

	REQUIRE(ss.str() == "mixing|  +0.120f");
	REQUIRE(ss.width() == 12);
	REQUIRE(ss.precision() == 6);

	ss.str({});
	printf(ss, "%0*u|%-*.*E", 8, 42, 10, 3, 0.12);
	auto s = aprintf("%0*u|%-*.*E", 8, 42, 10, 3, 0.12);

	REQUIRE(ss.str() == s);
	REQUIRE(ss.str() == "00000042|1.200E-01 ");
	REQUIRE(ss.width() == 12);
	REQUIRE(ss.precision() == 6);

	ss.str({});
	printf(ss, "%3$0+*2$u|%4$-*2$.*1$E", 3, 10, 42, 0.12);
	REQUIRE(ss.str() == "+000000042|1.200E-01 ");
}

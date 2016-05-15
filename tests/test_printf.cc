#include <xformat/ioformat.h>

#include "catch.hpp"
#include "streamtest.h"

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
	ss.fill('_');
	printf(ss, "mixing|%#+8.3ff", 0.12);

	REQUIRE(str(ss) == "mixing|__+0.120f");
	REQUIRE(ss.width() == 12);
	REQUIRE(ss.precision() == 6);

	printf(ss, "%3$0+*2$u|%4$-*2$.*1$E", 3, 10, 42, 0.12);

	REQUIRE(str(ss) == "+000000042|1.200E-01_");
	REQUIRE(ss.width() == 12);
	REQUIRE(ss.precision() == 6);

	ss.fill(' ');
	printf(ss, "%0*u|%-*.*E", 8, 42, 10, 3, 0.12);
	auto s = aprintf("%0*u|%-*.*E", 8, 42, 10, 3, 0.12);

	REQUIRE(ss.str() == s);
	REQUIRE(str(ss) == "00000042|1.200E-01 ");
}

TEST_CASE("printf")
{
	std::stringstream ss;
	auto test = [&](auto... v)
	{
		auto s = aprintf(v...);
		printf(ss, v...);
		CHECK(str(ss) == s);
	};

	test("%%|%c|%s|%d|%i|%lu|%llx", '-', "", 42, 43, 44ul, 45ull);

	test("%2c", 'a');
	test("%12s", "str");

	test("%p", (void*)&ss);
	test("%12p", (void*)&ss);

	test("%12d", 42);
	test("%-12d", 42);
	test("%+12d", 42);
	test("%012d", 42);
	test("%-012d", 42);

	test("%12i", 42);
	test("%-12i", 42);
	test("%+12i", 42);
	test("%012i", 42);
	test("%-012i", 42);

	test("%12u", 42u);
	test("%-12u", 42u);
	test("%+12u", 42u);
	test("%012u", 42u);
	test("%-012u", 42u);

	test("%12o", 42u);
	test("%-12o", 42u);
	test("%+12o", 42u);
	test("%012o", 42u);
	test("%#12o", 42u);
	test("%-012o", 42u);
	test("%#012o", 42u);
	test("%#0+12o", 42u);

	test("%12x", 42u);
	test("%-12x", 42u);
	test("%+12x", 42u);
	test("%012x", 42u);
	test("%#12x", 42u);
	test("%-012x", 42u);
	test("%#012x", 42u);
	test("%#0+12x", 42u);

	test("%12X", 42u);
	test("%-12X", 42u);
	test("%+12X", 42u);
	test("%012X", 42u);
	test("%#12X", 42u);
	test("%-012X", 42u);
	test("%#012X", 42u);
	test("%#0+12X", 42u);

	test("%f", 3.14);
	test("%-f", 3.14);
	test("%+f", 3.14);
	test("%0f", 3.14);
	test("%#f", 3.14);
	test("%-0f", 3.14);
	test("%#0f", 3.14);
	test("%#0+f", 3.14);

	test("%.12f", 3.14);
	test("%-.12f", 3.14);
	test("%+.12f", 3.14);
	test("%0.12f", 3.14);
	test("%#.12f", 3.14);
	test("%-0.12f", 3.14);
	test("%#0.12f", 3.14);
	test("%#0+.12f", 3.14);

	test("%.12F", 3.14);
	test("%-.12F", 3.14);
	test("%+.12F", 3.14);
	test("%0.12F", 3.14);
	test("%#.12F", 3.14);
	test("%-0.12F", 3.14);
	test("%#0.12F", 3.14);
	test("%#0+.12F", 3.14);

	test("%e", 3.14);
	test("%-e", 3.14);
	test("%+e", 3.14);
	test("%0e", 3.14);
	test("%#e", 3.14);
	test("%-0e", 3.14);
	test("%#0e", 3.14);
	test("%#0+e", 3.14);

	test("%.12e", 3.14);
	test("%-.12e", 3.14);
	test("%+.12e", 3.14);
	test("%0.12e", 3.14);
	test("%#.12e", 3.14);
	test("%-0.12e", 3.14);
	test("%#0.12e", 3.14);
	test("%#0+.12e", 3.14);

	test("%.12E", 3.14);
	test("%-.12E", 3.14);
	test("%+.12E", 3.14);
	test("%0.12E", 3.14);
	test("%#.12E", 3.14);
	test("%-0.12E", 3.14);
	test("%#0.12E", 3.14);
	test("%#0+.12E", 3.14);

	test("%g", 3.14);
	test("%-g", 3.14);
	test("%+g", 3.14);
	test("%0g", 3.14);
	test("%#g", 3.14);
	test("%-0g", 3.14);
	test("%#0g", 3.14);
	test("%#0+g", 3.14);

	test("%.12g", 3.14);
	test("%-.12g", 3.14);
	test("%+.12g", 3.14);
	test("%0.12g", 3.14);
	test("%#.12g", 3.14);
	test("%-0.12g", 3.14);
	test("%#0.12g", 3.14);
	test("%#0+.12g", 3.14);

	test("%.12G", 3.14);
	test("%-.12G", 3.14);
	test("%+.12G", 3.14);
	test("%0.12G", 3.14);
	test("%#.12G", 3.14);
	test("%-0.12G", 3.14);
	test("%#0.12G", 3.14);
	test("%#0+.12G", 3.14);
}

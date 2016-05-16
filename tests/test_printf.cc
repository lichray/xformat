#include <xformat/ioformat.h>

#include "catch.hpp"
#include "streamtest.h"

#include <string>
#include <stdio.h>
#include <wchar.h>
#include <assert.h>

using namespace stdex;

template <typename... T>
auto gprintf(char* buf, size_t sz, char const* fmt, T... v)
{
	return ::snprintf(buf, sz, fmt, v...);
}

template <typename... T>
auto gprintf(wchar_t* buf, size_t sz, wchar_t const* fmt, T... v)
{
	return ::swprintf(buf, sz, fmt, v...);
}

template <typename charT, typename... T>
auto aprintf(charT const* fmt, T... v)
{
	std::basic_string<charT> s(100, 'X');
	auto n = gprintf(&*s.begin(), s.size(), fmt, v...);
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

	printf(ss, "%3$0+*2$d|%4$-*2$.*1$E", 3, 10, 42, 0.12);

	REQUIRE(str(ss) == "+000000042|1.200E-01_");
	REQUIRE(ss.width() == 12);
	REQUIRE(ss.precision() == 6);

	ss.fill(' ');
	printf(ss, "%0*u|%-*.*E", 8, 42, 10, 3, 0.12);
	auto s = aprintf("%0*u|%-*.*E", 8, 42, 10, 3, 0.12);

	REQUIRE(ss.str() == s);
	REQUIRE(str(ss) == "00000042|1.200E-01 ");

	std::string st = "test";
	printf(ss, "%-7.s", st);
	s = str(ss);
	printf(ss, "%-7.s", std::experimental::string_view(st));

	REQUIRE(s == "test   ");
	REQUIRE(str(ss) == s);
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
	test("%-12c", 'x');

	test("%12s", "str");
	test("%12.4s", "A long time ago");
	char s[] = "in a galaxy far far away";
	auto p = s;
	test("%12.4s", p);

	test("%p", &ss);
	test("%12p", &ss);

	test("%12d", 42);
	test("%-12d", 42);
	test("%+12d", 42);
	test("%012d", 42);
	test("%-012d", 42);
	test("% 12d", 42);
	test("% -12d", 42);
	test("% +12d", 42);
	test("% 012d", 42);
	test("% -012d", 42);

	test("%12i", 42);
	test("%-12i", 42);
	test("%+12i", 42);
	test("%012i", 42);
	test("%-012i", 42);
	test("% 12i", 42);
	test("% -12i", 42);
	test("% +12i", 42);
	test("% 012i", 42);
	test("% -012i", 42);

	test("%12u", 42u);
	test("%-12u", 42u);
	test("%+12u", 42u);
	test("%012u", 42u);
	test("%-012u", 42u);
	test("% 12u", 42u);
	test("% -12u", 42u);
	test("% +12u", 42u);
	test("% 012u", 42u);
	test("% -012u", 42u);

	test("%12o", 42u);
	test("%-12o", 42u);
	test("%+12o", 42u);
	test("%012o", 42u);
	test("%#12o", 42u);
	test("%-012o", 42u);
	test("%#012o", 42u);
	test("%#0+12o", 42u);
	test("% 12o", 42u);
	test("% -12o", 42u);
	test("% +12o", 42u);
	test("% 012o", 42u);
	test("% #12o", 42u);
	test("% -012o", 42u);
	test("% #012o", 42u);
	test("% #0+12o", 42u);

	test("%12x", 42u);
	test("%-12x", 42u);
	test("%+12x", 42u);
	test("%012x", 42u);
	test("%#12x", 42u);
	test("%-012x", 42u);
	test("%#012x", 42u);
	test("%#0+12x", 42u);
	test("% 12x", 42u);
	test("% -12x", 42u);
	test("% +12x", 42u);
	test("% 012x", 42u);
	test("% #12x", 42u);
	test("% -012x", 42u);
	test("% #012x", 42u);
	test("% #0+12x", 42u);

	test("%12X", 42u);
	test("%-12X", 42u);
	test("%+12X", 42u);
	test("%012X", 42u);
	test("%#12X", 42u);
	test("%-012X", 42u);
	test("%#012X", 42u);
	test("%#0+12X", 42u);
	test("% 12X", 42u);
	test("% -12X", 42u);
	test("% +12X", 42u);
	test("% 012X", 42u);
	test("% #12X", 42u);
	test("% -012X", 42u);
	test("% #012X", 42u);
	test("% #0+12X", 42u);

	test("%12.6d", 42);
	test("%-12.6d", 42);
	test("%+12.6d", 42);
	test("%012.6d", 42);
	test("%-012.6d", 42);
	test("% 12.6d", 42);
	test("% -12.6d", 42);
	test("% +12.6d", 42);
	test("% 012.6d", 42);
	test("% -012.6d", 42);

	test("%12.6i", 42);
	test("%-12.6i", 42);
	test("%+12.6i", 42);
	test("%012.6i", 42);
	test("%-012.6i", 42);
	test("% 12.6i", 42);
	test("% -12.6i", 42);
	test("% +12.6i", 42);
	test("% 012.6i", 42);
	test("% -012.6i", 42);

	test("%12.6u", 42u);
	test("%-12.6u", 42u);
	test("%+12.6u", 42u);
	test("%012.6u", 42u);
	test("%-012.6u", 42u);
	test("% 12.6u", 42u);
	test("% -12.6u", 42u);
	test("% +12.6u", 42u);
	test("% 012.6u", 42u);
	test("% -012.6u", 42u);

	test("%12.6o", 42u);
	test("%-12.6o", 42u);
	test("%+12.6o", 42u);
	test("%012.6o", 42u);
	test("%#12.6o", 42u);
	test("%-012.6o", 42u);
	test("%#012.6o", 42u);
	test("%#0+12.6o", 42u);
	test("% 12.6o", 42u);
	test("% -12.6o", 42u);
	test("% +12.6o", 42u);
	test("% 012.6o", 42u);
	test("% #12.6o", 42u);
	test("% -012.6o", 42u);
	test("% #012.6o", 42u);
	test("% #0+12.6o", 42u);

	test("%12.6x", 42u);
	test("%-12.6x", 42u);
	test("%+12.6x", 42u);
	test("%012.6x", 42u);
	test("%#12.6x", 42u);
	test("%-012.6x", 42u);
	test("%#012.6x", 42u);
	test("%#0+12.6x", 42u);
	test("% 12.6x", 42u);
	test("% -12.6x", 42u);
	test("% +12.6x", 42u);
	test("% 012.6x", 42u);
	test("% #12.6x", 42u);
	test("% -012.6x", 42u);
	test("% #012.6x", 42u);
	test("% #0+12.6x", 42u);

	test("%12.6X", 42u);
	test("%-12.6X", 42u);
	test("%+12.6X", 42u);
	test("%012.6X", 42u);
	test("%#12.6X", 42u);
	test("%-012.6X", 42u);
	test("%#012.6X", 42u);
	test("%#0+12.6X", 42u);
	test("% 12.6X", 42u);
	test("% -12.6X", 42u);
	test("% +12.6X", 42u);
	test("% 012.6X", 42u);
	test("% #12.6X", 42u);
	test("% -012.6X", 42u);
	test("% #012.6X", 42u);
	test("% #0+12.6X", 42u);

	test("%12.d", 0);
	test("%-12.d", 0);
	//test("%+12.d", 0);
	test("%012.d", 0);
	test("%-012.d", 0);
	test("% 12.d", 0);
	test("% -12.d", 0);
	//test("% +12.d", 0);
	test("% 012.d", 0);
	test("% -012.d", 0);

	test("%12.i", 0);
	test("%-12.i", 0);
	//test("%+12.i", 0);
	test("%012.i", 0);
	test("%-012.i", 0);
	test("% 12.i", 0);
	test("% -12.i", 0);
	//test("% +12.i", 0);
	test("% 012.i", 0);
	test("% -012.i", 0);

	test("%12.u", 0u);
	test("%-12.u", 0u);
	test("%+12.u", 0u);
	test("%012.u", 0u);
	test("%-012.u", 0u);
	test("% 12.u", 0u);
	test("% -12.u", 0u);
	test("% +12.u", 0u);
	test("% 012.u", 0u);
	test("% -012.u", 0u);

	test("%12.o", 0u);
	test("%-12.o", 0u);
	test("%+12.o", 0u);
	test("%012.o", 0u);
	//test("%#12.o", 0u);
	test("%-012.o", 0u);
	//test("%#012.o", 0u);
	//test("%#0+12.o", 0u);
	test("% 12.o", 0u);
	test("% -12.o", 0u);
	test("% +12.o", 0u);
	test("% 012.o", 0u);
	//test("% #12.o", 0u);
	test("% -012.o", 0u);
	//test("% #012.o", 0u);
	//test("% #0+12.o", 0u);

	test("%12.x", 0u);
	test("%-12.x", 0u);
	test("%+12.x", 0u);
	test("%012.x", 0u);
	test("%#12.x", 0u);
	test("%-012.x", 0u);
	test("%#012.x", 0u);
	test("%#0+12.x", 0u);
	test("% 12.x", 0u);
	test("% -12.x", 0u);
	test("% +12.x", 0u);
	test("% 012.x", 0u);
	test("% #12.x", 0u);
	test("% -012.x", 0u);
	test("% #012.x", 0u);
	test("% #0+12.x", 0u);

	test("%12.X", 0u);
	test("%-12.X", 0u);
	test("%+12.X", 0u);
	test("%012.X", 0u);
	test("%#12.X", 0u);
	test("%-012.X", 0u);
	test("%#012.X", 0u);
	test("%#0+12.X", 0u);
	test("% 12.X", 0u);
	test("% -12.X", 0u);
	test("% +12.X", 0u);
	test("% 012.X", 0u);
	test("% #12.X", 0u);
	test("% -012.X", 0u);
	test("% #012.X", 0u);
	test("% #0+12.X", 0u);

	test("%12o", 0u);
	test("%-12o", 0u);
	test("%+12o", 0u);
	test("%012o", 0u);
	test("%#12o", 0u);
	test("%-012o", 0u);
	test("%#012o", 0u);
	test("%#0+12o", 0u);
	test("% 12o", 0u);
	test("% -12o", 0u);
	test("% +12o", 0u);
	test("% 012o", 0u);
	test("% #12o", 0u);
	test("% -012o", 0u);
	test("% #012o", 0u);
	test("% #0+12o", 0u);

	test("%12.o", 42u);
	test("%-12.o", 42u);
	test("%+12.o", 42u);
	test("%012.o", 42u);
	test("%#12.o", 42u);
	test("%-012.o", 42u);
	test("%#012.o", 42u);
	test("%#0+12.o", 42u);
	test("% 12.o", 42u);
	test("% -12.o", 42u);
	test("% +12.o", 42u);
	test("% 012.o", 42u);
	test("% #12.o", 42u);
	test("% -012.o", 42u);
	test("% #012.o", 42u);
	test("% #0+12.o", 42u);

	test("%12.6o", -42);
	test("%-12.6o", -42);
	test("%+12.6o", -42);
	test("%012.6o", -42);
	test("%#12.6o", -42);
	test("%-012.6o", -42);
	test("%#012.6o", -42);
	test("%#0+12.6o", -42);
	test("% 12.6o", -42);
	test("% -12.6o", -42);
	test("% +12.6o", -42);
	test("% 012.6o", -42);
	test("% #12.6o", -42);
	test("% -012.6o", -42);
	test("% #012.6o", -42);
	test("% #0+12.6o", -42);

	test("%12u", -42);
	test("%-12u", -42);
	test("%+12u", -42);
	test("%012u", -42);
	test("%-012u", -42);
	test("% 12u", -42);
	test("% -12u", -42);
	test("% +12u", -42);
	test("% 012u", -42);
	test("% -012u", -42);

	test("%12o", -42);
	test("%-12o", -42);
	test("%+12o", -42);
	test("%012o", -42);
	test("%#12o", -42);
	test("%-012o", -42);
	test("%#012o", -42);
	test("%#0+12o", -42);
	test("% 12o", -42);
	test("% -12o", -42);
	test("% +12o", -42);
	test("% 012o", -42);
	test("% #12o", -42);
	test("% -012o", -42);
	test("% #012o", -42);
	test("% #0+12o", -42);

	test("%12x", -42);
	test("%-12x", -42);
	test("%+12x", -42);
	test("%012x", -42);
	test("%#12x", -42);
	test("%-012x", -42);
	test("%#012x", -42);
	test("%#0+12x", -42);
	test("% 12x", -42);
	test("% -12x", -42);
	test("% +12x", -42);
	test("% 012x", -42);
	test("% #12x", -42);
	test("% -012x", -42);
	test("% #012x", -42);
	test("% #0+12x", -42);

	test("%12X", -42);
	test("%-12X", -42);
	test("%+12X", -42);
	test("%012X", -42);
	test("%#12X", -42);
	test("%-012X", -42);
	test("%#012X", -42);
	test("%#0+12X", -42);
	test("% 12X", -42);
	test("% -12X", -42);
	test("% +12X", -42);
	test("% 012X", -42);
	test("% #12X", -42);
	test("% -012X", -42);
	test("% #012X", -42);
	test("% #0+12X", -42);

	test("%f", 3.14);
	test("%-f", 3.14);
	test("%+f", 3.14);
	test("%0f", 3.14);
	test("%#f", 3.14);
	test("%-0f", 3.14);
	test("%#0f", 3.14);
	test("%#0+f", 3.14);
	test("% f", 3.14);
	test("% -f", 3.14);
	test("% +f", 3.14);
	test("% 0f", 3.14);
	test("% #f", 3.14);
	test("% -0f", 3.14);
	test("% #0f", 3.14);
	test("% #0+f", 3.14);

	test("%.12f", 3.14);
	test("%-.12f", 3.14);
	test("%+.12f", 3.14);
	test("%0.12f", 3.14);
	test("%#.12f", 3.14);
	test("%-0.12f", 3.14);
	test("%#0.12f", 3.14);
	test("%#0+.12f", 3.14);
	test("% .12f", 3.14);
	test("% -.12f", 3.14);
	test("% +.12f", 3.14);
	test("% 0.12f", 3.14);
	test("% #.12f", 3.14);
	test("% -0.12f", 3.14);
	test("% #0.12f", 3.14);
	test("% #0+.12f", 3.14);

	test("%.12F", 3.14);
	test("%-.12F", 3.14);
	test("%+.12F", 3.14);
	test("%0.12F", 3.14);
	test("%#.12F", 3.14);
	test("%-0.12F", 3.14);
	test("%#0.12F", 3.14);
	test("%#0+.12F", 3.14);
	test("% .12F", 3.14);
	test("% -.12F", 3.14);
	test("% +.12F", 3.14);
	test("% 0.12F", 3.14);
	test("% #.12F", 3.14);
	test("% -0.12F", 3.14);
	test("% #0.12F", 3.14);
	test("% #0+.12F", 3.14);

	test("%e", 3.14);
	test("%-e", 3.14);
	test("%+e", 3.14);
	test("%0e", 3.14);
	test("%#e", 3.14);
	test("%-0e", 3.14);
	test("%#0e", 3.14);
	test("%#0+e", 3.14);
	test("% e", 3.14);
	test("% -e", 3.14);
	test("% +e", 3.14);
	test("% 0e", 3.14);
	test("% #e", 3.14);
	test("% -0e", 3.14);
	test("% #0e", 3.14);
	test("% #0+e", 3.14);

	test("%.12e", 3.14);
	test("%-.12e", 3.14);
	test("%+.12e", 3.14);
	test("%0.12e", 3.14);
	test("%#.12e", 3.14);
	test("%-0.12e", 3.14);
	test("%#0.12e", 3.14);
	test("%#0+.12e", 3.14);
	test("% .12e", 3.14);
	test("% -.12e", 3.14);
	test("% +.12e", 3.14);
	test("% 0.12e", 3.14);
	test("% #.12e", 3.14);
	test("% -0.12e", 3.14);
	test("% #0.12e", 3.14);
	test("% #0+.12e", 3.14);

	test("%.12E", 3.14);
	test("%-.12E", 3.14);
	test("%+.12E", 3.14);
	test("%0.12E", 3.14);
	test("%#.12E", 3.14);
	test("%-0.12E", 3.14);
	test("%#0.12E", 3.14);
	test("%#0+.12E", 3.14);
	test("% .12E", 3.14);
	test("% -.12E", 3.14);
	test("% +.12E", 3.14);
	test("% 0.12E", 3.14);
	test("% #.12E", 3.14);
	test("% -0.12E", 3.14);
	test("% #0.12E", 3.14);
	test("% #0+.12E", 3.14);

	test("%g", 3.14);
	test("%-g", 3.14);
	test("%+g", 3.14);
	test("%0g", 3.14);
	test("%#g", 3.14);
	test("%-0g", 3.14);
	test("%#0g", 3.14);
	test("%#0+g", 3.14);
	test("% g", 3.14);
	test("% -g", 3.14);
	test("% +g", 3.14);
	test("% 0g", 3.14);
	test("% #g", 3.14);
	test("% -0g", 3.14);
	test("% #0g", 3.14);
	test("% #0+g", 3.14);

	test("%.12g", 3.14);
	test("%-.12g", 3.14);
	test("%+.12g", 3.14);
	test("%0.12g", 3.14);
	test("%#.12g", 3.14);
	test("%-0.12g", 3.14);
	test("%#0.12g", 3.14);
	test("%#0+.12g", 3.14);
	test("% .12g", 3.14);
	test("% -.12g", 3.14);
	test("% +.12g", 3.14);
	test("% 0.12g", 3.14);
	test("% #.12g", 3.14);
	test("% -0.12g", 3.14);
	test("% #0.12g", 3.14);
	test("% #0+.12g", 3.14);

	test("%.12G", 3.14);
	test("%-.12G", 3.14);
	test("%+.12G", 3.14);
	test("%0.12G", 3.14);
	test("%#.12G", 3.14);
	test("%-0.12G", 3.14);
	test("%#0.12G", 3.14);
	test("%#0+.12G", 3.14);
	test("% .12G", 3.14);
	test("% -.12G", 3.14);
	test("% +.12G", 3.14);
	test("% 0.12G", 3.14);
	test("% #.12G", 3.14);
	test("% -0.12G", 3.14);
	test("% #0.12G", 3.14);
	test("% #0+.12G", 3.14);

	test("%6f", 3.14);
	test("%-6f", 3.14);
	test("%+6f", 3.14);
	test("%06f", 3.14);
	test("%#6f", 3.14);
	test("%-06f", 3.14);
	test("%#06f", 3.14);
	test("%#0+6f", 3.14);
	test("% 6f", 3.14);
	test("% -6f", 3.14);
	test("% +6f", 3.14);
	test("% 06f", 3.14);
	test("% #6f", 3.14);
	test("% -06f", 3.14);
	test("% #06f", 3.14);
	test("% #0+6f", 3.14);

	test("%6.12f", 3.14);
	test("%-6.12f", 3.14);
	test("%+6.12f", 3.14);
	test("%06.12f", 3.14);
	test("%#6.12f", 3.14);
	test("%-06.12f", 3.14);
	test("%#06.12f", 3.14);
	test("%#0+6.12f", 3.14);
	test("% 6.12f", 3.14);
	test("% -6.12f", 3.14);
	test("% +6.12f", 3.14);
	test("% 06.12f", 3.14);
	test("% #6.12f", 3.14);
	test("% -06.12f", 3.14);
	test("% #06.12f", 3.14);
	test("% #0+6.12f", 3.14);

	test("%6.12F", 3.14);
	test("%-6.12F", 3.14);
	test("%+6.12F", 3.14);
	test("%06.12F", 3.14);
	test("%#6.12F", 3.14);
	test("%-06.12F", 3.14);
	test("%#06.12F", 3.14);
	test("%#0+6.12F", 3.14);
	test("% 6.12F", 3.14);
	test("% -6.12F", 3.14);
	test("% +6.12F", 3.14);
	test("% 06.12F", 3.14);
	test("% #6.12F", 3.14);
	test("% -06.12F", 3.14);
	test("% #06.12F", 3.14);
	test("% #0+6.12F", 3.14);

	test("%6e", 3.14);
	test("%-6e", 3.14);
	test("%+6e", 3.14);
	test("%06e", 3.14);
	test("%#6e", 3.14);
	test("%-06e", 3.14);
	test("%#06e", 3.14);
	test("%#0+6e", 3.14);
	test("% 6e", 3.14);
	test("% -6e", 3.14);
	test("% +6e", 3.14);
	test("% 06e", 3.14);
	test("% #6e", 3.14);
	test("% -06e", 3.14);
	test("% #06e", 3.14);
	test("% #0+6e", 3.14);

	test("%6.12e", 3.14);
	test("%-6.12e", 3.14);
	test("%+6.12e", 3.14);
	test("%06.12e", 3.14);
	test("%#6.12e", 3.14);
	test("%-06.12e", 3.14);
	test("%#06.12e", 3.14);
	test("%#0+6.12e", 3.14);
	test("% 6.12e", 3.14);
	test("% -6.12e", 3.14);
	test("% +6.12e", 3.14);
	test("% 06.12e", 3.14);
	test("% #6.12e", 3.14);
	test("% -06.12e", 3.14);
	test("% #06.12e", 3.14);
	test("% #0+6.12e", 3.14);

	test("%6.12E", 3.14);
	test("%-6.12E", 3.14);
	test("%+6.12E", 3.14);
	test("%06.12E", 3.14);
	test("%#6.12E", 3.14);
	test("%-06.12E", 3.14);
	test("%#06.12E", 3.14);
	test("%#0+6.12E", 3.14);
	test("% 6.12E", 3.14);
	test("% -6.12E", 3.14);
	test("% +6.12E", 3.14);
	test("% 06.12E", 3.14);
	test("% #6.12E", 3.14);
	test("% -06.12E", 3.14);
	test("% #06.12E", 3.14);
	test("% #0+6.12E", 3.14);

	test("%6g", 3.14);
	test("%-6g", 3.14);
	test("%+6g", 3.14);
	test("%06g", 3.14);
	test("%#6g", 3.14);
	test("%-06g", 3.14);
	test("%#06g", 3.14);
	test("%#0+6g", 3.14);
	test("% 6g", 3.14);
	test("% -6g", 3.14);
	test("% +6g", 3.14);
	test("% 06g", 3.14);
	test("% #6g", 3.14);
	test("% -06g", 3.14);
	test("% #06g", 3.14);
	test("% #0+6g", 3.14);

	test("%6.12g", 3.14);
	test("%-6.12g", 3.14);
	test("%+6.12g", 3.14);
	test("%06.12g", 3.14);
	test("%#6.12g", 3.14);
	test("%-06.12g", 3.14);
	test("%#06.12g", 3.14);
	test("%#0+6.12g", 3.14);
	test("% 6.12g", 3.14);
	test("% -6.12g", 3.14);
	test("% +6.12g", 3.14);
	test("% 06.12g", 3.14);
	test("% #6.12g", 3.14);
	test("% -06.12g", 3.14);
	test("% #06.12g", 3.14);
	test("% #0+6.12g", 3.14);

	test("%6.12G", 3.14);
	test("%-6.12G", 3.14);
	test("%+6.12G", 3.14);
	test("%06.12G", 3.14);
	test("%#6.12G", 3.14);
	test("%-06.12G", 3.14);
	test("%#06.12G", 3.14);
	test("%#0+6.12G", 3.14);
	test("% 6.12G", 3.14);
	test("% -6.12G", 3.14);
	test("% +6.12G", 3.14);
	test("% 06.12G", 3.14);
	test("% #6.12G", 3.14);
	test("% -06.12G", 3.14);
	test("% #06.12G", 3.14);
	test("% #0+6.12G", 3.14);
}

TEST_CASE("wprintf")
{
	std::wstringstream ss;
	auto test = [&](auto... v)
	{
		auto s = aprintf(v...);
		printf(ss, v...);
		CHECK(str(ss) == s);
	};

	test(L"%12s", "str");
	test(L"%12.4s", "A long time ago");
	char s[] = "in a galaxy far far away";
	auto p = s;
	test(L"%12.4s", p);

	test(L"%12ls", L"str");
	test(L"%12.4ls", L"There was an old lady called Wright");
	wchar_t ws[] = L" who could travel much faster than light.";
	auto wp = ws;
	test(L"%12.4ls", wp);
}

TEST_CASE("printf extras")
{
	std::stringstream ss;

	printf(ss, "%8s", true);
	REQUIRE(str(ss) == "    true");

	printf(ss, "%8s", false);
	REQUIRE(str(ss) == "   false");

	printf(ss, "%-7s", false);
	REQUIRE(str(ss) == "false  ");

	printf(ss, "%7d", true);
	REQUIRE(str(ss) == "      1");

	printf(ss, "%7.4d", true);
	REQUIRE(str(ss) == "      1");

	printf(ss, "%- 7d", true);
	REQUIRE(str(ss) == "1      ");
}

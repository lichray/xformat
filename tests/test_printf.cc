#include <xformat/ioformat.h>

#include "catch.hpp"
#include "streamtest.h"

using namespace stdex;

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
}

TEST_CASE("wprintf")
{
	std::wstringstream ss;

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

	WHEN("C locale is set to use a different decimal point")
	{
		auto p = []
		{
			if (auto p = setlocale(LC_ALL, "de_DE"))
				return p;
			else
				return setlocale(LC_ALL, "de_DE.UTF-8");
		}();
		if (p == nullptr)
			WARN("Cannot set locale to de_DE");

		THEN("C++ xformat does not follow it")
		{
			if (p)
			{
				auto s = aprintf("%#6a", 3.0);
				REQUIRE(s == "0x1,8p+1");

				printf(ss, "%#6a", 3.0);
				REQUIRE(str(ss) == "0x1.8p+1");
			}
		}

		if (p)
			setlocale(LC_ALL, "C");
	}
}

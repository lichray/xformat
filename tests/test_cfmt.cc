#include <xformat/format.h>

#include "catch.hpp"
#include "fmttest.h"

using namespace stdex;

TEST_CASE("cfmt")
{
	format(fmttest([](fmtshape sp, int w, int p)
	               {
		               REQUIRE(sp.facade() == 's');
		               REQUIRE(sp.options() == fmtoptions::right);
		               REQUIRE(w == -1);
		               REQUIRE(p == -1);
		       }),
	       L"%s"_cfmt, 0);

	format(fmttest([](fmtshape sp, ...)
	               {
		               REQUIRE(sp.facade() == 's');
		               REQUIRE((sp.options() & fmtoptions::left) !=
		                       fmtoptions::none);
		               REQUIRE((sp.options() & fmtoptions::right) !=
		                       fmtoptions::none);
		               REQUIRE((sp.options() & fmtoptions::zero) ==
		                       fmtoptions::none);
		               REQUIRE((sp.options() & fmtoptions::alt) !=
		                       fmtoptions::none);
		               REQUIRE((sp.options() & fmtoptions::sign) !=
		                       fmtoptions::none);
		               REQUIRE(
		                   (sp.options() & fmtoptions::aligned_sign) !=
		                   fmtoptions::none);
		       }),
	       u"%-# + s"_cfmt, 0);

	format(fmttest([](fmtshape sp, ...)
	               {
		               REQUIRE(sp.facade() == 's');
		               REQUIRE((sp.options() & fmtoptions::left) !=
		                       fmtoptions::none);
		               REQUIRE((sp.options() & fmtoptions::zero) !=
		                       fmtoptions::none);
		       }),
	       U"%-0s"_cfmt, 0);

	REQUIRE_THROWS_AS("%h"_cfmt, std::invalid_argument);
	REQUIRE_THROWS_AS("%hld"_cfmt, std::invalid_argument);
	REQUIRE_THROWS_AS("%lhd"_cfmt, std::invalid_argument);
	REQUIRE_THROWS_AS("%ljd"_cfmt, std::invalid_argument);
	REQUIRE_NOTHROW("%jd"_cfmt);
	REQUIRE_NOTHROW("%zu"_cfmt);
	REQUIRE_NOTHROW("%tx"_cfmt);
	REQUIRE_NOTHROW("%Ls"_cfmt);

	format(fmttest([](fmtshape sp, ...)
	               {
		               REQUIRE(sp.facade() == 'd');
		       }),
	       u"%-hhd"_cfmt, 0);

	format(fmttest([](fmtshape sp, ...)
	               {
		               REQUIRE(sp.facade() == 'u');
		       }),
	       u"%lu"_cfmt, 0);
}

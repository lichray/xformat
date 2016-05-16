#include <xformat/ioformat.h>

#include "catch.hpp"
#include "streamtest.h"

TEST_CASE("printf integer")
{
	std::stringstream ss;

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
	test("%+12.d", 0);
	test("%012.d", 0);
	test("%-012.d", 0);
	test("% 12.d", 0);
	test("% -12.d", 0);
	test("% +12.d", 0);
	test("% 012.d", 0);
	test("% -012.d", 0);

	test("%12.i", 0);
	test("%-12.i", 0);
	test("%+12.i", 0);
	test("%012.i", 0);
	test("%-012.i", 0);
	test("% 12.i", 0);
	test("% -12.i", 0);
	test("% +12.i", 0);
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
	test("%#12.o", 0u);
	test("%-012.o", 0u);
	test("%#012.o", 0u);
	test("%#0+12.o", 0u);
	test("% 12.o", 0u);
	test("% -12.o", 0u);
	test("% +12.o", 0u);
	test("% 012.o", 0u);
	test("% #12.o", 0u);
	test("% -012.o", 0u);
	test("% #012.o", 0u);
	test("% #0+12.o", 0u);

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
}

TEST_CASE("printf floating point")
{
	std::stringstream ss;

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

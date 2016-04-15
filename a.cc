#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <experimental/string_view>

#include "visit_at.h"

#include <iostream>

using std::experimental::basic_string_view;

namespace detail
{

// IR
template <typename charT>
struct fmtstack
{
	struct instruction
	{
		unsigned int op;
		union
		{
			int arg;
			struct
			{
				short off1;
				short off2;
			};
			charT ch;
		};
	};

	struct data_entry
	{
		int arg1;
		int arg2;
	};

	union entry
	{
		instruction i;
		data_entry d;
	};

	/*
	struct entry
	{
		int arg1;
		int arg2;
	};

	constexpr int op() const
	{
		return arg1;
	}
	*/

	charT const* start;
	// maximum 9 arguments, 12 raw inputs, 1 null instruction
	// for each extra escape character, sacrifice 1 argument.
	entry line[31];
};

template <typename charT, typename Iter>
constexpr
basic_string_view<charT> raw_string_at(fmtstack<charT> fstk, Iter it)
{
	return { fstk.start + it->i.off1, size_t(it->i.off2 - it->i.off1) };
}

static_assert(sizeof(fmtstack<char>::entry) == 2 * sizeof(int), "");
static_assert(sizeof(fmtstack<char>::line) == 62 * sizeof(int), "");
static_assert(sizeof(fmtstack<char>) <= 64 * sizeof(int), "");

enum op
{
	OP_STOP,
	OP_RAW_S,
	OP_RAW_C,
	OP_S,
	OP_C,
};

template <typename Iter, typename charT>
constexpr
auto findc(Iter first, Iter last, charT c)
{
	auto it = first;
	for (; it != last; ++it)
	{
		if (*it == c)
			break;
	}

	return it;
}

template <typename charT>
constexpr
fmtstack<charT> compile(charT const* s, size_t sz)
{
	fmtstack<charT> fstk { s };
	auto sv = basic_string_view<charT>(s, sz);
	auto it = fstk.line;
	int ac = 0;

	auto bp = sv.begin();

	while (1)
	{
		auto p = findc(bp, sv.end(), '%');
		if (p == bp)
			break;
		else if (p - bp == 1)
		{
			it->i.op = OP_RAW_C;
			it->i.ch = *bp;
		}
		else
		{
			it->i.op = OP_RAW_S;
			it->i.off1 = bp - sv.begin();
			it->i.off2 = p - sv.begin();
		}
		++it;

		if (p != sv.end())
		{
			++p;
			switch (*p)
			{
			case 's':
				it->i.op = OP_S;
				it->i.arg = ac;
				++ac;
				++p;
				break;
			default:
				throw std::invalid_argument
				{
				    "unknown format specifier"
				};
			}
			// not appending arg1 arg2
			++it;
		}
		++it;
		bp = p;
	}

	it->i.op = OP_STOP;

	return fstk;
}

}

constexpr
auto operator"" _fmt(char const* s, size_t sz)
{
	return detail::compile(s, sz);
}

template <typename charT, typename... Args>
inline
void printf_vm(detail::fmtstack<charT> fstk, Args&&... args)
{
	using namespace detail;

	for (auto it = fstk.line; it->i.op != OP_STOP; ++it)
	{
		switch (it->i.op)
		{
		case OP_RAW_S:
			std::cout << raw_string_at(fstk, it);
			break;
		case OP_RAW_C:
			std::cout.put(it->i.ch);
			break;
		case OP_S:
			visit_at(it->i.arg, [&](auto&& x)
			    {
				std::cout << x;
			    }, std::forward_as_tuple(args...));
			++it;
			break;
		case OP_C:
			++it;
			break;
		}
	}
}

int main()
{
	auto x = "hello, %s\n"_fmt;
	printf_vm(x, "nice");
}

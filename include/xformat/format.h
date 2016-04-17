/*-
 * Copyright (c) 2016 Zhihao Yuan.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once

#include <assert.h>
#include <limits>
#include <experimental/string_view>

#include "tuple.h"

namespace stdex
{

using std::experimental::basic_string_view;

namespace detail
{

using std::begin;
using std::end;

enum op_type
{
	OP_RAW_S = 0x1,
	OP_RAW_C,
	OP_S,
	OP_C,
};

struct entry
{
	constexpr auto op() const
	{
		return op_type(op_);
	}

	unsigned int op_;
	int arg;
	int arg1;
	int arg2;
};

}

template <typename charT>
struct fmtstack
{
	using iterator = detail::entry const*;

	constexpr iterator begin() const
	{
		return line;
	}

	constexpr iterator end() const
	{
		return line + size;
	}

	basic_string_view<charT> raw_string(detail::entry const& x) const
	{
		auto hi = x.arg1;
		auto lo = x.arg2;
		return { start + hi, size_t(lo - hi) };
	}

	static charT raw_char(detail::entry const& x)
	{
		assert(x.op() == detail::OP_RAW_C);
		return charT(x.arg);
	}

	charT const* start;
	size_t size;
	// maximum 9 arguments, 10 raw inputs
	// for each extra escape character, sacrifice 1 argument.
	detail::entry line[19];
};

static_assert(sizeof(fmtstack<char>) <= 80 * sizeof(int), "");

namespace detail
{

template <typename Iter>
constexpr
auto instruction(Iter from, Iter first, Iter last)
{
	assert(from <= first and first <= last);
	if (last - from > std::numeric_limits<int>::max())
		throw std::length_error{ "raw string is too long" };

	return entry{ OP_RAW_S, {}, int(first - from), int(last - from) };
}

template <typename charT>
constexpr
auto instruction(charT ch)
{
	return entry{ OP_RAW_C, int(ch) };
}

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
fmtstack<charT> compile_c(charT const* s, size_t sz)
{
	fmtstack<charT> fstk { s };
	auto sv = basic_string_view<charT>(s, sz);
	auto it = fstk.line;
	int ac = 0;

	auto bp = sv.begin();

	while (bp != sv.end())
	{
		auto p = findc(bp, sv.end(), '%');
		if (p == bp)
			;
		else if (p - bp == 1)
		{
			*it = instruction(*bp);
			++it;
			++fstk.size;
		}
		else
		{
			*it = instruction(sv.begin(), bp, p);
			++it;
			++fstk.size;
		}

		if (p != sv.end())
		{
			++p;
			switch (*p)
			{
			case 's':
				++ac;
				*it = entry{ OP_S, ac };
				++p;
				break;
			default:
				throw std::invalid_argument
				{
				    "unknown format specifier"
				};
			}
			++it;
			++fstk.size;
		}
		bp = p;
	}

	return fstk;
}

}

constexpr
auto operator"" _cfmt(char const* s, size_t sz)
{
	return detail::compile_c(s, sz);
}

template <typename charT, typename Formatter, typename... Args>
inline
decltype(auto) format(Formatter fter, fmtstack<charT> const& fstk,
                      Args&&... args)
{
	using namespace detail;

	for (auto&& et : fstk)
	{
		switch (et.op())
		{
		case OP_RAW_S:
			fter.send(fstk.raw_string(et));
			break;
		case OP_RAW_C:
			fter.send(fstk.raw_char(et));
			break;
		case OP_S:
			visit1_at(et.arg,
			          [&](auto&& x)
			          {
				          fter.format(x);
				  },
			          std::forward_as_tuple(args...));
			break;
		case OP_C:
			break;
		}
	}

	return fter.state();
}

}

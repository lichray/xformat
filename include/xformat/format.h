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
#include "gliteral.h"

namespace stdex
{

using std::experimental::basic_string_view;
using std::enable_if_t;

enum class fmtoptions
{
	none,
	left = 0x01,
	right = 0x02,
	sign = 0x04,
	aligned_sign = 0x08,
	alt = 0x10,
	zero = 0x20,
};

constexpr
fmtoptions operator&(fmtoptions __x, fmtoptions __y)
{
	return fmtoptions(int(__x) & int(__y));
}

constexpr
fmtoptions operator|(fmtoptions __x, fmtoptions __y)
{
	return fmtoptions(int(__x) | int(__y));
}

constexpr
fmtoptions operator^(fmtoptions __x, fmtoptions __y)
{
	return fmtoptions(int(__x) ^ int(__y));
}

constexpr
fmtoptions
operator~(fmtoptions __x)
{
	return fmtoptions(~int(__x) & 0x3f);
}

constexpr
fmtoptions& operator&=(fmtoptions& __x, fmtoptions __y)
{
	__x = __x & __y;
	return __x;
}

constexpr
fmtoptions& operator|=(fmtoptions& __x, fmtoptions __y)
{
	__x = __x | __y;
	return __x;
}

constexpr
fmtoptions& operator^=(fmtoptions& __x, fmtoptions __y)
{
	__x = __x ^ __y;
	return __x;
}

struct fmtshape
{
	constexpr auto facade() const
	{
		return ch_;
	}

	constexpr auto options() const
	{
		return fmtoptions(opts_);
	}

	constexpr void operator=(char ch)
	{
		ch_ = ch;
	}

	constexpr void operator=(fmtoptions opts)
	{
		opts_ = (unsigned char)opts;
	}

	constexpr void operator|=(fmtoptions opts)
	{
		opts_ |= (unsigned char)opts;
	}

private:
	char ch_ = '\0';
	unsigned char opts_{};
};

namespace detail
{

enum op_type
{
	OP_RAW_S = 1,
	OP_RAW_C,
	OP_FMT,
};

enum op_attr
{
	REG_ARG1 = 0b0100,
	REG_ARG2 = 0b1000,
};

struct entry
{
	constexpr auto opcode() const
	{
		return op_type(op_ & 0b11);
	}

	constexpr bool has(op_attr attr) const
	{
		return (op_ & attr) != 0;
	}

	unsigned short op_;
	fmtshape shape;
	int arg;
	int arg1;
	int arg2;
};

}

template <typename charT>
struct fmtstack
{
	using iterator = detail::entry const*;

	constexpr explicit fmtstack(charT const* s) : start(s)
	{
	}

	constexpr iterator begin() const
	{
		return line;
	}

	constexpr iterator end() const
	{
		return line + size;
	}

	static constexpr size_t max_size()
	{
		return sizeof(line) / sizeof(detail::entry);
	}

	constexpr void push(detail::entry x)
	{
		if (size == max_size())
			throw std::length_error{ "format string too complex" };

		line[size++] = std::move(x);
	}

	constexpr auto raw_string(detail::entry const& x) const
	    -> basic_string_view<charT>
	{
		auto hi = x.arg1;
		auto lo = x.arg2;
		return { start + hi, size_t(lo - hi) };
	}

	static constexpr charT raw_char(detail::entry const& x)
	{
		assert(x.opcode() == detail::OP_RAW_C);
		return charT(x.arg);
	}

private:
	charT const* start;
	size_t size = 0;
	// maximum 9 arguments, 10 raw inputs
	// for each extra escape character, sacrifice 1 argument.
	detail::entry line[19] = {};
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

	return entry{ OP_RAW_S, {}, {}, int(first - from), int(last - from) };
}

template <typename charT>
constexpr
auto instruction(charT ch)
{
	return entry{ OP_RAW_C, {}, int(ch) };
}

template <typename Iter, typename charT>
constexpr
auto findc(Iter first, Iter last, charT c)
{
	for (; first != last; ++first)
	{
		if (*first == c)
			return first;
	}

	return last;
}

template <typename charT>
struct bounded_reader
{
	using pointer = charT const*;

	constexpr bounded_reader(pointer p, size_t sz) : cur_(p), end_(p + sz)
	{
	}

	constexpr explicit operator bool() const
	{
		return not empty();
	}

	constexpr bool empty() const
	{
		return cur_ == end_;
	}

	constexpr void incr()
	{
		++cur_;
	}

	constexpr auto read()
	{
		return *cur_++;
	}

	constexpr auto find(charT c) const
	{
		return findc(cur_, end_, c);
	}

	constexpr auto operator*() const
	{
		return *ptr();
	}

	constexpr auto ptr() const
	{
		return cur_;
	}

	constexpr void ptr(pointer p)
	{
		cur_ = p;
	}

	constexpr auto eptr() const
	{
		return end_;
	}

private:
	pointer cur_;
	pointer end_;
};

template <typename charT>
constexpr
bool is_1to9(charT c)
{
	return STDEX_G(charT, '1') <= c and c <= STDEX_G(charT, '9');
}

template <typename charT>
constexpr
bool is_0to9(charT c)
{
	return STDEX_G(charT, '0') <= c and c <= STDEX_G(charT, '9');
}

template <typename charT>
constexpr
int to_int(charT c)
{
	return int(c - STDEX_G(charT, '0'));
}

template <typename charT>
constexpr
int parse_int(bounded_reader<charT>& r)
{
	int n = 0;
	for (; r and is_0to9(*r); r.incr())
	{
		n *= 10;
		n += to_int(*r);
	}
	return n;
}

template <typename charT>
constexpr
int parse_position(bounded_reader<charT>& r)
{
	int n = to_int(r.read());
	if (n < 1 or n > 9 or r.empty() or r.read() != STDEX_G(charT, '$'))
		throw std::invalid_argument{ "index is not 1-9$" };
	return n;
}

template <typename charT>
constexpr
auto parse_flags_c(bounded_reader<charT>& r)
{
	fmtshape sp;
	sp = fmtoptions::right;

	for (; r; r.incr())
	{
		// not clearing conflicting flags; the formatters have rights
		// to interpret them.
		switch (*r)
		{
		case STDEX_G(charT, '-'):
			sp |= fmtoptions::left;
			break;
		case STDEX_G(charT, '+'):
			sp |= fmtoptions::sign;
			break;
		case STDEX_G(charT, ' '):
			sp |= fmtoptions::aligned_sign;
			break;
		case STDEX_G(charT, '#'):
			sp |= fmtoptions::alt;
			break;
		case STDEX_G(charT, '0'):
			sp |= fmtoptions::zero;
			break;
		default:
			return sp;
		}
	}

	return sp;
}

template <typename charT>
constexpr
fmtstack<charT> compile_c(charT const* s, size_t sz)
{
	using std::invalid_argument;

	fmtstack<charT> fstk { s };
	bounded_reader<charT> r(s, sz);
	int ac = 0;
	bool sequential{};  // expects: ac > 0

	while (r)
	{
		auto np = r.find(STDEX_G(charT, '%'));

		// process escaping first
		if (np != r.eptr())
		{
			if (np + 1 == r.eptr())
				throw invalid_argument{ "single '%'" };

			if (np[1] == STDEX_G(charT, '%'))
			{
				if (np++ == r.ptr())
					fstk.push(instruction(*np));
				else
					fstk.push(instruction(s, r.ptr(), np));

				r.ptr(np + 1);
				continue;
			}
		}

		if (auto d = np - r.ptr())
		{
			if (d == 1)
				fstk.push(instruction(*r));
			else
				fstk.push(instruction(s, r.ptr(), np));
		}

		r.ptr(np);
		if (r.empty())
			break;

		r.incr();

		// assert: !r.empty()
		if (ac == 0)
			sequential = !is_1to9(*r);

		if (sequential)
			++ac;
		else
			ac = parse_position(r);

		auto sp = parse_flags_c(r);

		unsigned short op = OP_FMT;
		int width = -1;
		int precision = -1;

		// field width
		if (r)
		{
			if (is_1to9(*r))
				width = parse_int(r);
			else if (*r == STDEX_G(charT, '*'))
			{
				r.incr();
				op |= REG_ARG1;
				width = sequential ? ac++ : parse_position(r);
			}
		}

		// precision
		if (r and *r == STDEX_G(charT, '.'))
		{
			r.incr();
			if (r and *r == STDEX_G(charT, '*'))
			{
				r.incr();
				op |= REG_ARG2;
				precision =
				    sequential ? ac++ : parse_position(r);
			}
			else
				precision = parse_int(r);
		}

		// ignore all length modifiers
		if (r)
		{
			switch (auto c = *r)
			{
			case STDEX_G(charT, 'h'):
			case STDEX_G(charT, 'l'):
				r.incr();
				if (r and *r == c)
					r.incr();
				break;
			case STDEX_G(charT, 'j'):
			case STDEX_G(charT, 'z'):
			case STDEX_G(charT, 't'):
			case STDEX_G(charT, 'L'):
				r.incr();
				break;
			}
		}

		if (r.empty())
			throw invalid_argument{ "incomplete specification" };

		switch (r.read())
		{
		case STDEX_G(charT, 'd'):
			sp = 'd';
			break;
		case STDEX_G(charT, 'i'):
			sp = 'i';
			break;
		case STDEX_G(charT, 'o'):
			sp = 'o';
			break;
		case STDEX_G(charT, 'u'):
			sp = 'u';
			break;
		case STDEX_G(charT, 'x'):
			sp = 'x';
			break;
		case STDEX_G(charT, 'X'):
			sp = 'X';
			break;
		case STDEX_G(charT, 'f'):
			sp = 'f';
			break;
		case STDEX_G(charT, 'F'):
			sp = 'F';
			break;
		case STDEX_G(charT, 'e'):
			sp = 'e';
			break;
		case STDEX_G(charT, 'E'):
			sp = 'E';
			break;
		case STDEX_G(charT, 'g'):
			sp = 'g';
			break;
		case STDEX_G(charT, 'G'):
			sp = 'G';
			break;
		case STDEX_G(charT, 'a'):
			sp = 'a';
			break;
		case STDEX_G(charT, 'A'):
			sp = 'A';
			break;
		case STDEX_G(charT, 'c'):
			sp = 'c';
			break;
		case STDEX_G(charT, 's'):
			sp = 's';
			break;
		case STDEX_G(charT, 'p'):
			sp = 'p';
			break;
		default:
			throw invalid_argument{ "unknown format specifier" };
		}

		fstk.push({ op, sp, ac, width, precision });
	}

	return fstk;
}

#ifndef XFORMAT_HEADER_ONLY

extern template fmtstack<char> compile_c(char const*, size_t);
extern template fmtstack<wchar_t> compile_c(wchar_t const*, size_t);
extern template fmtstack<char16_t> compile_c(char16_t const*, size_t);
extern template fmtstack<char32_t> compile_c(char32_t const*, size_t);

#endif

template <typename T, typename = void>
struct do_int_cast
{
	[[noreturn]] static int fn(T const&)
	{
		throw std::invalid_argument{ "not an integer" };
	}
};

template <typename T>
struct do_int_cast<T, enable_if_t<std::is_convertible<T, int>::value and
                                  not std::is_floating_point<T>::value>>
{
	constexpr static int fn(_param_type_t<T> v)
	{
		return static_cast<int>(v);
	}
};

struct int_cast
{
	template <typename T>
	constexpr int operator()(T const& v) const
	{
		return do_int_cast<T>::fn(v);
	}
};

template <typename charT, typename Formatter, typename Tuple>
inline
decltype(auto) vformat(Formatter fter, fmtstack<charT> const& fstk, Tuple tp)
{
	for (auto&& et : fstk)
	{
		switch (et.opcode())
		{
		case OP_RAW_S:
			fter.send(fstk.raw_string(et));
			break;
		case OP_RAW_C:
			fter.send(fstk.raw_char(et));
			break;
		case OP_FMT:
			visit1_at(
			    et.arg,
			    [
			      shape = et.shape,
			      w = et.has(REG_ARG1)
				      ? visit1_at<int>(et.arg1, int_cast(), tp)
				      : et.arg1,
			      p = et.has(REG_ARG2)
				      ? visit1_at<int>(et.arg2, int_cast(), tp)
				      : et.arg2,
			      &fter
			    ](auto&& x)
			    {
				    fter.format(shape, w, p, x);
			    },
			    tp);
			break;
		}
	}

	return fter.state();
}

}

template <typename charT, typename Formatter, typename... Args>
inline
decltype(auto) format(Formatter fter, fmtstack<charT> const& fstk,
                      Args&&... args)
{
	return detail::vformat(fter, fstk, as_tuple_of_cref(args...));
}

inline namespace literals
{
constexpr
auto operator"" _cfmt(char const* s, size_t sz)
{
	return detail::compile_c(s, sz);
}

constexpr
auto operator"" _cfmt(wchar_t const* s, size_t sz)
{
	return detail::compile_c(s, sz);
}

constexpr
auto operator"" _cfmt(char16_t const* s, size_t sz)
{
	return detail::compile_c(s, sz);
}

constexpr
auto operator"" _cfmt(char32_t const* s, size_t sz)
{
	return detail::compile_c(s, sz);
}
}

}

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

#include <iosfwd>
#include <sstream>
#include <algorithm>
#include <experimental/string_view>

#include "gliteral.h"

namespace stdex
{

using std::experimental::basic_string_view;
using std::enable_if_t;

template <typename charT, typename traits>
struct ostream_outputter
{
	using ostream_type = std::basic_ostream<charT, traits>;

	explicit ostream_outputter(ostream_type& out) : out_(out)
	{
	}

	void send(charT ch)
	{
		state().put(ch);
	}

	void send(basic_string_view<charT, traits> sv)
	{
		state().write(sv.data(), std::streamsize(sv.size()));
	}

	ostream_type& state()
	{
		return out_;
	}

private:
	ostream_type& out_;
};

template <typename T>
struct superficial
{
	using type = T;
};

template <typename T>
struct superficial<T const*> : superficial<T*>
{
	using type = T*;
};

template <typename charT, typename traits>
struct ostream_formatter : ostream_outputter<charT, traits>
{
	using outputter_type = ostream_outputter<charT, traits>;
	using outputter_type::ostream_outputter;
	using outputter_type::state;

	template <typename T>
	auto format(fmtshape shape, int width, int precision, T const& v)
	    -> enable_if_t<not std::is_arithmetic<T>::value>
	{
		print(shape, width, precision, v, superficial<T>());
	}

	void format(fmtshape sp, int w, int p,
	            basic_string_view<charT, traits> sv)
	{
		print_string_ref(sp, w, p, sv);
	}

	template <typename Allocator>
	void format(fmtshape sp, int w, int p,
	            std::basic_string<charT, traits, Allocator> const& s)
	{
		print_string_ref(sp, w, p, s);
	}

	void format(fmtshape sp, int w, int p, bool v)
	{
		if (sp.facade() == 's')
			print_string_ref(sp, w, p, v, os::boolalpha);
		else
			potentially_formattable(sp, w, p, v);
	}

	template <typename T>
	auto format(fmtshape sp, int w, int p, T v)
	    -> enable_if_t<std::is_signed<T>::value and
	                   std::is_integral<T>::value>
	{
		switch (sp.facade())
		{
		case 'u':
		case 'o':
		case 'x':
		case 'X':
			return format(sp, w, p, std::make_unsigned_t<T>(v));
		}

		print_signed(sp, w, p, v);
	}

	template <typename T>
	auto format(fmtshape sp, int w, int p, T v)
	    -> enable_if_t<std::is_floating_point<T>::value>
	{
		print_signed(sp, w, p, v);
	}

	template <typename T>
	auto format(fmtshape sp, int w, int p, T v)
	    -> enable_if_t<std::is_unsigned<T>::value>
	{
		print_basic_arithmetic(sp, w, p, v);
	}

private:
	using os = typename outputter_type::ostream_type;
	using fmtflags = typename os::fmtflags;
	using view_type = basic_string_view<charT, traits>;

	template <typename T>
	void print(fmtshape sp, int w, int p, T const& v, ...)
	{
		potentially_formattable(sp, w, p, v);
	}

	template <typename T>
	void print(fmtshape sp, int w, int p, T s, superficial<charT*>)
	{
		if (p == -1 or sp.facade() != 's')
			print_string_ref(sp, w, p, s);
		else
			print_string_ref(sp, w, p, view_type(s, size_t(p)));
	}

	template <typename T,
	          typename = enable_if_t<
	              not std::is_same<typename superficial<T>::type,
	                               charT*>::value>>
	void print(fmtshape sp, int w, int p, T s, superficial<char*>)
	{
		if (p == -1 or sp.facade() != 's')
			print_string_ref(sp, w, p, s);
		else
			print_string_ref(sp, w, p,
			                 std::string(s, size_t(p)).data());
	}

	template <typename T>
	void potentially_formattable(fmtshape sp, int w, int p, T const& v,
	                             fmtflags fl = {}, bool intp = false)
	{
		fl |= base_flags();
		if (w != -1)
			state().width(w);
		state().precision(p);

		if (has(sp, fmtoptions::sign))
			fl |= os::showpos;
		if (has(sp, fmtoptions::alt))
			fl |= os::showbase | os::showpoint;
		if (has(sp, fmtoptions::left))
			fl |= os::left;
		else if (has(sp, fmtoptions::zero) and not intp)
		{
			auto fc = state().fill(STDEX_G(charT, '0'));
			state().flags(fl | os::internal | to_flags(sp));
			state() << v;
			state().fill(fc);
			return;
		}
		else if (has(sp, fmtoptions::right))
			fl |= os::right;

		state().flags(fl | to_flags(sp));
		state() << v;
	}

	template <typename T>
	__attribute__((always_inline))
	void print_string_ref(fmtshape sp, int w, int p, T const& v,
	                      fmtflags fl = {})
	{
		fl |= base_flags();
		if (has(sp, fmtoptions::left))
			fl |= os::left;
		state().flags(fl);
		if (w != -1)
			state().width(w);
		state() << v;
	}

	template <typename T>
	auto print_basic_arithmetic(fmtshape sp, int w, int p, T v,
	                            fmtflags fl = {})
	    -> enable_if_t<std::is_floating_point<T>::value>
	{
		potentially_formattable(sp, w, p, v, fl);
	}

	template <typename T>
	auto print_basic_arithmetic(fmtshape sp, int w, int p, T v,
	                            fmtflags fl = {})
	    -> enable_if_t<std::is_integral<T>::value>
	{
		int d;
		switch (sp.facade())
		{
		case 'd':
		case 'i':
		case 'u':
			d = lexical_width<10>(v);
			break;
		case 'o':
			d = lexical_width<8>(v) + has(sp, fmtoptions::alt);
			break;
		case 'x':
		case 'X':
			d = lexical_width<16>(v);
			break;
		default:
			return potentially_formattable(sp, w, p, v, fl);
		}

		if (p == 0 and v == 0)
			return print_string_ref(sp, w, p, view_type());
		if (p <= d)
			return potentially_formattable(sp, w, p, v, fl,
			                               p != -1);

		std::basic_stringstream<charT, traits> dout;
		state().copyfmt(dout);
		ostream_formatter(dout)
		    .potentially_formattable(sp, 0, p, v, fl, true);
		auto s = dout.str();
		s.insert(s.size() - size_t(d), size_t(p - d),
		         STDEX_G(charT, '0'));
		print_string_ref(sp, w, p, s);
	}

	template <int Base, typename Int>
	static int lexical_width(Int i)
	{
		if (i == 0)
			return 1;

		int n = 0;
		while (i != 0)
		{
			i /= Base;
			++n;
		}
		return n;
	}

	template <typename T>
	void print_signed(fmtshape sp, int w, int p, T v)
	{
		if (has(sp, fmtoptions::sign) or
		    not has(sp, fmtoptions::aligned_sign))
			return print_basic_arithmetic(sp, w, p, v);

		std::basic_stringstream<charT, traits> dout;
		state().copyfmt(dout);
		ostream_formatter(dout)
		    .print_basic_arithmetic(sp, w, p, v, os::showpos);
		auto s = dout.str();
		auto it = std::find(s.begin(), s.end(), STDEX_G(charT, '+'));
		if (it != s.end())
			*it = dout.fill();
		state().write(s.data(), std::streamsize(s.size()));
	}

	fmtflags base_flags()
	{
		return state().flags() & os::unitbuf;
	}

	constexpr static fmtflags to_flags(fmtshape sp)
	{
		switch (sp.facade())
		{
		case 'd':
		case 'u':
			return os::dec;
		case 'o':
			return os::oct;
		case 'x':
			return os::hex;
		case 'X':
			return os::hex | os::uppercase;
		case 'f':
			return os::fixed;
		case 'F':
			return os::fixed | os::uppercase;
		case 'e':
			return os::scientific;
		case 'E':
			return os::scientific | os::uppercase;
		case 'G':
			return os::uppercase;
		case 'a':
			return os::fixed | os::scientific;
		case 'A':
			return os::fixed | os::scientific | os::uppercase;
		case 's':
			return os::boolalpha;
		default:
			return {};
		}
	}

	constexpr static auto has(fmtshape sp, fmtoptions opt)
	{
		return (sp.options() & opt) != fmtoptions::none;
	}
};

}

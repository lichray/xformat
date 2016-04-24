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
#include <experimental/string_view>

namespace stdex
{

using std::experimental::basic_string_view;

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

template <typename charT, typename traits>
struct ostream_formatter : ostream_outputter<charT, traits>
{
	using outputter_type = ostream_outputter<charT, traits>;
	using outputter_type::ostream_outputter;

	template <typename T>
	void format(fmtshape shape, int width, int precision, T const& v)
	{
		this->state() << v;
	}
};

}

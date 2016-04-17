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

#include <tuple>

namespace stdex
{

template <int Low, int High, int Mid = (Low + High) / 2, typename = void>
struct _visit1_at;

template <int Low, int High, int Mid>
struct _visit1_at<Low, High, Mid, std::enable_if_t<(Low > High)>>
{
	template <typename... T>
	static decltype(auto) apply(int, T&&...)
	{
	}
};

template <int Mid>
struct _visit1_at<Mid, Mid, Mid>
{
	template <typename Tuple, typename F>
	static decltype(auto) apply(int n, F&& f, Tuple&& tp)
	{
		return std::forward<F>(f)(
		    std::get<Mid - 1>(std::forward<Tuple>(tp)));
	}
};

template <int Low, int High, int Mid>
struct _visit1_at<Low, High, Mid, std::enable_if_t<(Low < High)>>
{
	template <typename... T>
	static decltype(auto) apply(int n, T&&... t)
	{
		if (n < Mid)
			return _visit1_at<Low, Mid - 1>::apply(
			    n, std::forward<T>(t)...);
		else if (n == Mid)
			return _visit1_at<Mid, Mid>::apply(
			    n, std::forward<T>(t)...);
		else
			return _visit1_at<Mid + 1, High>::apply(
			    n, std::forward<T>(t)...);
	}
};

template <typename Tuple, typename F>
inline
decltype(auto) visit1_at(int n, F&& f, Tuple&& tp)
{
	constexpr int m = std::tuple_size<std::decay_t<Tuple>>::value;
	if (n < 1 or n > m)
		throw std::out_of_range{ "no such element" };

	return _visit1_at<1, m>::apply(n, std::forward<F>(f),
	                               std::forward<Tuple>(tp));
}

}

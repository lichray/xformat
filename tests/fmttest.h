#pragma once

#include <functional>
#include <experimental/string_view>

using std::experimental::string_view;

struct fmttest
{
	template <typename F,
	          typename = std::enable_if_t<
	              not std::is_base_of<fmttest,
	                                  std::remove_reference_t<F>>::value>>
	explicit fmttest(F&& f)
	    : f_(std::forward<F>(f))
	{
	}

	template <typename T>
	void send(T)
	{
	}

	void state()
	{
	}

	template <typename T>
	void format(stdex::fmtshape shape, int width, int precision, T const&)
	{
		f_(shape, width, precision);
	}

private:
	std::function<void(stdex::fmtshape, int, int)> f_;
};
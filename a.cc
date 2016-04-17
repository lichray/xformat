#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <tuple>
#include <assert.h>
#include <experimental/string_view>

#include <iostream>

using std::experimental::basic_string_view;

namespace detail
{

using std::begin;
using std::end;

enum op_type
{
	OP_STOP,
	OP_RAW_S,
	OP_RAW_C,
	OP_S,
	OP_C,
};

struct entry
{
	constexpr auto op() const
	{
		return op_type(arg1);
	}

	constexpr auto arg() const
	{
		return int(arg2);
	}

	constexpr auto hi() const
	{
		assert(op() == OP_RAW_S);
		return arg2 >> 16;
	}

	constexpr auto lo() const
	{
		assert(op() == OP_RAW_S);
		return arg2 & 0xffff;
	}

	unsigned int arg1;
	unsigned int arg2;
};

template <typename charT>
constexpr auto ch(entry const& x);

// IR
template <typename charT>
struct fmtstack
{
	using iterator = entry const*;

	constexpr iterator begin() const
	{
		return line;
	}

	struct sentinel
	{
		friend bool operator==(iterator it, sentinel st)
		{
			return it->op() == OP_STOP;
		}

		friend bool operator!=(iterator it, sentinel st)
		{
			return !(it == st);
		}
	};

	constexpr sentinel end() const
	{
		return {};
	}

	basic_string_view<charT> raw_string(entry const& x) const
	{
		auto hi = x.hi();
		auto lo = x.lo();
		return { start + hi, lo - hi };
	}

	static charT raw_char(entry const& x)
	{
		assert(x.op() == OP_RAW_C);
		return charT(x.arg2);
	}

	charT const* start;
	// maximum 9 arguments, 12 raw inputs, 1 null instruction
	// for each extra escape character, sacrifice 1 argument.
	entry line[31];
};

static_assert(sizeof(fmtstack<char>::line) == 62 * sizeof(int), "");
static_assert(sizeof(fmtstack<char>) <= 64 * sizeof(int), "");

constexpr
entry instruction(op_type op, int arg)
{
	return { op, unsigned(arg) };
}

constexpr
auto instruction()
{
	return instruction(OP_STOP, {});
}

template <typename Iter>
constexpr
auto instruction(Iter from, Iter first, Iter last)
{
	assert(from <= first and first <= last);
	if (last - from > 0xffff)
		throw std::length_error
		{
		    "raw string is too long"
		};

	return instruction(OP_RAW_S, ((first - from) << 16) ^ (last - from));
}

template <typename charT>
constexpr
auto instruction(charT ch)
{
	return entry{ OP_RAW_C, unsigned(ch) };
}

constexpr
entry data_entry(int arg1, int arg2)
{
	return { unsigned(arg1), unsigned(arg2) };
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
			*it = instruction(*bp);
		}
		else
		{
			*it = instruction(sv.begin(), bp, p);
		}
		++it;

		if (p != sv.end())
		{
			++p;
			switch (*p)
			{
			case 's':
				++ac;
				*it = instruction(OP_S, ac);
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

	*it = instruction();

	return fstk;
}

}

constexpr
auto operator"" _fmt(char const* s, size_t sz)
{
	return detail::compile(s, sz);
}

template <int Low, int High, int Mid = (Low + High) / 2, typename = void>
struct _visit1_at;

template <int Low, int High, int Mid>
struct _visit1_at<Low, High, Mid, std::enable_if_t<(Low > High)>>
{
	template <typename... T>
	static decltype(auto) apply(int, T&&...) {}
};

template <int Mid>
struct _visit1_at<Mid, Mid, Mid>
{
	template <typename Tuple, typename F>
	static decltype(auto) apply(int n, F&& f, Tuple&& tp) {
		return std::forward<F>(f)(
		    std::get<Mid - 1>(std::forward<Tuple>(tp)));
	}
};

template <int Low, int High, int Mid>
struct _visit1_at<Low, High, Mid, std::enable_if_t<(Low < High)>>
{
	template <typename... T>
	static decltype(auto) apply(int n, T&&... t) {
		if (n < Mid)
			return _visit1_at<Low, Mid - 1>::apply(n,
			    std::forward<T>(t)...);
		else if (n == Mid)
			return _visit1_at<Mid, Mid>::apply(n,
			    std::forward<T>(t)...);
		else
			return _visit1_at<Mid + 1, High>::apply(n,
			    std::forward<T>(t)...);
	}
};

template <typename Tuple, typename F>
inline
decltype(auto) visit1_at(int n, F&& f, Tuple&& tp) {
	constexpr int m = std::tuple_size<std::decay_t<Tuple>>::value;
	if (n > m)
		throw std::out_of_range
		{
		    "no such argument"
		};

	return _visit1_at<1, m>::apply(n, std::forward<F>(f),
	    std::forward<Tuple>(tp));
}

template <typename charT, typename... Args>
inline
void printf_vm(detail::fmtstack<charT> fstk, Args&&... args)
{
	using namespace detail;

	for (auto it = fstk.begin();; ++it)
	{
		switch (it->op())
		{
		case OP_STOP:
			return;
		case OP_RAW_S:
			{
				auto sv = fstk.raw_string(*it);
				std::cout.write(sv.data(), sv.size());
			}
			break;
		case OP_RAW_C:
			std::cout.put(fstk.raw_char(*it));
			break;
		case OP_S:
			visit1_at(it->arg(), [&](auto&& x)
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
	//constexpr auto x = "hello, %s\n"_fmt;
	auto x = "hello, %s\n"_fmt;
	printf_vm(x, "nice");
}

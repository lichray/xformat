#include <tuple>
#include <type_traits>

template <int Low, int High, int Mid = (Low + High) / 2, typename = void>
struct _visit_at;

template <int Low, int High, int Mid>
struct _visit_at<Low, High, Mid, std::enable_if_t<(Low > High)>>
{
	template <typename... T>
	static
	decltype(auto) apply(int, T&&...)
	{
		throw std::out_of_range("visit_at");
	}
};

template <int Mid>
struct _visit_at<Mid, Mid, Mid>
{
	template <typename Tuple, typename F>
	static
	decltype(auto) apply(int n, F&& f, Tuple&& tp)
	{
		if (n != Mid)
			throw std::out_of_range("visit_at");

		// use std::invoke
		return std::forward<F>(f)(
		    std::get<Mid>(std::forward<Tuple>(tp)));
	}
};

template <int Low, int High, int Mid>
struct _visit_at<Low, High, Mid, std::enable_if_t<(Low < High)>>
{
	template <typename... T>
	static
	decltype(auto) apply(int n, T&&... t)
	{
		if (n < Mid)
			return _visit_at<Low, Mid - 1>::apply(n,
			    std::forward<T>(t)...);
		else if (n == Mid)
			return _visit_at<Mid, Mid>::apply(n,
			    std::forward<T>(t)...);
		else
			return _visit_at<Mid + 1, High>::apply(n,
			    std::forward<T>(t)...);
	}
};

template <typename Tuple, typename F>
inline
decltype(auto) visit_at(int n, F&& f, Tuple&& tp)
{
	return _visit_at<0, int(std::tuple_size<std::decay_t<Tuple>>{}) - 1>
	    ::apply(n, std::forward<F>(f), std::forward<Tuple>(tp));
}

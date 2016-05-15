#pragma once

template <typename T>
inline
auto str(T& ss)
{
	auto s = ss.str();
	ss.str({});
	return s;
}

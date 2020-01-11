#pragma once
#include <future>

template<typename L> auto __await_async(L lambda)->decltype(lambda())
{
	auto f = std::async(std::launch::deferred, [=]()
	{
		auto r = lambda();
		return r;
	});

	return f.get();
}
#define await(l) __await_async(l)

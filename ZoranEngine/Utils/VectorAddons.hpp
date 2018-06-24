#pragma once
#include <vector>

template <typename T>
void remove(std::vector<T> &v, size_t i)
{
	std::swap(v[i], v.back());
	v.pop_back();
}

template <typename T>
T remove(std::vector<T> &v, T object)
{
	auto iter = std::find(v.begin(),v.end(),object);
	if (iter != v.end())
	{
		size_t i = (iter - v.begin());

		std::swap(v[i], v.back());
		v.pop_back();

		return object;
	}
	return NULL;
}

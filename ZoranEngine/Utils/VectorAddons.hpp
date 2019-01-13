#pragma once
#include <vector>

template <typename T>
void remove(std::vector<T> &v, size_t i)
{
	std::swap(v[i], v.back());
	v.pop_back();
}

template <typename T>
bool remove(std::vector<T> &v,typename std::vector<T>::iterator iter)
{
	size_t i = (iter - v.begin());

	std::swap(v[i], v.back());
	v.pop_back();

	return true;
}

template <typename T>
bool remove(std::vector<T> &v, T object)
{
	auto iter = std::find(v.begin(),v.end(),object);
	if (iter != v.end())
	{
		size_t i = (iter - v.begin());

		std::swap(v[i], v.back());
		v.pop_back();

		return true;
	}

	return false;
}

template<typename T>
typename std::vector<T>::iterator find(std::vector<T>& v, T object)
{
	auto itr = std::find(v.begin(), v.end(), object);
	return itr;
}

template<typename T>
bool contains(std::vector<std::vector<T>>& v, T object)
{
	for (auto v1 : v)
	{
		if (find(v1, object) != v1.end())return true;
	}

	return false;
}

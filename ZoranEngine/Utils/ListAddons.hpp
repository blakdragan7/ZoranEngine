#pragma once
#include <list>

template<typename t>
t ListIndex(std::list<t> & list, unsigned index)
{
	auto iter = list.begin();
	for (unsigned i = 0; i <= index; i++)
	{
		iter++;
	}
	return *iter;
}

template<typename t>
const t ListIndex(const std::list<t> & list, unsigned index)
{
	auto iter = list.begin();
	for (unsigned i = 0; i <= index; i++)
	{
		iter++;
	}
	return *iter;
}
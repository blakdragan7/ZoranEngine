#pragma once
template <typename a,typename b>
struct ZPair
{
	a first;
	b second;

	ZPair(a f, b s) : first(f), second(s){}
	~ZPair() {}
};

template <typename a,typename b,typename c>
struct ZTuple
{
	a first;
	b second;
	c third;

	ZTuple(a f, b s, c t) : first(f), second(s), third(t) {}
	~ZTuple() {}
};
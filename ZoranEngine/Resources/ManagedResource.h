#pragma once
/*
*	Abstraction of resource files that have been loaded from disk.
*/

#include <Core/PlatformTypes.h>

template<typename t>
class ZoranEngine_EXPORT ManagedResource
{
private:
	int* retainCount;
	t* pointer;

private:
	// object Lifecycle
	void Retain();
	void Release();

public:
	ManagedResource();
	ManagedResource(t* pointer);
	template<typename ... Args>
	ManagedResource(Args ... args) : pointer(new t(args ...)), retainCount(new int(1)) {}
	ManagedResource(const ManagedResource& other);
	~ManagedResource();

	void DefaultConstruct();

	void operator=(const ManagedResource& other);
	t &operator*();
	const t &operator*()const;
	t* operator->();
	const t* operator->()const;

	inline bool operator== (const ManagedResource& other)const
	{return pointer == pointer;}

	inline bool IsValid()const { return pointer != 0; }

	inline const int GetRetainCount()const { return *retainCount; }

	static ManagedResource Invalid;
};

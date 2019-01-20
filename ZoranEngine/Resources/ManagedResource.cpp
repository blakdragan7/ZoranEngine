#include "stdafx.h"
#include "ManagedResource.h"

#include <Resources/FontAsset.h>
#include <Resources/ImageAsset.h>
#include <Resources/SoundAsset.h>
#include <Resources/ModelAsset.h>

template<typename t>
ManagedResource<t> ManagedResource<t>::Invalid = ManagedResource<t>();

template<typename t>
void ManagedResource<t>::Retain()
{
	if(pointer != 0)
		++(*retainCount);
}

template<typename t>
void ManagedResource<t>::Release()
{
	if (pointer != 0)
	{
		--(*retainCount);
		if ((*retainCount) <= 0)
		{
			delete retainCount;
			if (pointer)delete pointer;
		}
	}
}

template<typename t>
inline ManagedResource<t>::ManagedResource() : pointer(0), retainCount(new int(0))
{
}

template<typename t>
inline ManagedResource<t>::ManagedResource(t * pointer) : pointer(pointer), retainCount(new int(1))
{
}

template<typename t>
inline ManagedResource<t>::ManagedResource(const ManagedResource & other) : pointer(other.pointer), retainCount(other.retainCount)
{
	Retain();
}

template<typename t>
void ManagedResource<t>::operator=(const ManagedResource & other)
{
	if (pointer)
	{
		Release();
	}

	pointer = other.pointer;
	retainCount = other.retainCount;

	Retain();
}

template<typename t>
t &ManagedResource<t>::operator *()
{
	return *pointer;
}

template<typename t>
t * ManagedResource<t>::operator->()
{
	return pointer;
}

template<typename t>
inline ManagedResource<t>::~ManagedResource()
{
	Release();
}

template<typename t>
void ManagedResource<t>::DefaultConstruct()
{
	if (pointer)
	{
		Log(LogLevel_Error, "Trying to Construct Resource that already exists !");
		return;
	}

	pointer = new t;
	(*retainCount) = 1;
}

template class ManagedResource<FontAsset>;
template class ManagedResource<ImageAsset>;
template class ManagedResource<SoundAsset>;
template class ManagedResource<ModelAsset>;
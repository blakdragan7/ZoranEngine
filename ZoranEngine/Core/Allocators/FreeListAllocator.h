#pragma once
#include <Core\Allocators\AllocatorBase.h>
class FreeListAllocator :
	public AllocatorBase
{
public:
	FreeListAllocator();
	virtual ~FreeListAllocator();
};



FreeListAllocator::FreeListAllocator()
{
}


FreeListAllocator::~FreeListAllocator()
{
}

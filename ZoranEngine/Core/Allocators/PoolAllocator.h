#pragma once
#include <Core\Allocators\AllocatorBase.h>
class PoolAllocator :
	public AllocatorBase
{
public:
	PoolAllocator();
	virtual ~PoolAllocator();
};



PoolAllocator::PoolAllocator()
{
}


PoolAllocator::~PoolAllocator()
{
}

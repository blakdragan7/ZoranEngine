#pragma once
#include <Core\Allocators\AllocatorBase.h>
class StackAllocator :
	public AllocatorBase
{
public:
	StackAllocator();
	virtual ~StackAllocator();
};



StackAllocator::StackAllocator()
{
}


StackAllocator::~StackAllocator()
{
}

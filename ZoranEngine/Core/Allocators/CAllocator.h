#pragma once
#include <Core\Allocators\AllocatorBase.h>
#include <cstdlib>

class CAllocator : public AllocatorBase
{
public:
	CAllocator() : AllocatorBase(0) {}
	virtual ~CAllocator() {}

	virtual void* Allocate(size_t& size, size_t allignment = 0)override
	{
		return malloc(size);
	}

	virtual void DeAllocate(void* ptr)override
	{
		free(ptr);
	}

	virtual void DeAllocate(void* ptr, size_t size)override
	{

	}

	virtual void* ResizeAllocation(void* mem, size_t size, size_t &old_size)override
	{
		if (old_size >= size) return mem;
		old_size = size;
		return realloc(mem,size);
	}

	virtual void Init()
	{

	}
};

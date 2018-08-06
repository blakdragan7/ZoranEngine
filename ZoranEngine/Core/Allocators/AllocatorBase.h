#pragma once

class AllocatorBase
{
protected:
	size_t totalSize;
	size_t used;
	size_t end;

public:
	AllocatorBase(size_t totalSize) 
	{ 
		this->totalSize = totalSize;
		used = 0;
		end = 0;
	}

	virtual ~AllocatorBase() {}

	virtual void* ResizeAllocation(void* mem, size_t size, size_t &old_size) = 0;
	virtual void* Allocate(size_t& size, size_t allignment = 0) = 0;
	virtual void DeAllocate(void* ptr) = 0;
	virtual void DeAllocate(void* ptr,size_t size) = 0;
	virtual void Init() = 0;
};

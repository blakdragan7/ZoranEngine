#pragma once
#include <Core/Allocators/AllocatorBase.h>
#include <Core/Allocators/CAllocator.h>
#include <assert.h>

template<typename t>
class ZArray
{
private:
	unsigned size;
	unsigned maxSize;
	size_t allocationSize;

	t* arrayPointerStart;
	t* arrayPointerEnd;
	AllocatorBase* allocator;

	bool deleteAllocator;

public:
	ZArray(AllocatorBase* allocator = 0)
	{
		if (allocator)
		{
			this->allocator = allocator;
			deleteAllocator = false;
		}
		else
		{
			this->allocator = new CAllocator();
			deleteAllocator = true;
		}
		maxSize = 0;
		allocationSize = 0;
		arrayPointerStart = 0;
		arrayPointerEnd = 0;
	}

	ZArray(unsigned maxSize, AllocatorBase* allocator = 0)
	{
		if (allocator)
		{
			this->allocator = allocator;
			deleteAllocator = false;
		}
		else
		{
			this->allocator = new CAllocator();
			deleteAllocator = true;
		}

		this->maxSize = maxSize;

		allocationSize = sizeof(t) * maxSize;
		arrayPointerStart = (t*)this->allocator->Allocate(allocationSize);
		arrayPointerEnd = arrayPointerStart;
	}

	~ZArray()
	{
		allocator->DeAllocate(arrayPointerStart);
		if (deleteAllocator)
		{
			delete allocator;
		}
	}

	inline unsigned GetSize() { return size; }
	inline unsigned GetMaxSize() { return maxSize; }

	inline t& operator[](unsigned index)
	{
		IndexCheck(index);
		return arrayPointerStart[index];
	}

	inline const t& operator[](unsigned index)const
	{
		IndexCheck(index);
		return arrayPointerStart[index];
	}

	inline void IndexCheck(unsigned index)
	{
		assert(index < size && "Index Greater Then Array size !!");
	}

	inline void InsertAt(t& object,unsigned index)
	{
		size++;
		if (size >= maxSize)
		{
			arrayPointerStart = allocator->ResizeAllocation(arrayPointerStart,size * sizeof(t), allocationSize);
			arrayPointerEnd = arrayPointerStart + size;
			maxSize = allocationSize / sizeof(t);
		}
		// - 2 because we already ++ size
		memmove(&arrayPointerStart[index+1],&arrayPointerStart[index], &arrayPointerStart[size-2] - &arrayPointerStart[index]);
		arrayPointerStart[index] = object;
	}

	inline void Add(t& object)
	{
		size++;
		if (size >= maxSize)
		{
			arrayPointerStart = (t*)allocator->ResizeAllocation(arrayPointerStart, size * sizeof(t), allocationSize);
			arrayPointerEnd = arrayPointerStart + size;
			maxSize = static_cast<unsigned>(allocationSize / sizeof(t));
		}
		arrayPointerStart[size-1] = object;
	}

	inline void RemoveAt(unsigned index, bool canShrink = false)
	{
		typedef t tt;
		(arrayPointerStart + index)->~tt();

		if (index + 1 == size)
		{
			size--;
		}
		else
		{
			memmove(&arrayPointerStart[index], &arrayPointerStart[index + 1], &arrayPointerStart[size - 1] - &arrayPointerStart[index + 1]);
		}

		if (canShrink)
		{
			arrayPointerStart = (t*)allocator->ResizeAllocation(arrayPointerStart, size * sizeof(t), allocationSize);
			arrayPointerEnd = arrayPointerStart + size;
			maxSize = static_cast<unsigned>(allocationSize / sizeof(t));
		}
	}

	inline void Remove(const t& object, bool canShrink = false)
	{
		unsigned index;
		if(Find(object,index))
		{
			RemoveAt(index,canShrink);
		}
	}

	inline t Pop(bool canShrink = false)
	{
		t object = LastObject();
		RemoveAt(LastIndex(),canShrink);
		return object;
	}

	inline unsigned LastIndex()
	{
		return size - 1;
	}

	inline t LastObject()
	{
		return arrayPointerStart[LastIndex()];
	}

	// Currently O(N)
	inline bool Find(const t& object,unsigned &index)
	{
		// ToDo make more efficient
		for (unsigned i = 0; i < size; i++)
		{
			if (arrayPointerStart[i] == object)
			{
				index = i;
				return true;
			}
		}

		return false;
	}

	inline void EmptyAndDelete()
	{
		for (t* ptr = arrayPointerStart; ptr != arrayPointerEnd; ptr++)
		{
			typedef t tt;
			ptr->~tt();
		}

		allocator->DeAllocate(arrayPointerStart);

		allocationSize = sizeof(t) * maxSize;
		arrayPointerStart = (t*)this->allocator->Allocate(allocationSize);
		arrayPointerEnd = arrayPointerStart;
	}

	inline void Empty()
	{
		for (t* ptr = arrayPointerStart; ptr != arrayPointerEnd; ptr++)
		{
			typedef t tt;
			ptr->~tt();
		}
		// zarray assumes you are managing the memory of the objects and it doesnt need to delete them here
		size = 0;
	}

	inline t* begin()
	{
		return arrayPointerStart;
	}

	inline t* end()
	{
		return arrayPointerEnd;
	}
};

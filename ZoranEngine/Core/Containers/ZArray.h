#pragma once
#ifdef CUSTOM_CONTAINERS
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

	ZArray(unsigned maxSize = 8, AllocatorBase* allocator = 0)
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

		size = 0;

		allocationSize = sizeof(t) * maxSize;
		void* mem = (t*)this->allocator->Allocate((size_t&)allocationSize);
		arrayPointerStart = new(mem) t[maxSize];
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

	inline unsigned GetSize()const { return size; }
	inline unsigned GetMaxSize()const { return maxSize; }

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
		assert(index < maxSize && "Index Greater Then Array size !!");
	}

	inline void InsertAt(t& object,unsigned index)
	{
		IndexCheck(index);
		size++;
		if (size >= maxSize)
		{
			ResizeTo(maxSize*2);
		}
		// - 2 because we already ++ size
		memmove(&arrayPointerStart[index+1],&arrayPointerStart[index], &arrayPointerStart[size-2] - &arrayPointerStart[index]);
		arrayPointerStart[index] = object;

		arrayPointerEnd = arrayPointerStart + size;
	}

	inline void Add(const t& object)
	{
		size++;
		if (size >= maxSize)
		{
			ResizeTo(maxSize*2);
		}
		arrayPointerStart[size-1] = object;

		arrayPointerEnd = arrayPointerStart + size;
	}

	inline void RemoveAt(unsigned index, bool canShrink = false)
	{
		IndexCheck(index);
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
			ResizeTo(size);
		}

		arrayPointerEnd = arrayPointerStart + size;
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

	inline unsigned LastIndex()const
	{
		return size - 1;
	}

	inline t LastObject()
	{
		return arrayPointerStart[LastIndex()];
	}

	inline void ResizeTo(size_t newSize)
	{
		assert(newSize > size);

		arrayPointerStart = (t*)allocator->ResizeAllocation(arrayPointerStart, newSize * sizeof(t), allocationSize);
		arrayPointerEnd = arrayPointerStart + size;
		maxSize = static_cast<unsigned>(allocationSize / sizeof(t));

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

	inline void Append(const ZArray<t>& other)
	{
		size_t newsize = size + other.GetSize();
		if (newsize >= maxSize)
		{
			ResizeTo(newsize);
		}

		memcpy(arrayPointerEnd,other.arrayPointerStart,other.GetSize() * sizeof(t));
	}

	inline void AddUnique(const t& object)
	{
		if (Contains(object) == false)
		{
			Add(object);
		}
	}

	// Currently O(N)
	inline bool Contains(const t& object)const
	{
		// ToDo make more efficient
		for (unsigned i = 0; i < size; i++)
		{
			if (arrayPointerStart[i] == object)
			{
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

		size = 0;
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

	// range based for loop functions

	inline t* begin()
	{
		return arrayPointerStart;
	}

	inline t* end()
	{
		return arrayPointerEnd;
	}
};
#endif

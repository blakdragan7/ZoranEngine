#pragma once
#include <Core/Allocators/AllocatorBase.h>
#include <Core/Containers/BasicContainers.h>
#include <Core/Containers/ZArray.h>

#include <algorithm>
// simple unordered hashing dictionary. Uses ZArray as base. 
// Possibly better more efficient way to make this but ftm this works good enough 

template<typename k,typename v>
class ZDictionary
{
private:
	typedef ZArray<k> KeyArray;
	typedef ZArray<v> ValueArray;

	AllocatorBase * allocator;
	bool deleteAlloc;

	KeyArray*	keys;
	ValueArray* hashArray;
	static std::hash<k> theHash;

	unsigned mod;

private:

	void ReHash()
	{
		unsigned old_mod = mod;
		if (mod == 0)mod = 1024;
		else mod = static_cast<unsigned>(mod * 1.5);
		keys->ResizeTo(mod);
		hashArray->ResizeTo(mod);

		if(old_mod != 0)
		{
			ValueArray tempArray(old_mod,this->allocator);
			tempArray.Append(*hashArray);
			for (auto key : *keys)
			{
				size_t currentHash = theHash(key);
				unsigned oldIndex = currentHash % old_mod;
				unsigned newIndex = currentHash % mod;

				(*hashArray)[newIndex] = tempArray[oldIndex];
			}
		}
	}

public:
	
	ZDictionary(unsigned startingSize = 8,AllocatorBase* allocator = 0)
	{
		if (allocator)
		{
			this->allocator = allocator;
			deleteAlloc = false;
		}
		else
		{
			this->allocator = new CAllocator();
			deleteAlloc = true;
		}
		static size_t arraySize = sizeof(KeyArray);
		void* mem = this->allocator->Allocate(arraySize);
		keys = new(mem) KeyArray(startingSize, this->allocator);

		arraySize = sizeof(ValueArray);
		mem = this->allocator->Allocate(arraySize);
		hashArray = new(mem) ValueArray(startingSize,this->allocator);

		mod = startingSize;
	}

	~ZDictionary()
	{
		keys->~ZArray();
		hashArray->~ZArray();

		allocator->DeAllocate(keys);
		allocator->DeAllocate(hashArray);

		if (deleteAlloc)delete allocator;
	}

	void Add(const k& key, const v& value)
	{
		keys->AddUnique(key);
		if (keys->GetSize() >= mod)
		{
			ReHash();
		}
		
		// use std hash for now
		auto hash = theHash(key);
		unsigned index = hash % mod;

		(*hashArray)[index] = value;

	}

	void Remove(const k& key)
	{
		if (keys->Contains(key))
		{
			keys->Remove(key);
		
			
		}

	}

	v& operator[] (const k& key)
	{
		assert(keys->Contains(key));

		auto hash = theHash(key);
		unsigned index = hash % mod;

		return (*hashArray)[index];
	}

	bool Contains(k key)
	{
		return keys->Contains(key);
	}

	// range based for loop functions

	k* begin()
	{
		return keys->begin();
	}

	k* end()
	{
		return keys->end();
	}

	const KeyArray& GetKeys()
	{
		return *keys;
	}
};

template<typename k,typename v>
std::hash<k> ZDictionary<k, v>::theHash = std::hash<k>();
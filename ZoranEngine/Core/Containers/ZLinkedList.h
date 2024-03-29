#pragma once
#ifdef CUSTOM_CONTAINERS
#include <Core/Allocators/AllocatorBase.h>
#include <assert.h>

template<typename t>
struct ZLinkedListNode
{
	const t data;
	ZLinkedListNode* next;

	ZLinkedListNode(const t& data) : data(data), next(0) {}
};

template<typename t>
class ZLinkedList
{
private:
	typedef ZLinkedListNode<t> node;
	
	node* start;
	node* end;

	AllocatorBase* allocator;
	bool deleteAlloc;

	unsigned size;

private:
	void IndexCheck(unsigned index)
	{
		assert(index < size && "Index is bigger then Size !!");
	}

	node* GetNodeAtIndex(unsigned index)
	{
		IndexCheck(index);
		node* current = start;

		for (unsigned i = 0; i < index; i++)
		{
			assert(current->next != 0 && "Next node in [] operator is Null !!");
			current = current->next;
		}

		return current;
	}

public:

	ZLinkedList(AllocatorBase* allocator=0)
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
		start = 0;
		end = 0;
		size = 0;
	}

	ZLinkedList(const t& data,AllocatorBase* allocator=0)
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
		size = 0;
		start = 0;
		end = 0;

		Add(data);
	}

	~ZLinkedList()
	{
		Empty();

		if (deleteAlloc)
			delete allocator;
	}

	inline unsigned GetSize()
	{
		return size;
	}

	inline void Empty()
	{
		while (start)
		{
			node* temp = start;
			start = start->next;

			temp->~node();

			allocator->DeAllocate(temp);
		}

		start = 0;
		end = 0;

		size = 0;
	}

	inline void Add(const t& data)
	{
		static size_t nodeSize = sizeof(node);
		void* mem = allocator->Allocate(nodeSize);
		node* n = new(mem) node(data);

		if (end)
		{
			end->next = n;
			end = end->next;
		}
		else
		{
			start = n;
			end = start;
		}

		size++;
	}

	inline t& RemoveAtRetData(unsigned index)
	{
		IndexCheck(index);

		node* n = GetNodeAtIndex(index - 1);
		node* toDelete = n->next;
		t data = toDelete->data;
		n->next = toDelete->next;
		toDelete->~node();
		allocator->DeAllocate(toDelete);

		size--;

		return data;
	}

	inline void RemoveAt(unsigned index)
	{
		IndexCheck(index);

		node* n = GetNodeAtIndex(index - 1);
		node* toDelete = n->next;
		n->next = toDelete->next;
		toDelete->~node();
		allocator->DeAllocate(toDelete);

		size--;
	}

	inline void InsertAt(unsigned index, const t& data)
	{
		IndexCheck(index);

		node* n = GetNodeAtIndex(index-1);
		void* mem = allocator->Allocate(sizeof(node));
		node* newnode = new(mem) node(t);
		newnode->next = n->next;
		n->next = newnode;
	
		size++;
	}

	inline t PopLast()
	{
		assert(size != 0 && "Trying to PopLast with nothing inserted !!");

		t data = end->data;
		node* tmp = end;
		if (size > 2)
		{
			node* n = GetNodeAtIndex(size - 2);
			end = n;
			end->next = 0;
		}
		else
		{
			// there was only one left in the linked list so we just set everything to 0
			start = end = 0;
		}

		size--;

		tmp->~node();
		allocator->DeAllocate(tmp);

		return data;
	}

	inline t& operator[](unsigned index)
	{
		IndexCheck(index);

		node* current = start;

		for (unsigned i = 0; i <= index; i++)
		{
			assert(current->next != 0 && "Next node in [] operator is Null !!")
			current = current->next;
		}

		return current->data;
	}
};
#endif

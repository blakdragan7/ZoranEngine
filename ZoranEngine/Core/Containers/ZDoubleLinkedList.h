#pragma once
#include <Core/Allocators/AllocatorBase.h>
#include <assert.h>

template<typename t>
struct ZDoubleLinkedListNode
{
	t data;
	ZDoubleLinkedListNode* previous;
	ZDoubleLinkedListNode* next;

	ZDoubleLinkedListNode(const t& data) : data(data), previous(0), next(0) {}
};

template<typename t>
class ZDoubleLinkedList
{
private:
	typedef ZDoubleLinkedListNode<t> node;
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

		node* current;

		if (index > size / 2) // iterate backwords to find the index faster
		{
			current = end;

			for (unsigned i = size - 1; i >= index; i++)
			{
				assert(current->next != 0 && "Next node in [] operator is Null !!");
				current = current->previous;
			}
		}
		else
		{
			current = start;

			for (unsigned i = 0; i < index; i++)
			{
				assert(current->next != 0 && "Next node in [] operator is Null !!");
				current = current->next;
			}
		}
		

		return current;
	}

public:
	ZDoubleLinkedList(AllocatorBase* allocator = 0)
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

	ZDoubleLinkedList(const t& data, AllocatorBase* allocator = 0)
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

	~ZDoubleLinkedList()
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
			n->previous = end;
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
		if (toDelete->next)
			toDelete->next->previous = n;

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
		if(toDelete->next)
			toDelete->next->previous = n;

		toDelete->~node();
		allocator->DeAllocate(toDelete);

		size--;
	}

	inline void InsertAt(unsigned index, const t& data)
	{
		IndexCheck(index);

		node* n = GetNodeAtIndex(index - 1);
		void* mem = allocator->Allocate(sizeof(node));
		node* newnode = new(mem) node(t);
		newnode->next = n->next;
		newnode->previous = n;
		n->next = newnode;
		if (newnode->next)
			newnode->next->previous = newnode;

		size++;
	}

	inline t PopLast()
	{
		assert(size != 0 && "Trying to PopLast with nothing inserted !!");

		node* tmp = end;

		t data = end->data;
		end = end->previous;
		end->next = 0;

		tmp->~node();
		allocator->DeAllocate(tmp);
		
		size--;

		return data;
	}

	inline t& operator[](unsigned index)
	{
		IndexCheck(index);

		node* current = GetNodeAtIndex(index);

		return current->data;
	}
};

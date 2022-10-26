#include "StackAllocator.h"
#include "System.h"

StackAllocator::StackAllocator() :buffer(DBG_NEW char[1024 * 1024 * STACK_BUFFER_SIZE]), base(buffer) {}

StackAllocator::~StackAllocator()
{
	/*if (buffer)
	{
		delete buffer;
	}
	buffer = nullptr;
	if (base)
	{
		delete base;
	}
	base = nullptr;*/
}


template<typename T> T* StackAllocator::alloc()
{
	size_t sizeToAllocate = sizeof(T);
	T* allocationPoint = (T*)base;
	
	if (base + sizeToAllocate > buffer + (1024 * 1024 * STACK_BUFFER_SIZE))
	{
		return nullptr;
	}

	base += sizeToAllocate;
	return allocationPoint;
}


template<typename T> T*  StackAllocator::alloc(size_t size)
{
	size_t sizeToAllocate = sizeof(T * size);
	T* allocationPoint = (T*)base;

	if (base + sizeToAllocate > buffer + (1024 * 1024 * STACK_BUFFER_SIZE))
	{
		return nullptr;
	}

	base += sizeToAllocate;
	return allocationPoint;
}
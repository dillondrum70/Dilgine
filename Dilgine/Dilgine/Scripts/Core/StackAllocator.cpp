#include "StackAllocator.h"
#include "System.h"

StackAllocator::StackAllocator() :buffer(DBG_NEW char[1024 * 1024 * STACK_BUFFER_SIZE]), base(buffer) {}

StackAllocator::~StackAllocator()
{
	if (buffer)
	{
		delete buffer;
	}
	buffer = nullptr;
	/*if (base)
	{
		delete base;
	}
	base = nullptr;*/
}
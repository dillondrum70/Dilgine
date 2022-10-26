#ifndef STACK_ALLOCATOR_H
#define STACK_ALLOCATOR_H
#include <iostream>

const int STACK_BUFFER_SIZE = 32; //In MB

class StackAllocator {
private:
	char* buffer;
	char* base;

public:
	StackAllocator();
	~StackAllocator();

	template<typename T>
	T* alloc()
	{
		size_t sizeToAllocate = sizeof(T);
		T* allocationPoint = (T*)base;

		if (base + sizeToAllocate > buffer + (1024 * 1024 * STACK_BUFFER_SIZE))
		{
			return nullptr;
		}

		//Print what's on stack
		{
			std::cout << "Stack: " << &allocationPoint << std::endl << std::endl;
		}

		base += sizeToAllocate;
		return allocationPoint;
	}

	template<typename T>
	T* alloc(size_t size)
	{
		size_t sizeToAllocate = sizeof(T) * size;
		T* allocationPoint = (T*)base;

		if (base + sizeToAllocate > buffer + (1024 * 1024 * STACK_BUFFER_SIZE))
		{
			return nullptr;
		}

		//Print what's on stack
		{
			for (int i = 0; i < size; i++)
			{
				std::cout << "Stack: " << &allocationPoint[i] << std::endl;
			}
			std::cout << std::endl;
		}
		
		base += sizeToAllocate;
		return allocationPoint;
	}

	void clear() { base = buffer; }
};

#endif

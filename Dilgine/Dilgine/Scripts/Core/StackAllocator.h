#ifndef STACK_ALLOCATOR_H
#define STACK_ALLOCATOR_H



const int STACK_BUFFER_SIZE = 32; //In MB

class StackAllocator {
private:
	char* buffer;
	char* base;

public:
	StackAllocator();
	~StackAllocator();

	template<typename T>
	T* alloc();

	template<typename T>
	T* alloc(size_t size);

	void clear() { base = buffer; }
};

#endif

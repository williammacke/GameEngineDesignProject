#ifndef STACKARRAY_H_
#define STACKARRAY_H_
#include <vector>
#include <string>
#include <glad/glad.h>
#include "linAlg.h"
#include <Components.h>
#include <cassert>

template <class T>
struct StackArray {
public:
	StackArray(size_t cap);
	StackArray(std::initializer_list<T> list);
	~StackArray();
	T& operator[](int index);
	T* begin();
	T* end();
	size_t size();
private:
	size_t length;
	T *data;
};

template <class T>
StackArray<T>::StackArray(size_t length) :  length(length) {
	auto stack  = &(MemoryManager::get().getStack());
	data = (T*) stack->allocSpace(length*sizeof(T));
}

template <class T>
StackArray<T>::StackArray(std::initializer_list<T> list) :  length(list.size()) {
	auto stack = &(MemoryManager::get().getStack());
	data = (T*) stack->allocSpace(length*sizeof(T));
	std::copy(list.begin(), list.end(), data);
}

template <class T>
StackArray<T>::~StackArray() {
}

template <class T>
T& StackArray<T>::operator[](int index) {
	assert(index >= 0 && index < length);
	return data[index];
}

template <class T>
size_t StackArray<T>::size() {
	return length;
}

template <class T>
T* StackArray<T>::begin() {
	return data;
}

template <class T>
T* StackArray<T>::end() {
	return data+length;
}

#endif

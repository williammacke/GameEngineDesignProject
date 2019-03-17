#ifndef MEMMAN_H_
#define MEMMAN_H_

#ifndef TEMP_SIZE
#define TEMP_SIZE 128000000
#endif

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <stack>
#include <vector>
#include <cstdarg>
#include <iostream>
#include <chrono>
#include <type_traits>

extern uint8_t temp_buffer[];
extern size_t temp_level;

extern char* tprintf(char *fmt, ...);
extern char* vtprintf(char *fmt, va_list args);
extern void resetLevel();


template <class T>
class PoolAlloc {
public:
	PoolAlloc();
	~PoolAlloc();
	template <typename...Args>
	T* alloc(Args&&...args);
	void remove(T*& item);
	void remove(T*&& item);
	void freeAll();
private:
	size_t cap;
	size_t lastCap;
	T *data;
	T* avail;
	std::vector<T*> starts;
	size_t current;
};



template <class T>
PoolAlloc<T>::PoolAlloc() {
	cap = 4096*2;
	data = (T*) malloc(cap*sizeof(T));
	starts.push_back(data);
	lastCap = cap;
	current = 0;
}

template <class T>
PoolAlloc<T>::~PoolAlloc() {
}

template <class T>
template <typename...Args>
T* PoolAlloc<T>::alloc(Args&&...args) {
	static_assert(sizeof(T) >= sizeof(void*));
	/*
	if (data == nullptr) {
		printf("test\n");
		data = (T*) malloc(cap*sizeof(T));
		assert (data != nullptr);
		if (data == nullptr) {
			printf("We're done\n");
		}
		starts.push_back(data);
		lastCap = cap;
		current = 0;
	}
	*/
	if (avail == nullptr && current >= lastCap-1) {
		printf("test\n");
		data = (T*) malloc(cap*sizeof(T));
		assert( data != nullptr);
		starts.push_back(data);
		/*
		for (int i = 0; i < cap; i++) {
			avail.push(data+i);
		}
		*/
		lastCap = cap;
		cap *= 2;
		current = 0;
	}
	T* val;
	if (avail != nullptr) {
		val = avail;
		T* next = *((T**) avail);
		avail = next;
	}
	else {
		val = &data[current];
		current++;
	}
	new (val) T(args...);
	return val;
}

template <class T>
void PoolAlloc<T>::remove(T*& item) {
	static_assert(sizeof(T) >= sizeof(void*));
	assert(item != nullptr);
	item->~T();
	T** temp = (T**) item;
	*temp = avail;
	avail = item;
	item = nullptr;
}

template <class T>
void PoolAlloc<T>::remove(T*&& item) {
	static_assert(sizeof(T) >= sizeof(void*));
	item->~T();
	T** temp = (T**) item;
	*temp = avail;
	avail = item;
}

template <class T>
void PoolAlloc<T>::freeAll() {
	static_assert(sizeof(T) >= sizeof(void*));
	for (auto& s:starts) {
		free(s);
	}
	starts = std::vector<T*>();
	avail = nullptr;
	data = nullptr;
	cap = 4096*2;
}

template <class T>
struct tempAlloc {
	template <typename...Args>
	static T* alloc(Args&&...args);
	template <typename...Args>
	static T* allocNum(size_t num, Args&&...args);
};

template <class T>
template <typename...Args>
T* tempAlloc<T>::alloc(Args&&...args) {
	T* item = (T*) &temp_buffer[temp_level];
	new (item) T(args...);
	temp_level += sizeof(T);
	return item;
}

template <class T>
template <typename...Args>
T* tempAlloc<T>::allocNum(size_t num,Args&&...args) {
	T* start = (T*) &temp_buffer[temp_level];
	for (int i = 0; i < num; i++) {
		new (start+i) T(args...);
	}
	temp_level += num*sizeof(T);
	return start;
}
class StackAlloc {
public:
	StackAlloc(size_t cap);
	~StackAlloc();
	template <typename T, typename...Args>
	T* alloc(Args&&...args);
	template <typename T, typename...Args>
	T* allocNum(size_t num, Args&&...args);
	void freeTo(void* nsp);
private:
	void *start, *sp;
	size_t cap;
};

template<typename T, typename...Args>
T* StackAlloc::alloc(Args&&...args) {
	T *item = (T*) sp;
	sp += sizeof(T);
	new (item) T(args...);
	return item;
}

template <typename T, typename...Args>
T* StackAlloc::allocNum(size_t num, Args&&...args) {
	T *items = (T*) sp;
	sp = sp+num*sizeof(T);
	for (int i = 0; i < num; i++) {
		new (items+i) T(args...);
	}
	return items;
}

#endif

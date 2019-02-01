#ifndef MEMMAN_H_
#define MEMMAN_H_

#ifndef TEMP_SIZE
#define TEMP_SIZE 128000000
#endif

#include <cstdlib>
#include <cstdint>
#include <stack>
#include <vector>
#include <cstdarg>

extern uint8_t temp_buffer[];
extern size_t temp_level;

extern char* tprintf(char *fmt, ...);
extern char* vtprintf(char *fmt, va_list args);
extern void resetLevel();


template <class T>
struct poolAlloc {
	template <typename...Args>
	static T* alloc(Args&&...args);
	static void remove(T*& item);
	static void remove(T*&& item);
	static void freeAll();
private:
	static size_t cap;
	static T *data;
	static std::stack<T*> avail;
	static std::vector<T*> starts;
};

template <class T>
size_t poolAlloc<T>::cap = 32;

template <class T>
T* poolAlloc<T>::data = nullptr;

template<class T>
std::stack<T*> poolAlloc<T>::avail = std::stack<T*>();

template <class T>
std::vector<T*> poolAlloc<T>::starts = std::vector<T*>();

template <class T>
template <typename...Args>
T* poolAlloc<T>::alloc(Args&&...args) {
	if (data == nullptr) {
		data = (T*) malloc(cap*sizeof(T));
		starts.push_back(data);
		for (int i = 0; i < cap; i++) {
			avail.push(data+i);
		}
	}
	if (avail.empty()) {
		data = (T*) malloc(cap*sizeof(T));
		starts.push_back(data);
		for (int i = 0; i < cap; i++) {
			avail.push(data+i);
		}
		cap *= 2;
	}
	T* val = avail.top();
	avail.pop();
	new (val) T(args...);
	return val;
}

template <class T>
void poolAlloc<T>::remove(T*& item) {
	item->~T();
	avail.push(item);
	item = nullptr;
}

template <class T>
void poolAlloc<T>::remove(T*&& item) {
	item->~T();
	avail.push(item);
}

template <class T>
void poolAlloc<T>::freeAll() {
	for (auto& s:starts) {
		free(s);
	}
	starts = std::vector<T*>();
	avail = std::stack<T*>();
	data = nullptr;
	cap = 32;
}

template <class T>
struct tempAlloc {
	template <typename...Args>
	static T* alloc(Args&&...args);
	template <typename...Args>
	static T* alloc(size_t num, Args&&...args);
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
T* tempAlloc<T>::alloc(size_t num,Args&&...args) {
	T* start = (T*) &temp_buffer[temp_level];
	for (int i = 0; i < num; i++) {
		new (start+i) T(args...);
	}
	temp_level += num*sizeof(T);
	return start;
}
#endif

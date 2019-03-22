#include "memMan.h"
#include <cstdio>
#include <cstdarg>


tempAlloc::tempAlloc() : cap(TEMP_SIZE), temp_level(0) {
	data = malloc(cap);
}

tempAlloc::~tempAlloc() {
	free(data);
}


char* tempAlloc::tprintf(char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	size_t size = vsprintf((char*) data+temp_level, fmt, args);
	char *s = (char*) data+temp_level;
	temp_level += size;
	return s;
}

char* tempAlloc::vtprintf(char *fmt, va_list args) {
	size_t size = vsprintf((char*) data+temp_level, fmt, args);
	char *s = (char*) data+temp_level;
	temp_level += size;
	return s;
}

void tempAlloc::resetLevel() {
	temp_level = 0;
}


StackAlloc::StackAlloc(size_t cap) {
	start = malloc(cap);
	if (start == nullptr) {
		exit(1);
	}
	sp = start;
}

StackAlloc::~StackAlloc() {
	if (start != nullptr)
		free(start);
}

void StackAlloc::freeTo(void* nsp) {
	if (nsp > sp) 
		return;
	sp = nsp;
}

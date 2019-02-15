#include "memMan.h"
#include <cstdio>
#include <cstdarg>


uint8_t temp_buffer[TEMP_SIZE];
size_t temp_level = 0;

char* tprintf(char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	size_t size = vsprintf((char*) &temp_buffer[temp_level], fmt, args);
	char *s = (char*) &temp_buffer[temp_level];
	temp_level += size;
	return s;
}

char *vtprintf(char *fmt, va_list args) {
	size_t size = vsprintf((char*) &temp_buffer[temp_level], fmt, args);
	char *s = (char*) &temp_buffer[temp_level];
	temp_level += size;
	return s;
}

void resetLevel() {
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

#pragma once
#include <stddef.h>

int strcmp(const char* str1, const char* str2);
int strncmp(const char* str1, const char* str2, size_t n);
size_t strlen(const char* str);
void* memcpy(void* dest, const void* src, size_t n);
char* strchr(const char* str, int c);
void* memset(void* ptr, int value, size_t num);


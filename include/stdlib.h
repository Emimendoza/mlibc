#pragma once
#include "internals/memory.h"

[[noreturn]] void exit(int exit_code);
int atexit(void (*function)(void));
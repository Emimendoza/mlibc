#pragma once

[[noreturn]] void exit(int exit_code);
int atexit(void (*function)(void));
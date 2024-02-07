#pragma once
#include <stddef.h>

typedef ptrdiff_t ssize_t;

typedef unsigned long int uid_t;
typedef unsigned long int gid_t;
typedef unsigned long int pid_t;

// File pd constant
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

long int syscall(long int syscall, ...);
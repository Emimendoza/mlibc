#pragma once
#include <stddef.h>

// Defining ssize_t
#ifdef _MLIBC_64BIT
typedef long long ssize_t;
#elifdef __32BIT__
typedef long int ssize_t;
#endif

typedef unsigned long int uid_t;
typedef unsigned long int gid_t;
typedef unsigned long int pid_t;

// File pd constant
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

long int syscall(long int syscall, ...);
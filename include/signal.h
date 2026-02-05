#pragma once
#include <asm/signal.h>
#include "_mlibc_internal/cpp_start.h"

int raise(int sig);
int tgkill(int tgid, int tid, int sig);


#include "_mlibc_internal/cpp_end.h"
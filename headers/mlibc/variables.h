#pragma once
#include <mlibc/macros.h>
#include <stdint.h>

#include "sys/types.h"
namespace mlibc {
inline uint32_t page_size;
CEXPORT char** environ;

extern const bool& dynamically_linked;
extern const thread_local pid_t pid;
extern const thread_local pid_t tid;
}

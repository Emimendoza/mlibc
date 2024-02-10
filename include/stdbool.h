#pragma once
#include "internals/bits/uint.h"

// If we are in c++ mode, we can use the built-in bool type
#if !defined(__cplusplus) && defined(__bool_true_false_are_defined) && __bool_true_false_are_defined == 0
typedef _Bool bool;
#define true 1
#define false 0
#endif

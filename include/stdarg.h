#pragma once

#define va_start(args, len) __builtin_va_start(args, len)
#define va_arg(args, type) __builtin_va_arg(args, type)
#define va_end(args) __builtin_va_end(args)
#define va_list __builtin_va_list
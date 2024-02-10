#pragma once

[[noreturn]] void __stack_chk_fail();
[[noreturn]] void __failure(const char* msg);
void _mlibc_dealloc_all();
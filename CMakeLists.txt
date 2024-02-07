cmake_minimum_required(VERSION 3.27)
project(mephlibc C ASM_NASM)
project(mephlibcs C ASM_NASM)

set(CMAKE_C_STANDARD 23)
set(CMAKE_NO_SYSTEM_FROM_IMPORTED ON)

# Since this is an implementation of the C standard library,
# we need to compile it with the -ffreestanding flag and the -nostdlib flag

set(CMAKE_C_FLAGS "-ffreestanding -nostdlib -nostdinc -nostdinc++")
include_directories("./include")
# we also need to set the object format to elf64 for the assembly files
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)

file(GLOB mephilibc_files
        "src/*.c"
        "src/*.h"
        "src/*.asm"
)
# The api include are in the include directory

add_library(mephlibcs STATIC ${mephilibc_files})
add_library(mephlibc SHARED ${mephilibc_files})

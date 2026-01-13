cmake_minimum_required(VERSION 4.0)

project(mlibc CXX)

include(configure_options.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nostdlib -ffreestanding -nostdinc -fno-builtin -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wall -Wextra -fpic")
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
	enable_language(ASM_NASM)
endif()

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/include")
file(CREATE_LINK "${KERNEL_HEADERS_PATH}/asm" "${CMAKE_BINARY_DIR}/include/asm" SYMBOLIC)
file(CREATE_LINK "${KERNEL_HEADERS_PATH}/asm-generic" "${CMAKE_BINARY_DIR}/include/asm-generic" SYMBOLIC)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.c)
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
	enable_language(ASM_NASM)
	file(GLOB_RECURSE ASM_SOURCES src/*.x86_64.asm)
endif()

add_library(mlibc_objects OBJECT ${SOURCES} ${ASM_SOURCES})
add_library(mlibc SHARED)
add_library(mlibc_static STATIC)
set_target_properties(mlibc_static PROPERTIES OUTPUT_NAME "mlibc")


target_include_directories(mlibc_objects PRIVATE "${CMAKE_BINARY_DIR}/include")
target_include_directories(mlibc_objects PUBLIC include)
target_link_libraries(mlibc PRIVATE mlibc_objects)
target_link_libraries(mlibc_static PRIVATE mlibc_objects)


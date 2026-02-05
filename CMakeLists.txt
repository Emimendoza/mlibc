cmake_minimum_required(VERSION 4.0)

project(mlibc CXX C)

include(configure_options.cmake)

set(BUILDING_FLAGS
	-nostdlib
	-nostdinc
	-Wall
	-Wextra
	-fpic
	-fno-exceptions
	-fno-rtti
)
set(CMAKE_CXX_STANDARD 26)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
	enable_language(ASM_NASM)
	set(INTERPRETER_PATH "/lib64/ld-linux-x86-64.so.2")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	list(APPEND BUILDING_FLAGS
		-target ${CMAKE_SYSTEM_PROCESSOR}-unknown-linux-musl
	)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	# Disable prio based warnings since we are the libc
	list(APPEND BUILDING_FLAGS
		-Wno-prio-ctor-dtor
	)
endif()


file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/include")
# TODO: add a proper way to install generated files properly
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/include/_mlibc_internal")
set(NEEDED_LINKS
	asm
	asm-generic
	linux
)
foreach (header ${NEEDED_LINKS})
	file(CREATE_LINK "${KERNEL_HEADERS_PATH}/${header}" "${CMAKE_BINARY_DIR}/include/${header}" SYMBOLIC)
endforeach()

execute_process(
	COMMAND ${CMAKE_COMMAND}
	-D "HEADER_DIR=${KERNEL_HEADERS_PATH}/asm"
	-P ${CMAKE_SOURCE_DIR}/cmake/generate_syscalls.cmake
	OUTPUT_VARIABLE SYSCALL_DEFINITIONS
	COMMAND_ERROR_IS_FATAL ANY
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

configure_file(
	include/_mlibc_internal/syscall_definitions.h.in
	"${CMAKE_BINARY_DIR}/include/_mlibc_internal/syscall_definitions.h"
	@ONLY
)
unset(SYSCALL_DEFINITIONS)

file(GLOB_RECURSE SOURCES src/mlibc/*.cpp src/mlibc/*.c)
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
	enable_language(ASM_NASM)
	file(GLOB_RECURSE ASM_SOURCES src/mlibc/*.x86_64.asm)
	file(GLOB_RECURSE ENTRY_SOURCES src/entry/*.x86_64.asm)
endif()

add_library(mlibc_objects OBJECT ${SOURCES} ${ASM_SOURCES})
add_library(mlibc_entry OBJECT ${ENTRY_SOURCES})
add_library(mlibc SHARED)
add_library(mlibc_static STATIC)
set_target_properties(mlibc_static PROPERTIES OUTPUT_NAME "mlibc")

target_compile_definitions(mlibc_objects PRIVATE __MLIBC_BUILDING_MLIBC)
target_compile_options(mlibc_objects PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${BUILDING_FLAGS}>)
target_link_options(mlibc PRIVATE ${BUILDING_FLAGS})
target_link_options(mlibc_static PRIVATE ${BUILDING_FLAGS})
target_include_directories(mlibc_objects PRIVATE "${CMAKE_BINARY_DIR}/include")
target_include_directories(mlibc_entry PRIVATE "${CMAKE_BINARY_DIR}/include")
target_include_directories(mlibc_objects PRIVATE headers)
target_include_directories(mlibc_entry PRIVATE headers)
target_include_directories(mlibc_objects PUBLIC include include/c++)
target_link_libraries(mlibc PUBLIC mlibc_objects)
configure_file(src/mlibc/libc.so.in "${CMAKE_BINARY_DIR}/libc.so")

target_link_libraries(mlibc_static PUBLIC mlibc_objects)

set_target_properties(mlibc_objects PROPERTIES
	C_VISIBILITY_PRESET hidden
	CXX_VISIBILITY_PRESET hidden
	VISIBILITY_INLINES_HIDDEN YES
)

if (TESTS_ENABLED)
	enable_testing()
	add_subdirectory(test)
endif()
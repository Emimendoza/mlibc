cmake_minimum_required(VERSION 4.0)

project(mlibc CXX)

include(configure_options.cmake)

set(BUILDING_FLAGS
	-nostdlib
	-nostdinc
	-Wall
	-Wextra
	-fpic
)
set(CMAKE_CXX_STANDARD 26)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
	enable_language(ASM_NASM)
	set(INTERPRETER_PATH "/lib64/ld-linux-x86-64.so.2")
endif()


file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/include")
set(NEEDED_LINKS
	asm
	asm-generic
	linux
)
foreach (header ${NEEDED_LINKS})
	file(CREATE_LINK "${KERNEL_HEADERS_PATH}/${header}" "${CMAKE_BINARY_DIR}/include/${header}" SYMBOLIC)
endforeach()


file(GLOB_RECURSE SOURCES src/mlibc/*.cpp)
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
target_include_directories(mlibc_objects PUBLIC include)
target_link_libraries(mlibc PUBLIC mlibc_objects)
configure_file(src/mlibc/libc.so.in "${CMAKE_BINARY_DIR}/libc.so")

target_link_libraries(mlibc_static PUBLIC mlibc_objects)

set_target_properties(mlibc_objects PROPERTIES
	C_VISIBILITY_PRESET hidden
	CXX_VISIBILITY_PRESET hidden
	VISIBILITY_INLINES_HIDDEN YES
)

# Disable warnings related to using things reserved for the implementation
# since we ARE the implementation.
target_compile_options(mlibc_objects PRIVATE
	-Wno-prio-ctor-dtor
)

if (TESTS_ENABLED)
	enable_testing()
	add_subdirectory(test)
endif()
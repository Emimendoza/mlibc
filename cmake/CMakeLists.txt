cmake_minimum_required(VERSION 3.27)
project(mlibc_compiler_checks C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_NO_SYSTEM_FROM_IMPORTED ON)
set(CMAKE_C_FLAGS "-ffreestanding -nostdlib -nostdinc")

set(MLIBC_DEFS "")
# Any compiler checks go here



set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MLIBC_DEFS}" CACHE STRING "" FORCE)
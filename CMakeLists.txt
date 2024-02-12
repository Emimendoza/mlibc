cmake_minimum_required(VERSION 3.27)
project(mephlibc C ASM_NASM)

set(CMAKE_C_STANDARD 23)
set(CMAKE_NO_SYSTEM_FROM_IMPORTED ON)

# Since this is an implementation of the C standard library,
# we need to compile it with the -ffreestanding flag and the -nostdlib flag

include_directories("./include")
# we also need to set the object format to elf64 for the assembly files
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)

# The cmake subdir will include setting compiler definitions based on the host compiler
add_subdirectory(./cmake)

file(GLOB mephilibc_files
        "src/*.c"
        "src/**/*.c"
        "src/*.asm"
        "src/**/*.asm"
)
# we also need to set the public symbols file
set(PUBLIC_SYMBOLS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/public_symbols.txt")
file(READ ${PUBLIC_SYMBOLS_FILE} PUBLIC_SYMBOLS_STR)
string(REPLACE "\n" ";" PUBLIC_SYMBOLS_STR ${PUBLIC_SYMBOLS_STR})

add_library(mc_s STATIC ${mephilibc_files})
add_library(mc SHARED ${mephilibc_files})
add_library(crt1 OBJECT "special/start.asm")

set_target_properties(mc PROPERTIES PUBLIC_SYMBOLS "${PUBLIC_SYMBOLS_STR}")
set_target_properties(mc_s PROPERTIES PUBLIC_SYMBOLS "${PUBLIC_SYMBOLS_STR}")
set_target_properties(crt1 PROPERTIES PUBLIC_SYMBOLS "${PUBLIC_SYMBOLS_STR}")

file(GLOB test_files
        "tests/*.c"
)
enable_testing()
foreach (test ${test_files})
    get_filename_component(test_name ${test} NAME_WE)
    set(test_name "test_${test_name}")
    add_executable(${test_name} ${test})
    add_executable(${test_name}_static ${test})
    target_link_libraries(${test_name} PRIVATE mc crt1)
    target_link_libraries(${test_name}_static PRIVATE mc_s crt1)
    add_test(NAME ${test_name} COMMAND ${test_name})
    add_test(NAME ${test_name}_static COMMAND ${test_name}_static)
endforeach (test)

# Tests which should fail
file(GLOB test_fail_files
        "tests/fail/*.c"
)
foreach (test ${test_fail_files})
    get_filename_component(test_name ${test} NAME_WE)
    set(test_name "test_fail_${test_name}")
    add_executable(${test_name} ${test})
    add_executable(${test_name}_static ${test})
    target_link_libraries(${test_name} PRIVATE mc crt1)
    target_link_libraries(${test_name}_static PRIVATE mc_s crt1)
    add_test(NAME ${test_name} COMMAND ${test_name})
    add_test(NAME ${test_name}_static COMMAND ${test_name}_static)
    set_tests_properties(${test_name} PROPERTIES WILL_FAIL TRUE)
    set_tests_properties(${test_name}_static PROPERTIES WILL_FAIL TRUE)
endforeach (test)
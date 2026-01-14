file(GLOB_RECURSE TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(mlibc_tests ${TEST_SOURCES})
target_include_directories(mlibc_tests PRIVATE include)
target_link_libraries(mlibc_tests PRIVATE mlibc_static mlibc_entry)
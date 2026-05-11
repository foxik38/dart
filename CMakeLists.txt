cmake_minimum_required(VERSION 4.3.2)
project(dart LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)

set(dart_header_files
        include/file.hpp
)

set(dart_source_files
        src/file.cpp
)

add_library(dart_target ${dart_header_files} ${dart_source_files})
target_include_directories(dart_target PUBLIC include)

add_executable(dart_executable src/main.cpp)
target_link_libraries(dart_executable PRIVATE dart_target)

add_executable(dart_unit_tests
        tests/file_test.cpp
)

target_link_libraries(dart_unit_tests PRIVATE dart_target gtest_main)
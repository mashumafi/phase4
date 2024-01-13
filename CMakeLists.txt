cmake_minimum_required(VERSION 3.28)

project(phase4 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

add_library(phase4 INTERFACE)
target_include_directories(phase4 INTERFACE include)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)

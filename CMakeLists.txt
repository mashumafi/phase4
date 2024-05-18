cmake_minimum_required(VERSION 3.27.9)

project(phase4 VERSION 1.0 LANGUAGES CXX)

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

add_library(phase4 INTERFACE)
target_include_directories(phase4 INTERFACE include)
target_compile_features(phase4 INTERFACE cxx_std_17)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)

add_subdirectory(benchmarks)

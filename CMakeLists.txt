cmake_minimum_required(VERSION 3.27.9)

project(phase4 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

add_library(phase4 INTERFACE)
target_include_directories(phase4 INTERFACE include)

add_library(phase4_fast STATIC
    include/phase4/engine/board/ordering/static_exchange_evaluation.cpp
)
target_compile_definitions(phase4_fast PRIVATE FAST_BUILD)
target_link_libraries(phase4_fast PUBLIC phase4)
target_compile_options(phase4_fast PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-fconstexpr-ops-limit=335544320>
    $<$<CXX_COMPILER_ID:Clang>:-fconstexpr-steps=335544320>
    $<$<CXX_COMPILER_ID:AppleClang>:-fconstexpr-steps=335544320>
)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)

add_subdirectory(benchmarks)

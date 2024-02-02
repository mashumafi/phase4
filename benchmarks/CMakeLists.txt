include(FetchContent)

set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON)
FetchContent_Declare(
    google-benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG        v1.8.3
)

FetchContent_MakeAvailable(google-benchmark)

add_executable(valid_move_check valid_move_check.b.cpp)
target_link_libraries(valid_move_check PRIVATE benchmark::benchmark phase4_fast)

add_executable(default_position default_position.b.cpp)
target_link_libraries(default_position PRIVATE benchmark::benchmark phase4_fast)

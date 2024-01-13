add_subdirectory(doctest)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${doctest_SOURCE_DIR}/scripts/cmake)
message(${doctest_SOURCE_DIR}/scripts/cmake)

add_executable(phase4_test
    phase4/common/bitset.t.cpp
    phase4/common/castling.t.cpp
    phase4/common/distance.t.cpp
    phase4/common/piece_color.t.cpp
    phase4/common/square.t.cpp
)
target_link_libraries(phase4_test PRIVATE phase4 doctest_main)

include(doctest)
doctest_discover_tests(phase4_test)

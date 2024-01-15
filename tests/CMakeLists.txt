add_subdirectory(doctest)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${doctest_SOURCE_DIR}/scripts/cmake)
message(${doctest_SOURCE_DIR}/scripts/cmake)

add_executable(phase4_test
    phase4/engine/board/position.t.cpp
    phase4/engine/board/session.t.cpp
    phase4/engine/board/zobrist_hashing.t.cpp
    phase4/engine/common/bitset.t.cpp
    phase4/engine/common/castling.t.cpp
    phase4/engine/common/distance.t.cpp
    phase4/engine/common/fast_vector.t.cpp
    phase4/engine/common/math.t.cpp
    phase4/engine/common/piece_color.t.cpp
    phase4/engine/common/piece_type.t.cpp
    phase4/engine/common/random.t.cpp
    phase4/engine/common/safe_vector.t.cpp
    phase4/engine/common/square.t.cpp
    phase4/engine/common/util.t.cpp
    phase4/engine/moves/move.t.cpp
    phase4/engine/moves/move_flags.t.cpp
)
target_link_libraries(phase4_test PRIVATE phase4 doctest_main)

include(doctest)
doctest_discover_tests(phase4_test)

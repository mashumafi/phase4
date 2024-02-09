add_subdirectory(doctest)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${doctest_SOURCE_DIR}/scripts/cmake)
message(${doctest_SOURCE_DIR}/scripts/cmake)

add_executable(phase4_test
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
    phase4/engine/common/wall_operations.t.cpp

    phase4/engine/moves/magic/attacks_generator.t.cpp
    phase4/engine/moves/magic/magic_bitboards.t.cpp
    phase4/engine/moves/magic/permutations_generator.t.cpp
    phase4/engine/moves/move_flags.t.cpp
    phase4/engine/moves/move.t.cpp
    phase4/engine/moves/moves_generator.t.cpp
    phase4/engine/moves/patterns/box_pattern_generator.t.cpp
    phase4/engine/moves/patterns/chain_pattern_generator.t.cpp
    phase4/engine/moves/patterns/diagonal_pattern_generator.t.cpp
    phase4/engine/moves/patterns/file_pattern_generator.t.cpp
    phase4/engine/moves/patterns/forward_box_pattern_generator.t.cpp
    phase4/engine/moves/patterns/outer_files_pattern_generator.t.cpp
    phase4/engine/moves/patterns/passing_pattern_generator.t.cpp
    phase4/engine/moves/patterns/rank_pattern_generator.t.cpp

    phase4/engine/board/operators.t.cpp
    phase4/engine/board/ordering/move_ordering.t.cpp
    phase4/engine/board/piece_square_tables/table_operations.t.cpp
    phase4/engine/board/position_state.t.cpp
    phase4/engine/board/position.t.cpp
    phase4/engine/board/session.t.cpp
    phase4/engine/board/transposition/transposition_table.t.cpp
    phase4/engine/board/zobrist_hashing.t.cpp

    phase4/engine/ai/score/evaluation.t.cpp
    phase4/engine/ai/score/evaluators/bishop_evaluator.t.cpp
    phase4/engine/ai/score/evaluators/king_safety_evaluator.t.cpp
    phase4/engine/ai/score/evaluators/material_evaluator.t.cpp
    phase4/engine/ai/score/evaluators/mobility_evaluator.t.cpp
    phase4/engine/ai/score/evaluators/pawn_structure_evaluator.t.cpp
    phase4/engine/ai/score/evaluators/position_evaluator.t.cpp
    phase4/engine/ai/score/evaluators/rook_evaluator.t.cpp
    phase4/engine/ai/search/iterative_deepening.t.cpp
    phase4/engine/ai/search/nega_max.t.cpp
    phase4/engine/ai/search/quiescence_search.t.cpp
    phase4/engine/ai/time/time_scheduler.t.cpp

    phase4/engine/fen/fen_to_position.t.cpp
    phase4/engine/fen/fen_validator.t.cpp
    phase4/engine/fen/position_to_fen.t.cpp

    phase4/main.t.cpp
)
target_link_libraries(phase4_test PRIVATE phase4_fast doctest)
target_compile_options(phase4_test PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-fconstexpr-ops-limit=335544320>
    $<$<CXX_COMPILER_ID:Clang>:-fconstexpr-steps=335544320>
    $<$<CXX_COMPILER_ID:AppleClang>:-fconstexpr-steps=335544320>
)

include(doctest)
doctest_discover_tests(phase4_test)

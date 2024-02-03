add_executable(generate_magics.tsk generate_magics.m.cpp)
target_link_libraries(generate_magics.tsk PRIVATE phase4_fast)

add_executable(solve_puzzles.tsk solve_puzzles.m.cpp)
target_link_libraries(solve_puzzles.tsk PRIVATE phase4_fast)

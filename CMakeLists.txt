cmake_minimum_required(VERSION 3.0.0)
project(sudoku-solver VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(sudoku-engine sudoku_engine.c sudoku_common.c)

#main code
add_executable(sudoku-solver main.cpp) 

target_link_libraries(sudoku-solver PUBLIC sudoku-engine)
#add_executable(cpp_test, src/test.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

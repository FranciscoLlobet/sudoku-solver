cmake_minimum_required(VERSION 3.0.0)
project(sudoku-solver VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(sudoku-engine sudoku_engine.c sudoku_common.c)

#main code
add_executable(sudoku-solver main.cpp) 

#Array testing
add_executable(sudoku-data-test test-sudoku-data.cc)

# Test Executable
add_executable(sudoku-engine-test test-sudoku-engine.cc sudoku_common.c)

target_link_libraries(sudoku-solver PUBLIC sudoku-engine)
target_link_libraries(sudoku-data-test PUBLIC sudoku-engine)
#add_executable(cpp_test, src/test.cpp)

add_test(NAME sudoku-engine-test COMMAND sudoku-engine-test)
add_test(NAME sudoku-solver-test COMMAND sudoku-solver)
add_test(NAME sudoku-data-test COMMAND sudoku-data-test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

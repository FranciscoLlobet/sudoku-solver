cmake_minimum_required(VERSION 3.0.0)
project(sudoku-solver VERSION 0.1.0)

include(CTest)
enable_testing()

# Create Sudoku Engine C-Library
add_library(sudoku-engine sudoku_engine.c sudoku_common.c)

# Library Test Executable (System Test)
add_executable(test-sudoku-library test-sudoku-library.cc) 

# Test Data Excecutable (System Test)
add_executable(test-sudoku-data test-sudoku-data.cc)

# Sudoku Engine Test (Unit Test)
add_executable(test-sudoku-engine test-sudoku-engine.cc sudoku_common.c)

# Link to test the Sudoku Library
target_link_libraries(test-sudoku-library PUBLIC sudoku-engine)

target_link_libraries(test-sudoku-data PUBLIC sudoku-engine)

add_test(NAME sudoku-engine-test COMMAND test-sudoku-library)
add_test(NAME sudoku-solver-test COMMAND test-sudoku-engine)
add_test(NAME sudoku-data-test COMMAND sudoku-data-test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

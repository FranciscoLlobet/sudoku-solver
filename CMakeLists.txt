cmake_minimum_required(VERSION 3.0.0)
project(sudoku-solver VERSION 0.1.0)

include(CTest)
enable_testing()

# Compile new sudoku C-library
add_library(sudoku sudoku.c)

# Compile new Sudoku C++ Library
add_library(sudoku_cc sudoku.cc sudoku.c)

# Unittest C-Library
add_executable(unittest-sudoku unittest-sudoku.cc)

# Data Test C++ Version
add_executable(test-sudoku data-sudoku.cc test-sudoku.cc)
add_executable(bench-sudoku bench-sudoku.cc test-sudoku.cc)

target_link_libraries(test-sudoku PUBLIC sudoku_cc)
target_link_libraries(bench-sudoku PUBLIC sudoku_cc)

add_test(NAME unittest-sudoku COMMAND unittest-sudoku)
add_test(NAME test-sudoku COMMAND test-sudoku)
add_test(NAME bench-sudoku COMMAND bench-sudoku)

# Link to test the Sudoku Library
#target_link_libraries(test-sudoku-library PUBLIC sudoku-engine)
#target_link_libraries(test-sudoku-data PUBLIC sudoku-engine)
#target_link_libraries(test-sudoku PUBLIC sudoku-engine)

#add_test(NAME sudoku-engine-test COMMAND test-sudoku-library)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

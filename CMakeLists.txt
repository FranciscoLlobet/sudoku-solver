cmake_minimum_required(VERSION 3.12)
project(sudoku-solver VERSION 0.1.0)
cmake_policy(SET CMP0057 NEW)

set(CMAKE_CXX_STANDARD 14)

include(CTest)
enable_testing()

set(PYTHON_EXECUTABLE  /C/msys64/clang/bin/python3.exe)
#et(PYTHON_LIBRARY /C/msys64/usr/clanf/libpython3.9.dll.a)

find_package(Python COMPONENTS Interpreter Development)
find_package(pybind11 CONFIG)
find_package(benchmark REQUIRED)

# Add doctest as light environment
add_subdirectory(doctest)

include_directories(include)
include_directories(src)

include_directories(${pybind11_INCLUDE_DIR})

# Compile new sudoku C-library
add_library(sudoku src/sudoku.c)
add_library(sudoku_cc src/sudoku.cc src/sudoku.c)
add_library(test-auxiliary test-sudoku.cc)

add_executable(unittest-sudoku unittest-sudoku.cc)
add_executable(test-sudoku data-sudoku.cc)
add_executable(benchmark-sudoku benchmark-sudoku.cc)

pybind11_add_module(sudoku_solver SHARED src/sudoku_py.cc)


target_compile_features(sudoku PRIVATE c_std_17)
target_compile_features(test-sudoku PRIVATE cxx_std_17)

target_link_libraries(test-sudoku PUBLIC test-auxiliary sudoku_cc)
target_link_libraries(benchmark-sudoku benchmark::benchmark test-auxiliary sudoku_cc)
target_link_libraries(sudoku_solver PRIVATE pybind11::module sudoku_cc)

add_test(NAME unittest-sudoku COMMAND unittest-sudoku)
add_test(NAME test-sudoku COMMAND test-sudoku)
add_test(NAME benchmark-sudoku COMMAND benchmark-sudoku)
add_test(NAME behave_tests
         COMMAND ${Python_EXECUTABLE} -m behave
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/features)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)



cmake_minimum_required(VERSION 3.0.0)
project(sudoku-solver VERSION 0.1.0)

include(CTest)
enable_testing()

add_subdirectory(doctest)

include_directories(include)
include_directories(src)
#add_subdirectory(src)

# include the google benchmark package 
find_package(benchmark REQUIRED)
find_package(GTest REQUIRED)

find_package(gRPC REQUIRED)
find_packagE(OpenSSL REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Capnproto REQUIRED)

# Compile new sudoku C-library
add_library(sudoku src/sudoku.c)
add_library(sudoku_cc src/sudoku.cc src/sudoku.c)
add_library(test-auxiliary test-sudoku.cc)

add_executable(unittest-sudoku unittest-sudoku.cc)
add_executable(test-sudoku data-sudoku.cc)
add_executable(benchmark-sudoku benchmark-sudoku.cc)

target_compile_features(sudoku PRIVATE c_std_17)
target_compile_features(test-sudoku PRIVATE cxx_std_17)

target_link_libraries(test-sudoku PUBLIC test-auxiliary sudoku_cc)
target_link_libraries(benchmark-sudoku benchmark::benchmark test-auxiliary sudoku_cc)

add_test(NAME unittest-sudoku COMMAND unittest-sudoku)
add_test(NAME test-sudoku COMMAND test-sudoku)
add_test(NAME benchmark-sudoku COMMAND benchmark-sudoku)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

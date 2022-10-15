//#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
//#include "doctest/doctest/doctest.h"
#include "hayai/src/hayai.hpp"

#include <fstream>
#include <iostream>

#include "sudoku.hh"
#include "test-sudoku.hh"

using namespace std;

int main()
{
    hayai::ConsoleOutputter consoleOutputter;

    hayai::Benchmarker::AddOutputter(consoleOutputter);
    hayai::Benchmarker::RunAllTests();
    return 0;
}

BENCHMARK(sudoku, validPuzzles, 10, 10)
{
    for (auto x : validTestPuzzles)
    {
        SudokuPuzzle p(x);
        p.Solve();
    }
}

BENCHMARK(sudoku, puzzles0_kaggle, 1, 1)
{
    string file_name = "../data/puzzles0_kaggle";

    (void)Process_File(file_name);

    ResetMaxLevel();
    ResetSolveCalls();
}
BENCHMARK(sudoku, puzzles1_unbiased, 1, 1)
{
    string file_name = "../data/puzzles1_unbiased";

    (void)Process_File(file_name);

    ResetMaxLevel();
    ResetSolveCalls();
}
BENCHMARK(sudoku, puzzles3_magictour_top1465, 1, 1)
{
    string file_name = "../data/puzzles3_magictour_top1465";

    (void)Process_File(file_name);

    ResetMaxLevel();
    ResetSolveCalls();
}
BENCHMARK(sudoku, puzzles2_17_clue, 1, 1)
{
    string file_name = "../data/puzzles2_17_clue";

    (void)Process_File(file_name);

    ResetMaxLevel();
    ResetSolveCalls();
}

#if 0
    "../data/puzzles4_forum_hardest_1905",
    "../data/puzzles5_forum_hardest_1905_11+",
    "../data/puzzles6_forum_hardest_1106",
    "../data/puzzles7_serg_benchmark",
    "../data/puzzles8_gen_puzzles"
#endif
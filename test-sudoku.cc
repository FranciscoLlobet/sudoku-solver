#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest/doctest.h"

#include <fstream>
#include <iostream>

#include "test-sudoku.hh"

using namespace std;

#include "sudoku.cc"

TEST_CASE("Solvable Puzzles")
{
    for(auto x : validTestPuzzles)
    {
        SudokuPuzzle p(x);
        CHECK(SUDOKU_RC_SUCCESS == p.Solve());
    }
}

TEST_CASE("Unsolvable Puzzles")
{
    for(auto x : invalidTestPuzzles)
    {
        SudokuPuzzle p(x);
        CHECK(SUDOKU_RC_ERROR == p.Solve());
    }
}
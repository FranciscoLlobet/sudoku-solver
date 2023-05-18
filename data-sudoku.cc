#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest/doctest.h"
//#include "hayai/src/hayai.hpp"

#include <fstream>
#include <iostream>

#include "sudoku.hh"
#include "test-sudoku.hh"

using namespace std;

TEST_CASE("Solvable Puzzles")
{
    for (auto x : validTestPuzzles)
    {
        SudokuPuzzle p(x);
        CHECK(SUDOKU_RC_SUCCESS == p.Solve());
    }
}

TEST_CASE("Unsolvable Puzzles")
{
    for (auto x : invalidTestPuzzles)
    {
        SudokuPuzzle p(x);
        CHECK(SUDOKU_RC_ERROR == p.Solve());
    }
}

/**
 * @brief This Dataset tests (mostly) the pruning algorithm
 *
 * Basically one solve() call per puzzle
 *
 * Max Level: 5
 * Solves Per Puzzle: 1.00081
 *
 */
TEST_CASE("Puzzle0 Test for pruning algorithm (100k)")
{

    string file_name = "../data/puzzles0_kaggle";

    auto [success, prune, error, count] = Process_File(file_name);

    CHECK(count == success);
    CHECK(0 == error);
    CHECK(0 == prune);
    double solves_per_puzzle = (double)GetSolveCalls() / (double)count;
    cout << count << " Max Level: " << GetMaxLevel() << " Solve Calls: " << GetSolveCalls() << " spp: " << solves_per_puzzle << endl;

    /* Reset max_level and solve calls */
    ResetMaxLevel();
    ResetSolveCalls();
}


#if 1
/**
 * @brief
 *
 * Solves Per Puzzle: 20.9701
 * Max level: 17
 */
TEST_CASE("Puzzle1 Test. 1M Sudoku Puzzles")
{
    string file_name = "../data/puzzles1_unbiased";

    auto [success, prune, error, count] = Process_File(file_name);

    CHECK(count == success);
    CHECK(0 == error);
    CHECK(0 == prune);
    double solves_per_puzzle = (double)GetSolveCalls() / (double)count;
    cout << count << " Max Level: " << GetMaxLevel() << " Solve Calls: " << GetSolveCalls() << " spp: " << solves_per_puzzle << endl;

    /* Reset max_level and solve calls */
    ResetMaxLevel();
    ResetSolveCalls();
}
#endif
/**
 * @brief
 *
 * Solves Per Puzzle: 664.712
 * Max level: 20ma
 */
TEST_CASE("Puzzle3 Test.")
{
    string file_name = "../data/puzzles3_magictour_top1465";

    auto [success, prune, error, count] = Process_File(file_name);

    CHECK(count == success);
    CHECK(0 == error);
    CHECK(0 == prune);

    double solves_per_puzzle = (double)GetSolveCalls() / (double)count;
    cout << count << " Max Level: " << GetMaxLevel() << " Solve Calls: " << GetSolveCalls() << " spp: " << solves_per_puzzle << endl;

    /* Reset max_level and solve calls */
    ResetMaxLevel();
    ResetSolveCalls();
}
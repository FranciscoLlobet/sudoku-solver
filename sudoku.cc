#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest/doctest.h"

#include <fstream>
#include <iostream>

#include "sudoku.hh"

using namespace std;
#include "test_sudoku.hh"

static unsigned int max_level = 0;
static unsigned int solve_calls = 0;

unsigned int GetMaxLevel(void)
{
    return max_level;
}

unsigned int GetSolveCalls(void)
{
    return solve_calls;
}

SudokuPuzzle::SudokuPuzzle(void)
{
    (void)this->InitializePuzzle();
}

SudokuPuzzle::SudokuPuzzle(string p)
{
    (void)this->InitializePuzzle(p);
}

SudokuPuzzle::SudokuPuzzle(SudokuPuzzle_P p)
{
    (void)this->InitializePuzzle(p);
}

/* Initialize Blank Puzzle*/
Sudoku_RC_T SudokuPuzzle::InitializePuzzle(void)
{
    return (Sudoku_RC_T)Sudoku_InitializePuzzle(&(this->puzzle));
}

Sudoku_RC_T SudokuPuzzle::InitializePuzzle(string p)
{
    Sudoku_RC_T rc = (Sudoku_RC_T)Sudoku_InitializePuzzle(&(this->puzzle));

    if (SUDOKU_RC_SUCCESS == rc)
    {
        Sudoku_InitializeFromArray(&(this->puzzle), p.c_str());
    }

    return rc;
}

Sudoku_RC_T SudokuPuzzle::InitializePuzzle(SudokuPuzzle_P p)
{
    if ((SudokuPuzzle_P)NULL == p)
    {
        return SUDOKU_RC_ERROR;
    }
    else
    {
        (void)memmove(&(this->puzzle), p, sizeof(this->puzzle));
    }

    return SUDOKU_RC_SUCCESS;
}

Sudoku_RC_T SudokuPuzzle::InitializePuzzle(SudokuPuzzle *p)
{

    *this = *p;

    return SUDOKU_RC_SUCCESS;
}

Sudoku_RC_T SudokuPuzzle::Print(void)
{
    return (Sudoku_RC_T)Sudoku_PrintPuzzle(&(this->puzzle));
}

SudokuPuzzle *SudokuPuzzle::SetValue(Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, Sudoku_Values_T val)
{
    (void)Sudoku_SetValue(&(this->puzzle), row, col, val);
    return this;
}

Sudoku_Values_T SudokuPuzzle::GetValue(Sudoku_Row_Index_T row, Sudoku_Column_Index_T col)
{
    return (Sudoku_Values_T)Sudoku_GetValue(&(this->puzzle), row, col);
}

Sudoku_RC_T SudokuPuzzle::Solve(void)
{
    return Solve(0);
}

Sudoku_RC_T SudokuPuzzle::Solve(unsigned int level)
{
    max_level = (level > max_level) ? level : max_level;
    solve_calls++;

    /* Prune and Validate Grid */
    auto rc = PrunePuzzle(&(this->puzzle));

    while (rc == SUDOKU_RC_PRUNE)
    {
        auto cand = SimpleSelectionStrategy(&(this->puzzle));

        auto p_new = new SudokuPuzzle(&(this->puzzle)); // Creates new puzzle

        rc = (p_new->SetValue(cand.row, cand.col, cand.val))->Solve(level + 1);

        if (SUDOKU_RC_SUCCESS == rc)
        {
            /* Overwrite current puzzle */
            this->InitializePuzzle(p_new);
        }
        else if (SUDOKU_RC_ERROR == rc)
        {
            RemoveCandidate(&(this->puzzle), cand.row, cand.col, cand.val);
            rc = PrunePuzzle(&(this->puzzle));
        }

        delete (p_new);
    }

    return rc;
}

Sudoku_RC_T SudokuSolver::Solve(void)
{
    // Has to be rewritten for C++
    return SUDOKU_RC_SUCCESS;
}

TEST_CASE("Solvable Puzzles")
{
    SudokuPuzzle a(sudokuTestStrings[0]);
    SudokuPuzzle b(sudokuTestStrings[1]);
    SudokuPuzzle c(sudokuTestStrings[2]);
    SudokuPuzzle d(sudokuTestStrings[3]);

    CHECK(SUDOKU_RC_SUCCESS == a.Solve());
    CHECK(SUDOKU_RC_SUCCESS == b.Solve());
    CHECK(SUDOKU_RC_SUCCESS == c.Solve());
    CHECK(SUDOKU_RC_SUCCESS == d.Solve());
}

TEST_CASE("Unsolvable Puzzles")
{
    SudokuPuzzle e(sudokuTestStrings[4]);
    SudokuPuzzle f(sudokuTestStrings[5]);
    SudokuPuzzle g(sudokuTestStrings[6]);

    CHECK(SUDOKU_RC_ERROR == e.Solve());
    CHECK(SUDOKU_RC_ERROR == f.Solve());
    CHECK(SUDOKU_RC_ERROR == g.Solve());
}

#define MAX_LINES 40000


tuple<unsigned int, unsigned int, unsigned int, unsigned int> run_file_test(string file_name)
{
    unsigned int success = 0;
    unsigned int prune = 0;
    unsigned int error = 0;
    unsigned int count = 0;
    ifstream test_data_file;
    string data_array;
    
    test_data_file.open(file_name);

    while (getline(test_data_file, data_array))
    {
        if (81 == data_array.length())
        {
            SudokuPuzzle p(data_array);

            auto rc = p.Solve();
            switch (rc)
            {
            case SUDOKU_RC_SUCCESS:
                success++;
                break;
            case SUDOKU_RC_PRUNE:
                prune++;
                break;
            default:
                error++;
            }
            count++;
        }
    }

    test_data_file.close();

    return make_tuple(success, prune, error, count);
}


/**
 * @brief This Dataset tests (mostly) the pruning algorithm
 *
 * Basically one solve() call per puzzle
 *
 */
TEST_CASE("Puzzle0 Test for pruning algorithm (100k)")
{
    string file_name = "../data/puzzles0_kaggle";
    
    auto[success, prune, error, count] = run_file_test(file_name);

    CHECK(count == success);
    CHECK(0 == error);
    CHECK(0 == prune);
    double solves_per_puzzle = (double)GetSolveCalls() / (double)count;
    cout << count << " Max Level: " << GetMaxLevel() << " Solve Calls: " << GetSolveCalls() << " spp: " << solves_per_puzzle << endl;

    /* Reset max_level and solve calls */
    max_level = 0;
    solve_calls = 0;
}

/**
 * @brief
 *
 * Current benchmark: 25 Solve Calls per puzzle
 */
TEST_CASE("Puzzle1 Test. 1M Sudoku Puzzles")
{
    string file_name = "../data/puzzles1_unbiased";
    
    auto[success, prune, error, count] = run_file_test(file_name);

    CHECK(count == success);
    CHECK(0 == error);
    CHECK(0 == prune);
    double solves_per_puzzle = (double)GetSolveCalls() / (double)count;
    cout << count << " Max Level: " << GetMaxLevel() << " Solve Calls: " << GetSolveCalls() << " spp: " << solves_per_puzzle << endl;

    /* Reset max_level and solve calls */
    max_level = 0;
    solve_calls = 0;
}



TEST_CASE("Puzzle3 Test.")
{
    string file_name = "../data/puzzles3_magictour_top1465";
    
    auto[success, prune, error, count] = run_file_test(file_name);

    CHECK(count == success);
    CHECK(0 == error);
    CHECK(0 == prune);
    
    double solves_per_puzzle = (double)GetSolveCalls() / (double)count;
    cout << count << " Max Level: " << GetMaxLevel() << " Solve Calls: " << GetSolveCalls() << " spp: " << solves_per_puzzle << endl;

    /* Reset max_level and solve calls */
    max_level = 0;
    solve_calls = 0;
}

TEST_CASE("Puzzle6 Test.")
{
    string file_name = "../data/puzzles6_forum_hardest_1106";
    
    auto[success, prune, error, count] = run_file_test(file_name);

    CHECK(count == success);
    CHECK(0 == error);
    CHECK(0 == prune);
    
    double solves_per_puzzle = (double)GetSolveCalls() / (double)count;
    cout << count << " Max Level: " << GetMaxLevel() << " Solve Calls: " << GetSolveCalls() << " spp: " << solves_per_puzzle << endl;

    /* Reset max_level and solve calls */
    max_level = 0;
    solve_calls = 0;
}


TEST_CASE("Puzzle7 Test.")
{
    string file_name = "../data/puzzles7_serg_benchmark";
    
    auto[success, prune, error, count] = run_file_test(file_name);

    CHECK(count == success);
    CHECK(0 == error);
    CHECK(0 == prune);
    
    double solves_per_puzzle = (double)GetSolveCalls() / (double)count;
    cout << count << " Max Level: " << GetMaxLevel() << " Solve Calls: " << GetSolveCalls() << " spp: " << solves_per_puzzle << endl;

    /* Reset max_level and solve calls */
    max_level = 0;
    solve_calls = 0;
}
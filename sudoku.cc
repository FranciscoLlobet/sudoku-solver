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

TEST_CASE("FileTest")
{

    ifstream test_data_file;

    test_data_file.open("../sudoku17.txt");

    string data_array;
    size_t count = 0;

    while (getline(test_data_file, data_array))
    {
        if (81 == data_array.length())
        {
            SudokuPuzzle p(data_array);

            CHECK(0 == p.Solve());
            count++;
        }
    }

    test_data_file.close();
    cout << count << " Max Level: " << GetMaxLevel() << " Solve Calls: " << GetSolveCalls() << endl;
}
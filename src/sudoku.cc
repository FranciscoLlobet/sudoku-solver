/**
 * @file
 * @author Francisco Llobet (llobetblandino@gmail.com)
 * @brief Sudoku Solver C++ Library
 * @version 0.1
 * @date 2022-09-24
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "sudoku.hh"

/* Give access to private C-Library*/
#include "_sudoku.h"

using namespace std;

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

void ResetMaxLevel(void)
{
    max_level = 0;
}

void ResetSolveCalls(void)
{
    solve_calls = 0;
}

SudokuPuzzle::SudokuPuzzle(void)
{
    this->puzzle = new SudokuPuzzle_S();
    (void)this->InitializePuzzle();
}

SudokuPuzzle::SudokuPuzzle(string p)
{
    this->puzzle = new SudokuPuzzle_S();
    (void)this->InitializePuzzle(p);
}

SudokuPuzzle::SudokuPuzzle(SudokuPuzzle_P p)
{
    this->puzzle = new SudokuPuzzle_S();
    (void)this->InitializePuzzle(p);
}

/* Initialize Blank Puzzle*/
Sudoku_RC_T SudokuPuzzle::InitializePuzzle(void)
{
    return Sudoku_InitializePuzzle(this->puzzle);
}

Sudoku_RC_T SudokuPuzzle::InitializePuzzle(string p)
{
    Sudoku_RC_T rc = Sudoku_InitializePuzzle(this->puzzle);

    if (SUDOKU_RC_SUCCESS == rc)
    {
        rc = Sudoku_InitializeFromArray(this->puzzle, p.c_str());
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
        (void)memmove(this->puzzle, p, sizeof(struct SudokuPuzzle_S));
    }

    return SUDOKU_RC_SUCCESS;
}

Sudoku_RC_T SudokuPuzzle::InitializePuzzle(SudokuPuzzle *p)
{
    if ((SudokuPuzzle *)NULL == p)
    {
        return SUDOKU_RC_ERROR;
    }

    *this = *p;

    return SUDOKU_RC_SUCCESS;
}

Sudoku_RC_T SudokuPuzzle::Print(void)
{
    return (Sudoku_RC_T)Sudoku_PrintPuzzle(this->puzzle);
}

SudokuPuzzle *SudokuPuzzle::SetValue(Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, Sudoku_Values_T val)
{
    if (SUDOKU_RC_SUCCESS == Sudoku_SetValue(this->puzzle, row, col, val))
    {
        return this;
    }

    return NULL;
}

SudokuPuzzle *SudokuPuzzle::SetValue(Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, Sudoku_BitValues_T val)
{
    if (SUDOKU_RC_SUCCESS == Sudoku_SetValueUsingBitmask(this->puzzle, row, col, val))
    {
        return this;
    }
    return NULL;
}

Sudoku_Values_T SudokuPuzzle::GetValue(Sudoku_Row_Index_T row, Sudoku_Column_Index_T col)
{
    return (Sudoku_Values_T)Sudoku_GetValue(this->puzzle, row, col);
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
    auto rc = Sudoku_PrunePuzzle(this->puzzle);

    while (rc == SUDOKU_RC_PRUNE)
    {
        Sudoku_Row_Index_T row = 0;
        Sudoku_Column_Index_T col = 0;
        auto cand = Sudoku_SelectCandidate(this->puzzle, &row, &col);

        auto p_new = new SudokuPuzzle(this->puzzle); // Creates new puzzle

        rc = (p_new->SetValue(row, col, cand))->Solve(level + 1);

        if (SUDOKU_RC_SUCCESS == rc)
        {
            /* Overwrite current puzzle */
            this->InitializePuzzle(p_new);
        }
        else if (SUDOKU_RC_ERROR == rc)
        {
            (void)Sudoku_RemoveCandidate(this->puzzle, row, col, cand);
            rc = Sudoku_PrunePuzzle(this->puzzle);
        }

        delete (p_new);
    }

    return rc;
}

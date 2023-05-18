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

#include <sstream>

/* Give access to private C-Library*/
#include "_sudoku.h"

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

SudokuPuzzle::SudokuPuzzle(const std::string &p)
{
    this->puzzle = new SudokuPuzzle_S();
    (void)this->InitializePuzzle(p);
}

SudokuPuzzle::SudokuPuzzle(SudokuPuzzle_P p)
{
    this->puzzle = new SudokuPuzzle_S();
    (void)this->InitializePuzzle(p);
}

SudokuPuzzle::SudokuPuzzle(const SudokuPuzzle &p)
{
    this->puzzle = new SudokuPuzzle_S();
    memcpy(this->puzzle, p.puzzle, sizeof(SudokuPuzzle_S));
}

SudokuPuzzle::~SudokuPuzzle(void)
{
    delete this->puzzle;
}

SudokuPuzzle &SudokuPuzzle::operator=(const SudokuPuzzle &p)
{
    if (this == &p)
    {
        return *this;
    }

    if (this->puzzle != nullptr)
    {
        delete this->puzzle;
    }

    this->puzzle = new SudokuPuzzle_S(*p.puzzle);
    return *this;
}

/* Initialize Blank Puzzle*/
Sudoku_RC_T SudokuPuzzle::InitializePuzzle(void)
{
    return Sudoku_InitializePuzzle(this->puzzle);
}

Sudoku_RC_T SudokuPuzzle::InitializePuzzle(const std::string &p)
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

Sudoku_RC_T SudokuPuzzle::InitializePuzzle(const SudokuPuzzle *p)
{
    if ((SudokuPuzzle *)NULL == p)
    {
        return SUDOKU_RC_ERROR;
    }

    *this = *p;

    return SUDOKU_RC_SUCCESS;
}

Sudoku_RC_T SudokuPuzzle::InitializePuzzle(const SudokuPuzzle &p)
{
    if (this == &p)
    {
        return SUDOKU_RC_SUCCESS;
    }

    *this = p;
    return SUDOKU_RC_SUCCESS;
}

std::string SudokuPuzzle::GetPuzzleAsString(void)
{
    std::stringstream puzzle_stream;

    for (Sudoku_Row_Index_T  r = 0; r < NUM_ROWS; r++)
    {
        for (Sudoku_Column_Index_T  c = 0; c < NUM_COLS; c++)
        {
            auto val = Sudoku_GetValue(this->puzzle, r, c);
            puzzle_stream << val;
        }
    }

    return puzzle_stream.str();
}

Sudoku_RC_T SudokuPuzzle::SetValue(Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, Sudoku_Values_T val)
{
    return Sudoku_SetValue(this->puzzle, row, col, val);
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

Sudoku_RC_T SudokuPuzzle::Check(void)
{
    return Sudoku_Check(this->puzzle);
}

Sudoku_RC_T SudokuPuzzle::Solve(void)
{
    return Solve(0);
}

/**
 * @brief Recursively solves the Sudoku Puzzle using a pruning and backtracking algorithm.
 *
 * The algorithm works as follows:
 * 1. Prune the puzzle using the Sudoku_PrunePuzzle() function.
 * 2. If the puzzle is solved, return success. If it is invalid, return an error.
 * 3. Select a candidate value for a cell with the least number of candidates using the Sudoku_SelectCandidate() function.
 * 4. Create a new puzzle and set the chosen candidate value in the new puzzle.
 * 5. Recursively call the Solve() method on the new puzzle with an incremented level.
 * 6. If the recursive call succeeds, overwrite the current puzzle with the new puzzle.
 * 7. If the recursive call fails, remove the candidate value from the current puzzle and repeat the pruning.
 *
 * @param level Level of recursion.
 * @return Sudoku_RC_T Result code.
 */
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

        SudokuPuzzle p_new(*this);

        rc = (p_new.SetValue(row, col, cand))->Solve(level + 1);

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
    }

    return rc;
}

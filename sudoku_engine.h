#ifndef SUDOKU_ENGINE_H_INCLUDED
#define SUDOKU_ENGINE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "sudoku.h"

enum CellType_E
{
    CELL_TYPE_INVALID = 0,
    CELL_TYPE_SIMPLE_CELL = 1,
    CELL_TYPE_ROW = 2,
    CELL_TYPE_COL = 3,
    CELL_TYPE_SUB = 4,
};

struct SudokuCell_S
{
    int value;
    uint32_t value_mask;
    enum CellType_E cell_type;
    uint32_t candidates;    // Candidate Bitmask
};

struct SudokuPuzzle_S
{
    struct SudokuCell_S grid[NUM_ROWS][NUM_COLS]; /* Main Grid */
    struct SudokuCell_S row[NUM_ROWS]; /* Row Candidates */
    struct SudokuCell_S col[NUM_COLS]; /* Column Candidates */
    struct SudokuCell_S sub[NUM_SUBGRID_ROWS][NUM_SUBGRID_COLS]; /* Subgrids*/
};

struct BackTrackCandidate_S
{
    enum SudokuValues_E val;
    unsigned int row;
    unsigned int col;
};

/* Initialize an empty puzzle object */
enum Sudoku_RC_E InitializePuzzle(SudokuPuzzle_P puzzle);

enum SudokuValues_E SetValue(SudokuPuzzle_P puzzle, enum SudokuValues_E val, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col);
enum SudokuValues_E GetValue(SudokuPuzzle_P puzzle, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col);

int CountCandidates(SudokuPuzzle_P p, unsigned int row, unsigned int col);

/**
 * @brief Prune and validate the grid
 * 
 * @param p 
 * @return enum Sudoku_RC_E 
 */
enum Sudoku_RC_E PrunePuzzle(SudokuPuzzle_P p);


int pruneCandidates(SudokuPuzzle_P p);

struct BackTrackCandidate_S SimpleSelectionStrategy(SudokuPuzzle_P p);
void RemoveCandidate(SudokuPuzzle_P p, unsigned int row, unsigned int col, Sudoku_Values_T val);

enum Sudoku_RC_E Solve(SudokuPuzzle_P p, int level);


#ifdef __cplusplus
}
#endif

#endif // SUDOKU_ENGINE_H_INCLUDED

#ifndef PRIV_SUDOKU_H_INCLUDED
#define PRIV_SUDOKU_H_INCLUDED

#ifdef __cplusplus
extern "C"
{
#endif

#include "sudoku.h"

    /**
     * @brief Represents a single cell in the Sudoku grid.
     */
    struct SudokuCell_S
    {
        uint32_t value;      /**< Value mask. */
        uint32_t candidates; /**< Candidate mask. */
    };

    /**
     * @brief Represents the Sudoku grid and its metadata.
     */
    struct SudokuPuzzle_S
    {
        struct SudokuCell_S grid[NUM_ROWS][NUM_COLS];                /**< A 9x9 grid of Sudoku cells. */
        uint32_t row_candidates[NUM_ROWS];                           /**< Candidates for each row. */
        uint32_t col_candidates[NUM_COLS];                           /**< Candidates for each column. */
        uint32_t sub_candidates[NUM_SUBGRID_ROWS][NUM_SUBGRID_COLS]; /**< Candidates for each subgrid. */

        unsigned int n_candidates[NUM_ROWS][NUM_COLS]; /**< Number of candidates for each cell in the grid. */
        unsigned int val_n_candidates[NUM_CANDIDATES]; /**< Number of candidates for each possible value (1 to 9) in the puzzle. */
    };

#ifdef __cplusplus
}
#endif

#endif // PRIV_SUDOKU_H_INCLUDED
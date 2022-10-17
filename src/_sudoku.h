#ifndef PRIV_SUDOKU_H_INCLUDED
#define PRIV_SUDOKU_H_INCLUDED

#ifdef __cplusplus
extern "C"
{
#endif

#include "sudoku.h"

    struct SudokuCell_S
    {
        uint32_t value;      /* Value Mask */
        uint32_t candidates; /* Candidate mask*/
    };

    struct SudokuPuzzle_S
    {
        struct SudokuCell_S grid[NUM_ROWS][NUM_COLS];
        uint32_t row_candidates[NUM_ROWS];
        uint32_t col_candidates[NUM_COLS];
        uint32_t sub_candidates[NUM_SUBGRID_ROWS][NUM_SUBGRID_COLS];

        unsigned int n_candidates[NUM_ROWS][NUM_COLS];
        unsigned int val_n_candidates[NUM_CANDIDATES];
    };

#ifdef __cplusplus
}
#endif

#endif // PRIV_SUDOKU_H_INCLUDED
#ifndef SUDOKU_H_INCLUDED
#define SUDOKU_H_INCLUDED

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

    /* ********************************************************************** */
    /* CONSTANT DEFINITIONS ************************************************* */
    /* ********************************************************************** */

#define NUM_ROWS 9 // Number of rows in a puzzle.
#define NUM_COLS 9 // Number of columns in a puzzle.
#define NUM_SUBGRID 9 // Number of subgrids in a puzzle.
#define NUM_SUBGRID_COLS 3 // Number of columns in a subgrid.
#define NUM_SUBGRID_ROWS 3 // Number of columns in a subgrid.
#define NUM_CANDIDATES 9 // Number of candidates a cell can have.

#define SUDOKU_MASK_NONE ((uint32_t)0)
#define SUDOKU_MASK_1 ((uint32_t)(1 << 0))
#define SUDOKU_MASK_2 ((uint32_t)(1 << 1))
#define SUDOKU_MASK_3 ((uint32_t)(1 << 2))
#define SUDOKU_MASK_4 ((uint32_t)(1 << 3))
#define SUDOKU_MASK_5 ((uint32_t)(1 << 4))
#define SUDOKU_MASK_6 ((uint32_t)(1 << 5))
#define SUDOKU_MASK_7 ((uint32_t)(1 << 6))
#define SUDOKU_MASK_8 ((uint32_t)(1 << 7))
#define SUDOKU_MASK_9 ((uint32_t)(1 << 8))
#define SUDOKU_MASK_ALL ((uint32_t)(SUDOKU_MASK_1 | SUDOKU_MASK_2 | SUDOKU_MASK_3 | SUDOKU_MASK_4 | SUDOKU_MASK_5 | SUDOKU_MASK_6 | SUDOKU_MASK_7 | SUDOKU_MASK_8 | SUDOKU_MASK_9))

    /**
     * @brief Sudoku Cell Values.
     * 
     * Enumeration of valid and some invalid values a sudoku cell can hold
     * 
     */
    typedef enum SudokuValues_E
    {
        SUDOKU_INVALID_VALUE = -1,
        SUDOKU_NO_VALUE = 0,
        SUDOKU_VALUE_1 = 1,
        SUDOKU_VALUE_2 = 2,
        SUDOKU_VALUE_3 = 3,
        SUDOKU_VALUE_4 = 4,
        SUDOKU_VALUE_5 = 5,
        SUDOKU_VALUE_6 = 6,
        SUDOKU_VALUE_7 = 7,
        SUDOKU_VALUE_8 = 8,
        SUDOKU_VALUE_9 = 9,
        SUDOKU_INVALID_VALUE_10 = 10
    } Sudoku_Values_T;

    /// @brief Sudoku Return Code Enumeration.
    typedef enum Sudoku_RC_E
    {
        SUDOKU_RC_ERROR = -1,  /* Error Code */
        SUDOKU_RC_SUCCESS = 0, /* Operation was succesful */
        SUDOKU_RC_PRUNE = 1,   /* Operation was succesful, puzzle is not completely solved yet */
    } Sudoku_RC_T;

    /// @brief Type used for Sudoku Row Index.
    typedef unsigned int Sudoku_Row_Index_T;

    /// @brief Type used for Sudoku Column Index.
    typedef unsigned int Sudoku_Column_Index_T;

    /**
     * @brief
     *
     */
    typedef struct SudokuCell_S *SudokuCell_P;

    /**
     * @brief Sudoku Puzzle Object Reference.
     *
     * Forward declaration of the Sudoku Puzzle Object as C-Style opaque type
     */
    typedef struct SudokuPuzzle_S *SudokuPuzzle_P;

    /* ********************************************************************** */
    /* ********************************************************************** */
    /* ********************************************************************** */

    /**
     * @brief Initialize a Sudoku Solver Puzzle Object.
     *
     * This will initialize all cells in a puzzle object to @ref SUDOKU_NO_VALUE
     * and all candidates to ref SUDOKU_MASK_ALL.
     *
     * @param p : Valid reference to Sudoku Puzzle.
     *
     * @return enum Sudoku_RC_E
     */
    enum Sudoku_RC_E Sudoku_InitializePuzzle(SudokuPuzzle_P p);

    /**
     * @brief Initialize a Sudoku Solver Puzzle Object from string array notation.
     *
     * @param p : Valid reference to Sudoku Puzzle.
     * @param sudoku_array : C-Style string containing a Sudoku puzzle in array notation.
     */
    void Sudoku_InitializeFromArray(SudokuPuzzle_P p, const char *sudoku_array);

    /**
     * @brief Set numeric value of a puzzle cell using row and column indexes.
     *
     * @param p : Valid reference to Sudoku Puzzle.
     * @param row : Row index.
     * @param col : Column Index.
     * @param val : Numeric value to asign.
     * @return Sudoku_Values_T
     */
    Sudoku_Values_T Sudoku_SetValue(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, unsigned int val);

    /**
     * @brief Get numeric value of a puzzle cell using row and column indexes
     *
     * @param p
     * @param row
     * @param col
     * @return unsigned int
     */
    unsigned int Sudoku_GetValue(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col);

    int Sudoku_GetNumCandidates(SudokuPuzzle_P p, unsigned int row, unsigned int col);

    SudokuPuzzle_P Sudoku_MallocPuzzle(void);
    void Sudoku_FreePuzzle(SudokuPuzzle_P puzzle);
    SudokuPuzzle_P Sudoku_CopyPuzzle(SudokuPuzzle_P destination, SudokuPuzzle_P source);

    int Sudoku_Solve(SudokuPuzzle_P p);
    int Sudoku_PrintPuzzle(SudokuPuzzle_P p);
    int Sudoku_RemoveCandidate(SudokuCell_P cell, int cand);

    /* Set Cell Value in Puzzle given row and column */
    enum SudokuValues_E SetValue(SudokuPuzzle_P puzzle, enum SudokuValues_E val, unsigned int row, unsigned int col);

#ifdef __cplusplus
}
#endif

#endif // SUDOKU_H_INCLUDED

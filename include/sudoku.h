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

#define NUM_ROWS 9             // Number of rows in a puzzle.
#define NUM_COLS 9             // Number of columns in a puzzle.
#define NUM_SUBGRID 9          // Number of subgrids in a puzzle.
#define NUM_SUBGRID_COLS 3     // Number of columns in a subgrid.
#define NUM_SUBGRID_ROWS 3     // Number of rows in a subgrid.
#define NUM_SUBGRID_ELEMENTS 9 // Number of elements in a subgrid
#define NUM_CANDIDATES 9       // Number of candidates a cell can have.

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
#define SUDOKU_MASK_INVALID ((uint32_t)UINT32_MAX)

    /**
     * @brief Sudoku Cell Values.
     *
     * Enumeration of valid and some invalid values a sudoku cell can hold
     *
     */
    typedef enum SudokuValues_E
    {
        SUDOKU_NOT_EXCLUSIVE_VALUE = -2, /* More than one value, will think about using 16 (?) */
        SUDOKU_INVALID_VALUE = -1,       /* Invalid value */
        SUDOKU_NO_VALUE = 0,             /* No value */
        SUDOKU_VALUE_1 = 1,
        SUDOKU_VALUE_2 = 2,
        SUDOKU_VALUE_3 = 3,
        SUDOKU_VALUE_4 = 4,
        SUDOKU_VALUE_5 = 5,
        SUDOKU_VALUE_6 = 6,
        SUDOKU_VALUE_7 = 7,
        SUDOKU_VALUE_8 = 8,
        SUDOKU_VALUE_9 = 9,
        SUDOKU_INVALID_VALUE_10 = 10 /* Out of bounds value */
    } Sudoku_Values_T;

    /**
     * @brief Bit Value Masks
     *
     */
    typedef enum SudokuBitValues_E
    {
        SUDOKU_BIT_INVALID_VALUE = (int32_t)SUDOKU_MASK_INVALID, /* May resolve to -1 if signed */
        SUDOKU_BIT_NO_VALUE = (int32_t)SUDOKU_MASK_NONE,         /* Bit mask for no value */
        SUDOKU_BIT_VALUE_1 = (int32_t)SUDOKU_MASK_1,
        SUDOKU_BIT_VALUE_2 = (int32_t)SUDOKU_MASK_2,
        SUDOKU_BIT_VALUE_3 = (int32_t)SUDOKU_MASK_3,
        SUDOKU_BIT_VALUE_4 = (int32_t)SUDOKU_MASK_4,
        SUDOKU_BIT_VALUE_5 = (int32_t)SUDOKU_MASK_5,
        SUDOKU_BIT_VALUE_6 = (int32_t)SUDOKU_MASK_6,
        SUDOKU_BIT_VALUE_7 = (int32_t)SUDOKU_MASK_7,
        SUDOKU_BIT_VALUE_8 = (int32_t)SUDOKU_MASK_8,
        SUDOKU_BIT_VALUE_9 = (int32_t)SUDOKU_MASK_9,
        SUDOKU_BIT_VALUE_ALL = (int32_t)SUDOKU_MASK_ALL /* 511 */
    } Sudoku_BitValues_T;

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
     * @brief Sudoku Cell Object Reference
     *
     * Forward declaration of the Sudoku Cell Object as C-Style opaque type
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
    Sudoku_RC_T Sudoku_InitializePuzzle(SudokuPuzzle_P p);

    /**
     * @brief Initialize a Sudoku Solver Puzzle Object from string array notation.
     *
     * @param p : Valid reference to Sudoku Puzzle.
     * @param sudoku_array : C-Style string containing a Sudoku puzzle in array notation.
     */
    Sudoku_RC_T Sudoku_InitializeFromArray(SudokuPuzzle_P p, const char *sudoku_array);

    /**
     * @brief Set numeric value of a puzzle cell using row and column indexes.
     *
     * @param p : Valid reference to Sudoku Puzzle.
     * @param row : Row index.
     * @param col : Column Index.
     * @param val : Numeric value to asign.
     */
    Sudoku_RC_T Sudoku_SetValue(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, int val);

    /**
     * @brief
     *
     * @param p
     * @param row
     * @param col
     * @param value
     * @return Sudoku_RC_T
     */
    Sudoku_RC_T Sudoku_SetValueUsingBitmask(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, Sudoku_BitValues_T value);

    /**
     * @brief Get numeric value of a puzzle cell using row and column indexes
     *
     * @param p
     * @param row
     * @param col
     * @return unsigned int
     */
    int Sudoku_GetValue(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col);

    /**
     * @brief Check the puzzle for validity or completeness
     *
     * @param p
     * @return Sudoku_RC_T
     */
    Sudoku_RC_T Sudoku_Check(SudokuPuzzle_P p);

    Sudoku_RC_T Sudoku_RemoveCandidate(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, Sudoku_BitValues_T candidate);

    Sudoku_BitValues_T Sudoku_GetCandidates(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col);

    Sudoku_BitValues_T Sudoku_SelectCandidate(SudokuPuzzle_P p, Sudoku_Row_Index_T *row, Sudoku_Column_Index_T *col);

    Sudoku_RC_T Sudoku_PrunePuzzle(SudokuPuzzle_P p);

    /* ********************************************************************** */
    /* ********************************************************************** */
    /* ********************************************************************** */
    /* ********************************************************************** */
    /* ********************************************************************** */
    /* ********************************************************************** */
    int Sudoku_PrintPuzzle(SudokuPuzzle_P p);

    SudokuPuzzle_P Sudoku_MallocPuzzle(void);
    void Sudoku_FreePuzzle(SudokuPuzzle_P puzzle);
    SudokuPuzzle_P Sudoku_CopyPuzzle(SudokuPuzzle_P destination, SudokuPuzzle_P source);

#ifdef __cplusplus
}
#endif

#endif // SUDOKU_H_INCLUDED

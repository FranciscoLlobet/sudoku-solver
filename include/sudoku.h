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

    /**
     * @brief Sudoku return code enumeration.
     *
     * This enumeration defines the possible return codes for various functions
     * used in the Sudoku solver. It covers success, error, and intermediate states.
     */
    typedef enum Sudoku_RC_E
    {
        SUDOKU_RC_ERROR = INT8_MIN,  /**< Generic error code */
        SUDOKU_RC_NULL_POINTER,      /**< Input had a NULL pointer */
        SUDOKU_RC_INVALID_INPUT,     /**< Invalid row or column input */
        SUDOKU_RC_INVALID_VALUE,     /**< Invalid input value. Accepted are: 0-9 */
        SUDOKU_RC_NOT_SOLVABLE = -1, /**< Sudoku puzzle is not solvable */
        SUDOKU_RC_SUCCESS = 0,       /**< Operation was successful */
        SUDOKU_RC_PRUNE = 1,         /**< Operation was successful, but the puzzle is not completely solved yet */
    } Sudoku_RC_T;

    /// @brief Type used for Sudoku Row Index.
    typedef size_t Sudoku_Row_Index_T;

    /// @brief Type used for Sudoku Column Index.
    typedef size_t Sudoku_Column_Index_T;

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
     * @brief Initialize a Sudoku puzzle object.
     *
     * This function initializes a Sudoku puzzle object by setting all cell values
     * to @ref SUDOKU_NO_VALUE and all candidates to @ref SUDOKU_MASK_ALL. It also
     * initializes the row, column, and subgrid candidates accordingly.
     *
     * @param p A valid reference to a Sudoku puzzle object.
     * @return Sudoku_RC_T Returns SUDOKU_RC_SUCCESS if the initialization is successful,
     *         SUDOKU_RC_ERROR if the provided puzzle pointer is NULL.
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
     * @brief Set the numeric value of a puzzle cell using row and column indexes.
     *
     * This function sets the value of a specific cell in the Sudoku puzzle. It also
     * checks for valid input parameters and returns appropriate error codes if any
     * of the input parameters are invalid.
     *
     * @param p : Valid reference to a Sudoku Puzzle.
     * @param row : Row index.
     * @param col : Column Index.
     * @param val : Numeric value to assign (0-9).
     * @return Sudoku_RC_T Returns the appropriate return code based on the operation outcome:
     * - SUDOKU_RC_SUCCESS: The operation was successful.
     * - SUDOKU_RC_NULL_POINTER: The provided Sudoku puzzle pointer is NULL.
     * - SUDOKU_RC_INVALID_INPUT: The row or column index is out of range.
     * - SUDOKU_RC_INVALID_VALUE: The provided value is not within the accepted range (0-9).
     */
    Sudoku_RC_T Sudoku_SetValue(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, int val);

    /**
     * @brief Set the numeric value of a puzzle cell using row and column indexes and a bitmask value.
     *
     * This function sets the value of a specific cell in the Sudoku puzzle using a bitmask value.
     * It also checks for valid input parameters and returns appropriate error codes if any
     * of the input parameters are invalid.
     *
     * Note: This function assumes that the `Sudoku_BitValues_T` enumeration is used for the input value.
     *
     * @param p : Valid reference to a Sudoku Puzzle.
     * @param row : Row index.
     * @param col : Column Index.
     * @param value : Numeric value to assign using a bitmask from the `Sudoku_BitValues_T` enumeration.
     * @return Sudoku_RC_T Returns the appropriate return code based on the operation outcome:
     * - SUDOKU_RC_SUCCESS: The operation was successful.
     * - SUDOKU_RC_NULL_POINTER: The provided Sudoku puzzle pointer is NULL.
     * - SUDOKU_RC_INVALID_INPUT: The row or column index is out of range.
     */
    Sudoku_RC_T Sudoku_SetValueUsingBitmask(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, Sudoku_BitValues_T value);

    /**
     * @brief Get the numeric value of a puzzle cell using row and column indexes.
     *
     * This function returns the numeric value of a cell in the puzzle.
     * In case of invalid input parameters, it returns negative error codes.
     *
     * @note The users of this function should handle the return value accordingly
     *       by checking for negative values as error codes.
     *
     * @param p : Valid reference to a Sudoku Puzzle.
     * @param row : Row index.
     * @param col : Column index.
     * @return int : Returns the numeric value of the cell (1-9) if successful.
     *               Returns a negative error code in case of invalid input:
     *               - SUDOKU_RC_NULL_POINTER if the provided puzzle pointer is NULL.
     *               - SUDOKU_RC_INVALID_INPUT if the row or column index is out of range.
     */
    int Sudoku_GetValue(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col);

    /**
     * @brief Checks if the given Sudoku puzzle is valid or not.
     *
     * This function takes a pointer to a SudokuPuzzle and checks if it's valid or not.
     * It calls four other static functions to perform the checks, namely checkRows,
     * checkCols, checkSubgrid, and checkEmptyVals. If the puzzle is invalid, this function
     * returns an appropriate error code.
     *
     * @param p Pointer to the SudokuPuzzle to be checked.
     * @return Sudoku_RC_T The result of the Sudoku check operation.
     * @retval SUDOKU_RC_SUCCESS If the puzzle is valid.
     * @retval SUDOKU_RC_ERROR If the input pointer is NULL.
     * @retval SUDOKU_RC_PRUNE If there are empty values in the puzzle.
     * @retval SUDOKU_RC_INVALID_INPUT If the input puzzle is invalid.
     * @retval SUDOKU_RC_NOT_SOLVABLE If the input puzzle is not solvable.
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

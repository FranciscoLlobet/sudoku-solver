/**
 * @file sudoku.hh
 * @author Francisco Llobet (llobetblandino@gmail.com)
 * @brief Sudoku Solver C++ Library
 * @version 0.1
 * @date 2022-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef SUDOKU_HH_INCLUDED
#define SUDOKU_HH_INCLUDED

#include <string>
#include "sudoku.h"

/**
 * @class SudokuPuzzle
 * @brief A class representing a Sudoku puzzle with methods for initialization, solving, and manipulation.
 */
class SudokuPuzzle
{
public:
    /**
     * @brief Default constructor, creates an empty Sudoku puzzle.
     */
    SudokuPuzzle(void);

    /**
     * @brief Constructor, creates a Sudoku puzzle using a string notation.
     * @param p The input puzzle in string notation.
     */
    SudokuPuzzle(std::string p);

    // Manipulation functions

    /**
     * @brief Initializes an empty puzzle.
     * @return Sudoku_RC_T The result code indicating success or failure.
     */
    Sudoku_RC_T InitializePuzzle(void);

    /**
     * @brief Initializes a puzzle using a string notation.
     * @param p The input puzzle in string notation.
     * @return Sudoku_RC_T The result code indicating success or failure.
     */
    Sudoku_RC_T InitializePuzzle(std::string p);

    /**
     * @brief Prints the Sudoku puzzle to the console.
     * @return Sudoku_RC_T The result code indicating success or failure.
     */
    Sudoku_RC_T Print(void);

    /**
     * @brief Sets a value in the puzzle.
     * @param row The row index.
     * @param col The column index.
     * @param val The value to set.
     * @return SudokuPuzzle A pointer to the modified puzzle object.
     */
    SudokuPuzzle *SetValue(Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, Sudoku_Values_T val);

    /**
     * @brief Gets a value from the puzzle.
     * @param row The row index.
     * @param col The column index.
     * @return Sudoku_Values_T The value at the specified row and column.
     */
    Sudoku_Values_T GetValue(Sudoku_Row_Index_T row, Sudoku_Column_Index_T col);

    /**
     * @brief Solves the puzzle using pruning and backtracking.
     * @return Sudoku_RC_T The result code indicating success or failure.
     */
    Sudoku_RC_T Solve(void); // Solve this puzzle

private:
    /**
     * @brief Constructor, creates a new Sudoku puzzle object.
     * @param p A pointer to an existing SudokuPuzzle_S object.
     */
    SudokuPuzzle(SudokuPuzzle_P p);

    /**
     * @brief Initializes a puzzle using a C-style reference.
     * @param p A pointer to an existing SudokuPuzzle_S object.
     * @return Sudoku_RC_T The result code indicating success or failure.
     */
    Sudoku_RC_T InitializePuzzle(SudokuPuzzle_P p);

    /**
     * @brief Initializes a puzzle using a C++ object.
     * @param p A pointer to an existing SudokuPuzzle object.
     * @return Sudoku_RC_T The result code indicating success or failure.
     */
    Sudoku_RC_T InitializePuzzle(SudokuPuzzle *p);

    /**
     * @brief Sets a value in the puzzle using a bitmask.
     * @param row The row index.
     * @param col The column index.
     * @param val The value bitmask to set.
     * @return SudokuPuzzle A pointer to the modified puzzle object.
     */
    SudokuPuzzle *SetValue(Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, Sudoku_BitValues_T val);

    /**
     * @brief Recursively solves the Sudoku Puzzle using pruning and backtracking.
     *
     * @param level Level of recursion.
     * @return Sudoku_RC_T Result code.
     */
    Sudoku_RC_T Solve(unsigned int level);

    /**
     * @brief A pointer to the Sudoku Puzzle structure.
     */
    SudokuPuzzle_P puzzle;
};

unsigned int GetMaxLevel(void);
unsigned int GetSolveCalls(void);
void ResetMaxLevel(void);
void ResetSolveCalls(void);

#endif // SUDOKU_HH_INCLUDED

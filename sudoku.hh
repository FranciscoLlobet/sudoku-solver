/**
 * @file sudoku.hh
 * @author Francisco Llobet (llobetblandino@gmail.com)
 * @brief Sudoku Solver C++ Library
 * @version 0.1
 * @date 2022-09-24
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef SUDOKU_HH_INCLUDED
#define SUDOKU_HH_INCLUDED

#include <string>
#include "sudoku.h"


class SudokuPuzzle
{
public:
    /**
     * @brief Construct a new and empty Sudoku Puzzle object
     */
    SudokuPuzzle(void);

    /**
     * @brief Construct a new Sudoku Puzzle object using string notation
     *
     * @param p : Sudoku Puzzle in string notation
     */
    SudokuPuzzle(std::string p);

    // Manipulation functions

    /**
     * @brief Initialize puzzle without arguments
     *
     * @return Sudoku_RC_T
     */
    Sudoku_RC_T InitializePuzzle(void);

    /**
     * @brief Initialize puzzle using the string notation
     *
     * @param p : Sudoku Puzzle in string notation
     * @return Sudoku_RC_T
     */
    Sudoku_RC_T InitializePuzzle(std::string p);

    /**
     * @brief Print Sudoku Puzzle to console
     *
     * @return Sudoku_RC_T
     */
    Sudoku_RC_T Print(void);

    /**
     * @brief Set the Value object
     *
     * @param row
     * @param col
     * @param val
     * @return SudokuPuzzle
     */
    SudokuPuzzle *SetValue(Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, Sudoku_Values_T val);

    /**
     * @brief Get the Value object
     *
     * @param row
     * @param col
     * @return Sudoku_Values_T
     */
    Sudoku_Values_T GetValue(Sudoku_Row_Index_T row, Sudoku_Column_Index_T col);

    /**
     * @brief Recursively solve puzzle object
     *
     * @return Sudoku_RC_T
     */
    Sudoku_RC_T Solve(void); // Solve this puzzle

private:
    /**
     * @brief Construct a new Sudoku Puzzle object
     *
     * @param p
     */
    SudokuPuzzle(SudokuPuzzle_P p);

    /**
     * @brief Initialize Puzzle using C-Style reference
     *
     * @param p
     * @return Sudoku_RC_T
     */
    Sudoku_RC_T InitializePuzzle(SudokuPuzzle_P p);

    /**
     * @brief Initialize Puzzle using Cxx object
     *
     * @param p
     * @return Sudoku_RC_T
     */
    Sudoku_RC_T InitializePuzzle(SudokuPuzzle *p);

    SudokuPuzzle *SetValue(Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, Sudoku_BitValues_T val);

    /**
     * @brief Recursively solve puzzle object
     *
     * @param level : Level of recursion
     * @return Sudoku_RC_T
     */
    Sudoku_RC_T Solve(unsigned int level);
   
    /**
     * @brief Sudoku Puzzle reference
     */
    SudokuPuzzle_P puzzle;
};

unsigned int GetMaxLevel(void);
unsigned int GetSolveCalls(void);
void ResetMaxLevel(void);
void ResetSolveCalls(void);

#endif // SUDOKU_HH_INCLUDED

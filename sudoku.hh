#ifndef SUDOKU_HH_INCLUDED
#define SUDOKU_HH_INCLUDED

#include "sudoku_engine.h"
#include <string>

class SudokuPuzzle
{
public:
    /* Constructor options */
    SudokuPuzzle(void);
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
     * @return Sudoku_Values_T
     */
    Sudoku_Values_T SetValue(Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, Sudoku_Values_T val);

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
     * @brief Recursively solve puzzle object
     *
     * @param level : Level of recursion
     * @return Sudoku_RC_T
     */
    Sudoku_RC_T Solve(unsigned int level);

    /**
     * @brief Sudoku Puzzle struct
     *
     */
    struct SudokuPuzzle_S puzzle;
};

using namespace std;

/* Create CPP based solver */
class SudokuSolver
{
public:
    Sudoku_RC_T Solve(void);
    Sudoku_RC_T Solve(SudokuPuzzle &p);                     // Solve this puzzle
    Sudoku_RC_T Solve(SudokuPuzzle &p, unsigned int level); // Recursive Level info
    Sudoku_RC_T Prune(void);                                // Solve this puzzle
};

#endif // SUDOKU_HH_INCLUDED

#ifndef SUDOKU_H_INCLUDED
#define SUDOKU_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define NUM_ROWS            9
#define NUM_COLS            9
#define NUM_SUBGRID         9
#define NUM_SUBGRID_COLS    3
#define NUM_SUBGRID_ROWS    3

#define NUM_CANDIDATES      9


enum SudokuValues_E
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
};

enum
{
    SUDOKU_CHECK_INVALID = -1,
    SUDOKU_CHECK_VALID_NO_CHANGE = 0,
    SUDOKU_CHECK_VALID_CHANGE = 1
};

enum
{
    SUDOKU_TEST_CANDIDATE_INVALID = -1,
    SUDOKU_TEST_CANDIDATE_NO_CHANGE = 0,
    SUDOKU_TEST_CANDIDATE_VALID = 1,
};

enum CellType_E
{
    CELL_TYPE_INVALID = 0,
    CELL_TYPE_SIMPLE_CELL = 1,
    CELL_TYPE_ROW = 2,
    CELL_TYPE_COL = 3,
    CELL_TYPE_SUB = 4,
};

/* Forward Declaration of the Sudoku Cell Reference*/
typedef struct SudokuCell_S * SudokuCell_P;

/* Forward Declaration of the Sudoku Puzzle Reference*/
typedef struct SudokuPuzzle_S * SudokuPuzzle_P;



#define SUDOKU_MASK_NONE ((uint32_t)0)
#define SUDOKU_MASK_1    ((uint32_t)(1<<0))
#define SUDOKU_MASK_2    ((uint32_t)(1<<1))
#define SUDOKU_MASK_3    ((uint32_t)(1<<2))
#define SUDOKU_MASK_4    ((uint32_t)(1<<3))
#define SUDOKU_MASK_5    ((uint32_t)(1<<4))
#define SUDOKU_MASK_6    ((uint32_t)(1<<5))
#define SUDOKU_MASK_7    ((uint32_t)(1<<6))
#define SUDOKU_MASK_8    ((uint32_t)(1<<7))
#define SUDOKU_MASK_9    ((uint32_t)(1<<8))
#define SUDOKU_MASK_ALL  ((uint32_t)(SUDOKU_MASK_1 | SUDOKU_MASK_2 | SUDOKU_MASK_3 | SUDOKU_MASK_4 | SUDOKU_MASK_5 | SUDOKU_MASK_6 | SUDOKU_MASK_7 | SUDOKU_MASK_8 | SUDOKU_MASK_9))


int Sudoku_InitializePuzzle(SudokuPuzzle_P p);
void Sudoku_InitializeFromArray(SudokuPuzzle_P p, const char *sudoku_array);
int Sudoku_SetValue(SudokuPuzzle_P p, unsigned int row, unsigned int col, int val);
int Sudoku_GetValue(SudokuPuzzle_P p, unsigned int row, unsigned int col);
int Sudoku_GetNumCandidates(SudokuPuzzle_P p, unsigned int row, unsigned int col);


SudokuPuzzle_P Sudoku_MallocPuzzle(void);
void Sudoku_FreePuzzle(SudokuPuzzle_P puzzle);
SudokuPuzzle_P Sudoku_CopyPuzzle(SudokuPuzzle_P destination, SudokuPuzzle_P source);














/* Set Cell Value in Puzzle given row and column */
enum SudokuValues_E SetCellValueInPuzzle(SudokuPuzzle_P puzzle, enum SudokuValues_E val, unsigned int row, unsigned int col);


















int Sudoku_Solve(SudokuPuzzle_P p);




/*
 * Set cell value
 */
//int Sudoku_SetCell(SudokuPuzzle_T *p, int row, int col, int val);








int Sudoku_PrintPuzzle(SudokuPuzzle_P p);




int Sudoku_RemoveCandidate(SudokuCell_P cell, int cand);






#ifdef __cplusplus
}
#endif

#endif // SUDOKU_H_INCLUDED

#include "sudoku.h"
#include "sudoku_engine.h"

/* Get a Sudoku Malloc Instance */
SudokuPuzzle_P Sudoku_MallocPuzzle(void)
{
    printf("alloc: %d\n\r", sizeof(struct SudokuPuzzle_S));
    return (SudokuPuzzle_P)malloc(sizeof(struct SudokuPuzzle_S));
}

void Sudoku_FreePuzzle(SudokuPuzzle_P puzzle)
{
    free(puzzle);
}

SudokuPuzzle_P Sudoku_CopyPuzzle(SudokuPuzzle_P destination, SudokuPuzzle_P source)
{
    return (SudokuPuzzle_P)memmove(destination, source, sizeof(struct SudokuPuzzle_S));
}

int Sudoku_InitializePuzzle(SudokuPuzzle_P p)
{
    return (int)InitializePuzzle(p);
}

int Sudoku_SetValue(SudokuPuzzle_P p, unsigned int row, unsigned int col, int val)
{
    return (int)SetCellValueInPuzzle(p, (enum SudokuValues_E)val, row, col);
}

int Sudoku_GetValue(SudokuPuzzle_P p, unsigned int row, unsigned int col)
{
    return (int)GetCellValueInPuzzle(p, row, col);
}

int Sudoku_GetNumCandidates(SudokuPuzzle_P p, unsigned int row, unsigned int col)
{
    return GetCandidatesInCell(p, row, col);
}

int Sudoku_Solve(SudokuPuzzle_P p)
{
    return Solve(p, 0);
}

int Sudoku_PrintPuzzle(SudokuPuzzle_P p)
{
    int ret = 0;
    if(NULL == p) return -1;

    int i;

    for(i = 0; i<NUM_ROWS; i++)
    {
        if(!(i%3)) printf("-------------------\n");
        printf("|%d %d %d|%d %d %d|%d %d %d|\n",
               Sudoku_GetValue(p,i,0),
               Sudoku_GetValue(p,i,1),
               Sudoku_GetValue(p,i,2),
               Sudoku_GetValue(p,i,3),
               Sudoku_GetValue(p,i,4),
               Sudoku_GetValue(p,i,5),
               Sudoku_GetValue(p,i,6),
               Sudoku_GetValue(p,i,7),
               Sudoku_GetValue(p,i,8));

    }
    printf("-------------------\n");

    for(i = 0; i<NUM_ROWS; i++)
    {
        if(!(i%3)) printf("-------------------\n");
        printf("|%d %d %d|%d %d %d|%d %d %d|\n",
               Sudoku_GetNumCandidates(p,i,0),
               Sudoku_GetNumCandidates(p,i,1),
               Sudoku_GetNumCandidates(p,i,2),
               Sudoku_GetNumCandidates(p,i,3),
               Sudoku_GetNumCandidates(p,i,4),
               Sudoku_GetNumCandidates(p,i,5),
               Sudoku_GetNumCandidates(p,i,6),
               Sudoku_GetNumCandidates(p,i,7),
               Sudoku_GetNumCandidates(p,i,8));

    }
    printf("-------------------\n");

    return ret;
}
#include "sudoku.h"
#include "sudoku_engine.h"

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/* Get a Sudoku Malloc Instance */
SudokuPuzzle_P Sudoku_MallocPuzzle(void)
{
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

Sudoku_RC_T Sudoku_InitializePuzzle(SudokuPuzzle_P p)
{
    return InitializePuzzle(p);
}


Sudoku_Values_T Sudoku_SetValue(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, unsigned int val)
{
    return SetValue(p, (Sudoku_Values_T)val, row, col);
}

unsigned int Sudoku_GetValue(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col)
{
    return (unsigned int)GetValue(p, row, col);
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

void Sudoku_InitializeFromArray(SudokuPuzzle_P p, const char *sudoku_array)
{

    for (unsigned int row = 0; row < NUM_ROWS; row++)
    {
        for (unsigned int col = 0; col < NUM_COLS; col++)
        {
            char char_value[2];

            strncpy_s(char_value, sizeof(char_value), sudoku_array + row * 9 + col, 1);

            if (isdigit(char_value[0]))
            {
                int val = atoi(char_value);
                if(val != 0) 
                {
                    Sudoku_SetValue(p, row, col, val);
                }
            }
        }
    }
}
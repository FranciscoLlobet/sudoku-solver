#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest/doctest.h"

#include "sudoku.h"
#include "sudoku_engine.h"

const char * sudokuTestArrays[] = {
    "974236158638591742125487936316754289742918563589362417867125394253649871491873625",
    "2564891733746159829817234565932748617128.6549468591327635147298127958634849362715",
    "3.542.81.4879.15.6.29.5637485.793.416132.8957.74.6528.2413.9.655.867.192.965124.8",
    "..2.3...8.....8....31.2.....6..5.27..1.....5.2.4.6..31....8.6.5.......13..531.4.."
};

void TEST_InitializeTestPuzzle(SudokuPuzzle_P puzzle)
{
    Sudoku_SetValue(puzzle, 0, 1, 2);
    Sudoku_SetValue(puzzle, 0, 7, 5);

    Sudoku_SetValue(puzzle, 1, 0, 3);
    Sudoku_SetValue(puzzle, 1, 3, 5);
    Sudoku_SetValue(puzzle, 1, 8, 4);

    Sudoku_SetValue(puzzle, 2, 1, 5);
    Sudoku_SetValue(puzzle, 2, 3, 9);
    Sudoku_SetValue(puzzle, 2, 4, 7);

    Sudoku_SetValue(puzzle, 3, 1, 4);
    Sudoku_SetValue(puzzle, 3, 4, 9);

    Sudoku_SetValue(puzzle, 4, 1, 9);
    Sudoku_SetValue(puzzle, 4, 2, 6);
    Sudoku_SetValue(puzzle, 4, 6, 4);
    Sudoku_SetValue(puzzle, 4, 7, 2);

    Sudoku_SetValue(puzzle, 5, 4, 3);
    Sudoku_SetValue(puzzle, 5, 7, 8);

    Sudoku_SetValue(puzzle, 6, 4, 2);
    Sudoku_SetValue(puzzle, 6, 5, 3);
    Sudoku_SetValue(puzzle, 6, 6, 1);
    Sudoku_SetValue(puzzle, 6, 7, 4);

    Sudoku_SetValue(puzzle, 7, 0, 6);
    Sudoku_SetValue(puzzle, 7, 5, 7);
    Sudoku_SetValue(puzzle, 7, 8, 2);

    Sudoku_SetValue(puzzle, 8, 1, 8);
    Sudoku_SetValue(puzzle, 8, 7, 7);
}

void TEST_InitializeSudokuFromArray(SudokuPuzzle_P p, const char * sudoku_array)
{

    for (unsigned int row = 0; row < NUM_ROWS; row++)
    {
        for (unsigned int col = 0; col < NUM_COLS; col++)
        {
            char char_value[2];

            strncpy(char_value, sudoku_array + row*9 + col, 1);

            if (isdigit(char_value[0]))
            {
                int val = atoi(char_value);
                Sudoku_SetValue(p, row, col, val);
            }
        }
    }
}

TEST_CASE("Test Initialization")
{
    int rc = 0;
    SudokuPuzzle_P p = Sudoku_MallocPuzzle();

    CHECK((SudokuPuzzle_P)NULL != p);

    Sudoku_InitializePuzzle(p);

    TEST_InitializeTestPuzzle(p);

    rc = Sudoku_PrintPuzzle(p);

    CHECK((int)0 == rc);

    Sudoku_FreePuzzle(p);
}

TEST_CASE("Test Solve NULL")
{
    int rc = 0;

    rc = Sudoku_Solve((SudokuPuzzle_P)NULL);

    CHECK(-1 == rc);
}

TEST_CASE("Test Completed Solution")
{
    int rc = 0;

    SudokuPuzzle_P p = Sudoku_MallocPuzzle();

    CHECK((SudokuPuzzle_P)NULL != p);

    Sudoku_InitializePuzzle(p);

    TEST_InitializeSudokuFromArray(p, sudokuTestArrays[0]);

    rc = Sudoku_Solve((SudokuPuzzle_P)p);

    //Sudoku_PrintPuzzle(p);
    CHECK((int)0 == rc);

    Sudoku_FreePuzzle(p);
}

TEST_CASE("Last Empty Square")
{
    int rc = 0;

    SudokuPuzzle_P p = Sudoku_MallocPuzzle();

    CHECK((SudokuPuzzle_P)NULL != p);

    Sudoku_InitializePuzzle(p);

    TEST_InitializeSudokuFromArray(p, sudokuTestArrays[1]);

    rc = Sudoku_Solve((SudokuPuzzle_P)p);

    Sudoku_PrintPuzzle(p);
    CHECK((int)0 == rc);

    Sudoku_FreePuzzle(p);
}

TEST_CASE("Naked Singles")
{
    int rc = 0;

    SudokuPuzzle_P p = Sudoku_MallocPuzzle();

    CHECK((SudokuPuzzle_P)NULL != p);

    Sudoku_InitializePuzzle(p);

    TEST_InitializeSudokuFromArray(p, sudokuTestArrays[2]);

    rc = Sudoku_Solve((SudokuPuzzle_P)p);

    Sudoku_PrintPuzzle(p);
    CHECK((int)0 == rc);

    Sudoku_FreePuzzle(p);
}

TEST_CASE("Hidden Singles")
{
    int rc = 0;

    SudokuPuzzle_P p = Sudoku_MallocPuzzle();

    CHECK((SudokuPuzzle_P)NULL != p);

    Sudoku_InitializePuzzle(p);

    TEST_InitializeSudokuFromArray(p, sudokuTestArrays[3]);

    rc = Sudoku_Solve((SudokuPuzzle_P)p);

    Sudoku_PrintPuzzle(p);
    CHECK((int)0 == rc);

    Sudoku_FreePuzzle(p);
}
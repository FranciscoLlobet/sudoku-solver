#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest/doctest.h"

#include "sudoku.h"
#include "sudoku_engine.h"

const char * sudokuTestArrays[] = {
    "974236158638591742125487936316754289742918563589362417867125394253649871491873625", /* Array 0 */
    "2564891733746159829817234565932748617128.6549468591327635147298127958634849362715", /* Array 1 */
    "3.542.81.4879.15.6.29.5637485.793.416132.8957.74.6528.2413.9.655.867.192.965124.8", /* Array 2 */
    "..2.3...8.....8....31.2.....6..5.27..1.....5.2.4.6..31....8.6.5.......13..531.4..", /* Array 3*/
    "11...............................................................................", /* Array 4 */
    "1........1.......................................................................", /* Array 5 */
    "1.........1.........1............................................................", /* Array 6 */
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

    TEST_InitializeTestPuzzle(p);

    Sudoku_InitializeFromArray(p, sudokuTestArrays[0]);

    rc = Sudoku_Solve((SudokuPuzzle_P)p);

    CHECK((int)0 == rc);

    Sudoku_FreePuzzle(p);
}

TEST_CASE("Last Empty Square")
{
    int rc = 0;

    SudokuPuzzle_P p = Sudoku_MallocPuzzle();

    CHECK((SudokuPuzzle_P)NULL != p);

    Sudoku_InitializePuzzle(p);

    Sudoku_InitializeFromArray(p, sudokuTestArrays[1]);

    rc = Sudoku_Solve((SudokuPuzzle_P)p);

    CHECK((int)0 == rc);

    Sudoku_FreePuzzle(p);
}

TEST_CASE("Naked Singles")
{
    int rc = 0;

    SudokuPuzzle_P p = Sudoku_MallocPuzzle();

    CHECK((SudokuPuzzle_P)NULL != p);

    Sudoku_InitializePuzzle(p);

    Sudoku_InitializeFromArray(p, sudokuTestArrays[2]);

    rc = Sudoku_Solve((SudokuPuzzle_P)p);

    CHECK((int)0 == rc);

    Sudoku_FreePuzzle(p);
}

TEST_CASE("Hidden Singles")
{
    int rc = 0;

    SudokuPuzzle_P p = Sudoku_MallocPuzzle();

    CHECK((SudokuPuzzle_P)NULL != p);

    Sudoku_InitializePuzzle(p);

    Sudoku_InitializeFromArray(p, sudokuTestArrays[3]);

    rc = Sudoku_Solve((SudokuPuzzle_P)p);

    CHECK((int)0 == rc);

    Sudoku_FreePuzzle(p);
}

TEST_CASE("Row Conflict")
{
    int rc = 0;

    SudokuPuzzle_P p = Sudoku_MallocPuzzle();

    CHECK((SudokuPuzzle_P)NULL != p);

    Sudoku_InitializePuzzle(p);

    Sudoku_InitializeFromArray(p, sudokuTestArrays[4]);

    rc = Sudoku_Solve((SudokuPuzzle_P)p);

    CHECK((int)SUDOKU_RC_ERROR == rc);

    Sudoku_FreePuzzle(p);
}

TEST_CASE("Column Conflict")
{
    int rc = 0;

    SudokuPuzzle_P p = Sudoku_MallocPuzzle();

    CHECK((SudokuPuzzle_P)NULL != p);

    Sudoku_InitializePuzzle(p);

    Sudoku_InitializeFromArray(p, sudokuTestArrays[5]);

    rc = Sudoku_Solve((SudokuPuzzle_P)p);

    CHECK((int)SUDOKU_RC_ERROR == rc);

    Sudoku_FreePuzzle(p);
}

TEST_CASE("Subrgid Conflict")
{
    int rc = 0;

    SudokuPuzzle_P p = Sudoku_MallocPuzzle();

    CHECK((SudokuPuzzle_P)NULL != p);

    Sudoku_InitializePuzzle(p);

    Sudoku_InitializeFromArray(p, sudokuTestArrays[6]);

    rc = Sudoku_Solve((SudokuPuzzle_P)p);

    CHECK((int)SUDOKU_RC_ERROR == rc);

    Sudoku_FreePuzzle(p);
}

TEST_CASE("Row Prune")
{
    //Sudoku_RC_E rc = SUDOKU_RC_SUCCESS;
    int rc = 0;
    struct SudokuPuzzle_S p;
    Sudoku_InitializePuzzle( (SudokuPuzzle_P)&p);
    Sudoku_InitializeFromArray((SudokuPuzzle_P)&p, sudokuTestArrays[4]);

    rc = pruneCandidates(&p);

    CHECK(-1 == rc);
}

TEST_CASE("Prune Isses")
{
    Sudoku_RC_E rc = SUDOKU_RC_ERROR;

    struct SudokuPuzzle_S p;
    Sudoku_InitializePuzzle( (SudokuPuzzle_P)&p);

    Sudoku_InitializeFromArray((SudokuPuzzle_P)&p, sudokuTestArrays[6]);

    rc = PrunePuzzle((SudokuPuzzle_P)&p);

    CHECK(SUDOKU_RC_ERROR == rc);
}
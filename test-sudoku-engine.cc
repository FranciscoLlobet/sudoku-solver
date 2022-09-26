
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest/doctest.h"

#include "test-sudoku.hh"

#include "sudoku_engine.h"

extern "C"
{
#include "sudoku_engine.c"
}

TEST_CASE("Correct Row. No Candidates left")
{
    struct SudokuPuzzle_S p;

    Sudoku_InitializePuzzle(&p);

    Sudoku_SetValue(&p, 0, 0, 1);
    Sudoku_SetValue(&p, 0, 1, 2);
    Sudoku_SetValue(&p, 0, 2, 3);
    Sudoku_SetValue(&p, 0, 3, 4);
    Sudoku_SetValue(&p, 0, 4, 5);
    Sudoku_SetValue(&p, 0, 5, 6);
    Sudoku_SetValue(&p, 0, 6, 7);
    Sudoku_SetValue(&p, 0, 7, 8);
    Sudoku_SetValue(&p, 0, 8, 9);

    enum Sudoku_RC_E rc = generateRowMask(&p, 0);

    CHECK(SUDOKU_RC_PRUNE == rc);

    uint32_t mask = getCellMask(getRowReference(&p, 0, 0));

    CHECK(mask == 0);

    rc = generateRowMask(&p, 0);

    CHECK(SUDOKU_RC_SUCCESS == rc);

    CHECK(mask == 0);
}

TEST_CASE("Correct Row. One Candidates left")
{
    struct SudokuPuzzle_S p;

    Sudoku_InitializePuzzle(&p);

    Sudoku_SetValue(&p, 0, 0, 1);
    Sudoku_SetValue(&p, 0, 1, 2);
    Sudoku_SetValue(&p, 0, 2, 3);
    Sudoku_SetValue(&p, 0, 3, 4);
    Sudoku_SetValue(&p, 0, 5, 6);
    Sudoku_SetValue(&p, 0, 6, 7);
    Sudoku_SetValue(&p, 0, 7, 8);
    Sudoku_SetValue(&p, 0, 8, 9);

    enum Sudoku_RC_E rc = generateRowMask(&p, 0);

    CHECK(SUDOKU_RC_PRUNE == rc);

    uint32_t mask = getCellMask(getRowReference(&p, 0, 0));

    CHECK(mask == SUDOKU_MASK_5);

    rc = generateRowMask(&p, 0);

    CHECK(SUDOKU_RC_SUCCESS == rc);

    CHECK(mask == SUDOKU_MASK_5);
}

TEST_CASE("Row Conflict")
{
    struct SudokuPuzzle_S p;

    Sudoku_InitializePuzzle(&p);

    Sudoku_SetValue(&p, 0, 0, 1);
    Sudoku_SetValue(&p, 0, 1, 1);

    enum Sudoku_RC_E rc = generateRowMask(&p, 0);

    CHECK(SUDOKU_RC_ERROR == rc);

    uint32_t mask = getCellMask(getRowReference(&p, 0, 0));

    CHECK(mask == 0);
}

TEST_CASE("Column Conflict")
{
    struct SudokuPuzzle_S p;

    const unsigned int col = 0;
    Sudoku_InitializePuzzle(&p);

    Sudoku_SetValue(&p, 0, col, 1);
    Sudoku_SetValue(&p, 1, col, 1);

    enum Sudoku_RC_E rc = generateColumnMask(&p, col);

    CHECK(SUDOKU_RC_ERROR == rc);

    uint32_t mask = getCellMask(getColumnReference(&p, 0, col));

    CHECK(mask == 0);
}

TEST_CASE("Correct Column. Now Candidates left")
{
    struct SudokuPuzzle_S p;

    Sudoku_InitializePuzzle(&p);
    const unsigned int col = 0;

    Sudoku_SetValue(&p, 0, col, 1);
    Sudoku_SetValue(&p, 1, col, 2);
    Sudoku_SetValue(&p, 2, col, 3);
    Sudoku_SetValue(&p, 3, col, 4);
    Sudoku_SetValue(&p, 4, col, 5);
    Sudoku_SetValue(&p, 5, col, 6);
    Sudoku_SetValue(&p, 6, col, 7);
    Sudoku_SetValue(&p, 7, col, 8);
    Sudoku_SetValue(&p, 8, col, 9);

    enum Sudoku_RC_E rc = generateColumnMask(&p, col);

    CHECK(SUDOKU_RC_PRUNE == rc);

    uint32_t mask = getCellMask(getColumnReference(&p, 0, col));

    CHECK(mask == 0);

    rc = generateColumnMask(&p, col);

    CHECK(SUDOKU_RC_SUCCESS == rc);

    CHECK(mask == 0);
}

TEST_CASE("Correct Subgrid. No Candidates left")
{
    struct SudokuPuzzle_S p;

    Sudoku_InitializePuzzle(&p);
    const unsigned int sub_col = 0;
    const unsigned int sub_row = 0;

    Sudoku_SetValue(&p, 0, 0, 1);
    Sudoku_SetValue(&p, 1, 0, 2);
    Sudoku_SetValue(&p, 2, 0, 3);
    Sudoku_SetValue(&p, 0, 1, 4);
    Sudoku_SetValue(&p, 1, 1, 5);
    Sudoku_SetValue(&p, 2, 1, 6);
    Sudoku_SetValue(&p, 0, 2, 7);
    Sudoku_SetValue(&p, 1, 2, 8);
    Sudoku_SetValue(&p, 2, 2, 9);

    enum Sudoku_RC_E rc = generateSubCellMask(&p, sub_row, sub_col);

    CHECK(SUDOKU_RC_PRUNE == rc);

    uint32_t mask = getCellMask(getSubgridReference(&p, sub_row, sub_col));

    CHECK(mask == 0);

    rc = generateSubCellMask(&p, sub_row, sub_col);

    CHECK(SUDOKU_RC_SUCCESS == rc);

    CHECK(mask == 0);
}

TEST_CASE("Correct Subgrid. One Candidates left")
{
    struct SudokuPuzzle_S p;

    Sudoku_InitializePuzzle(&p);
    const unsigned int sub_col = 0;
    const unsigned int sub_row = 0;

    Sudoku_SetValue(&p, 0, 0, 1);
    Sudoku_SetValue(&p, 1, 0, 2);
    Sudoku_SetValue(&p, 2, 0, 3);
    Sudoku_SetValue(&p, 0, 1, 4);
    Sudoku_SetValue(&p, 2, 1, 6);
    Sudoku_SetValue(&p, 0, 2, 7);
    Sudoku_SetValue(&p, 1, 2, 8);
    Sudoku_SetValue(&p, 2, 2, 9);

    enum Sudoku_RC_E rc = generateSubCellMask(&p, sub_row, sub_col);

    CHECK(SUDOKU_RC_PRUNE == rc);

    uint32_t mask = getCellMask(getSubgridReference(&p, sub_row, sub_col));

    CHECK(mask == SUDOKU_MASK_5);

    rc = generateSubCellMask(&p, sub_row, sub_col);

    CHECK(SUDOKU_RC_SUCCESS == rc);

    CHECK(mask == SUDOKU_MASK_5);
}

TEST_CASE("Subgrid Error")
{
    struct SudokuPuzzle_S p;

    Sudoku_InitializePuzzle(&p);
    const unsigned int sub_col = 0;
    const unsigned int sub_row = 0;

    Sudoku_SetValue(&p, 0, 0, 1);
    Sudoku_SetValue(&p, 1, 0, 1);
    Sudoku_SetValue(&p, 2, 0, 1);
    Sudoku_SetValue(&p, 0, 1, 1);
    Sudoku_SetValue(&p, 2, 1, 1);
    Sudoku_SetValue(&p, 0, 2, 1);
    Sudoku_SetValue(&p, 1, 2, 1);
    Sudoku_SetValue(&p, 2, 2, 1);

    enum Sudoku_RC_E rc = generateSubCellMask(&p, sub_row, sub_col);

    CHECK(SUDOKU_RC_ERROR == rc);

    uint32_t mask = getCellMask(getSubgridReference(&p, sub_row, sub_col));

    CHECK(mask == 0);
}

TEST_CASE("Test Pruning Valid Cases")
{
    struct SudokuPuzzle_S p;
 
    SUBCASE("Already Solved Puzzle")
    {
        Sudoku_InitializeFromArray(&p, validTestPuzzles[0].c_str());
        CHECK(SUDOKU_RC_SUCCESS == PrunePuzzle(&p));
    }

    SUBCASE("Almost Solved Puzzle")
    {
        Sudoku_InitializeFromArray(&p,validTestPuzzles[1].c_str());
        CHECK(SUDOKU_RC_SUCCESS == PrunePuzzle(&p));
    }

    SUBCASE("Test Naked Singles")
    {
        Sudoku_InitializeFromArray(&p,validTestPuzzles[2].c_str());
        CHECK(SUDOKU_RC_SUCCESS == PrunePuzzle(&p));
    }

    SUBCASE("Test Hidden Singles")
    {
        Sudoku_InitializeFromArray(&p,validTestPuzzles[3].c_str());
        CHECK(SUDOKU_RC_PRUNE == PrunePuzzle(&p));
    }
}

TEST_CASE("Test Pruning Invalid Cases")
{
    struct SudokuPuzzle_S p;
 
    for(auto x : invalidTestPuzzles)
    {
        Sudoku_InitializeFromArray(&p, x.c_str());
        CHECK(SUDOKU_RC_ERROR == PrunePuzzle(&p));
    }
}

TEST_CASE("Test Valid Puzzles")
{
    struct SudokuPuzzle_S p;

    for(auto x: validTestPuzzles)
    {
        Sudoku_InitializeFromArray(&p, x.c_str());
        CHECK(SUDOKU_RC_SUCCESS == Solve(&p, 0));
    }
}

TEST_CASE("Test Invalid Puzzles")
{
    struct SudokuPuzzle_S p;

    for(auto x: invalidTestPuzzles)
    {
        Sudoku_InitializeFromArray(&p, x.c_str());
        CHECK(SUDOKU_RC_ERROR == Solve(&p, 0));
    }
}




#if 0
TEST_CASE("Test validate Grid")
{
    struct SudokuPuzzle_S p;
    enum Sudoku_RC_E rc;

    SUBCASE("Grid is completed")
    {
        Sudoku_InitializePuzzle(&p);

        Sudoku_InitializeFromArray(&p, sudokuTestArrays[0]); /* Valid Sudoku Puzzle*/

        rc = validateGrid(&p);

        CHECK(rc == SUDOKU_RC_SUCCESS);
    }
    SUBCASE("Grid is not complete")
    {
        Sudoku_InitializePuzzle(&p);

        Sudoku_InitializeFromArray(&p, sudokuTestArrays[1]); /* Valid Sudoku Puzzle*/

        rc = validateGrid(&p);

        CHECK(rc == SUDOKU_RC_PRUNE);
    }
    SUBCASE("Grid is not complete 2")
    {
        Sudoku_InitializePuzzle(&p);

        Sudoku_InitializeFromArray(&p, sudokuTestArrays[2]); /* Valid Sudoku Puzzle*/

        rc = validateGrid(&p);

        CHECK(rc == SUDOKU_RC_PRUNE);
    }
    SUBCASE("Grid is not complete 3")
    {
        Sudoku_InitializePuzzle(&p);

        Sudoku_InitializeFromArray(&p, sudokuTestArrays[3]); /* Valid Sudoku Puzzle*/

        rc = validateGrid(&p);

        CHECK(rc == SUDOKU_RC_PRUNE);
    }
}
#endif
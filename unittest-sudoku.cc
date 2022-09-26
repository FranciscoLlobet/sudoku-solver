#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest/doctest.h"

/* Include test vectors */
#include "test-sudoku.hh"

#include "sudoku.c"

TEST_CASE("Initialize Puzzle")
{
    struct SudokuPuzzle_S p;

    SUBCASE("NULL Pointer test")
    {
        CHECK(SUDOKU_RC_ERROR == Sudoku_InitializePuzzle((SudokuPuzzle_P)NULL));
    }

    SUBCASE("Initialize Puzzle")
    {
        CHECK(SUDOKU_RC_SUCCESS == Sudoku_InitializePuzzle(&p));
    }
}

TEST_CASE("Set Value")
{
    struct SudokuPuzzle_S p;

    (void)Sudoku_InitializePuzzle(&p);

    SUBCASE("NULL Pointer Test")
    {
        CHECK(SUDOKU_RC_ERROR == Sudoku_SetValue((SudokuPuzzle_P)NULL, 0, 0, 1));
    }
    SUBCASE("Error Invalid Value")
    {
        CHECK(SUDOKU_RC_ERROR == Sudoku_SetValue(&p, 0, 0, 10));
    }
    SUBCASE("Error Valid Values 1..9")
    {
        for (unsigned int val = 1; val <= 9; val++)
        {
            CHECK(SUDOKU_RC_SUCCESS == Sudoku_SetValue(&p, 0, 0, val));
            CHECK(p.grid[0][0].value == (1 << (val - 1)));
        }
    }
    SUBCASE("Error Valid 0")
    {
        CHECK(SUDOKU_RC_SUCCESS == Sudoku_SetValue(&p, 0, 0, 0));
    }
}

TEST_CASE("Get Value")
{
    struct SudokuPuzzle_S p;
    const int test_value = 5;

    (void)Sudoku_InitializePuzzle(&p);

    SUBCASE("NULL Pointer Test")
    {
        CHECK(SUDOKU_RC_ERROR == Sudoku_GetValue((SudokuPuzzle_P)NULL, 0, 0));
    }
    SUBCASE("Set-Get Value")
    {
        CHECK(SUDOKU_RC_SUCCESS == Sudoku_SetValue(&p, 0, 0, test_value));
        CHECK(test_value == Sudoku_GetValue(&p, 0, 0));
    }
    SUBCASE("Set-Get Values")
    {
        for (int test_value = 1; test_value <= 9; test_value++)
        {
            for (Sudoku_Row_Index_T row = 0; row < NUM_ROWS; row++)
            {
                for (Sudoku_Column_Index_T col = 0; col < NUM_COLS; col++)
                {
                    CHECK(SUDOKU_RC_SUCCESS == Sudoku_SetValue(&p, row, col, test_value));
                    CHECK(test_value == Sudoku_GetValue(&p, row, col));
                }
            }
        }
    }
}

TEST_CASE("Test Initialize from Array")
{
    struct SudokuPuzzle_S p;
    char test_array[] = "Test Array";

    CHECK(SUDOKU_RC_ERROR == Sudoku_InitializeFromArray(NULL, test_array));

    CHECK(SUDOKU_RC_ERROR == Sudoku_InitializeFromArray(&p, NULL));
}

TEST_CASE("Check Rows")
{
    struct SudokuPuzzle_S p;

    SUBCASE("Check Invalid Rows")
    {
        CHECK(SUDOKU_RC_SUCCESS == Sudoku_InitializeFromArray(&p, invalidTestPuzzles[0].c_str()));
        CHECK(SUDOKU_RC_ERROR == checkRows(&p));
    }
    SUBCASE("Check Valid Rows")
    {
        for (auto x : validTestPuzzles)
        {
            CHECK(SUDOKU_RC_SUCCESS == Sudoku_InitializeFromArray(&p, x.c_str()));
            CHECK(SUDOKU_RC_SUCCESS == checkRows(&p));
        }
    }
}

TEST_CASE("Check Cols")
{
    struct SudokuPuzzle_S p;

    SUBCASE("Check Invalid Cols")
    {
        CHECK(SUDOKU_RC_SUCCESS == Sudoku_InitializeFromArray(&p, invalidTestPuzzles[1].c_str()));
        CHECK(SUDOKU_RC_ERROR == checkCols(&p));
    }
    SUBCASE("Check Valid Cols")
    {
        for (auto x : validTestPuzzles)
        {
            CHECK(SUDOKU_RC_SUCCESS == Sudoku_InitializeFromArray(&p, x.c_str()));
            CHECK(SUDOKU_RC_SUCCESS == checkCols(&p));
        }
    }
}

TEST_CASE("Check Subgrid")
{
    struct SudokuPuzzle_S p;

    SUBCASE("Check Invalid Subgrid")
    {
        CHECK(SUDOKU_RC_SUCCESS == Sudoku_InitializeFromArray(&p, invalidTestPuzzles[2].c_str()));
        CHECK(SUDOKU_RC_ERROR == checkSubgrid(&p));
    }
    SUBCASE("Check Valid Subgrid")
    {
        for (auto x : validTestPuzzles)
        {
            CHECK(SUDOKU_RC_SUCCESS == Sudoku_InitializeFromArray(&p, x.c_str()));
            CHECK(SUDOKU_RC_SUCCESS == checkSubgrid(&p));
        }
    }
}

TEST_CASE("Check Puzzle")
{
    struct SudokuPuzzle_S p;

    SUBCASE("Check Solved Puzzle")
    {
        CHECK(SUDOKU_RC_SUCCESS == Sudoku_InitializeFromArray(&p, validTestPuzzles[0].c_str()));
        CHECK(SUDOKU_RC_SUCCESS == Sudoku_Check(&p));
    }
    SUBCASE("Check Almost Solved Puzzle")
    {
        CHECK(SUDOKU_RC_SUCCESS == Sudoku_InitializeFromArray(&p, validTestPuzzles[1].c_str()));
        CHECK(SUDOKU_RC_PRUNE == Sudoku_Check(&p));
    }
    SUBCASE("Check Naked Singles")
    {
        CHECK(SUDOKU_RC_SUCCESS == Sudoku_InitializeFromArray(&p, validTestPuzzles[2].c_str()));
        CHECK(SUDOKU_RC_PRUNE == Sudoku_Check(&p));
    }
    SUBCASE("Check Hidden Singles")
    {
        CHECK(SUDOKU_RC_SUCCESS == Sudoku_InitializeFromArray(&p, validTestPuzzles[2].c_str()));
        CHECK(SUDOKU_RC_PRUNE == Sudoku_Check(&p));
    }
    SUBCASE("Check Invalid Cases")
    {
        for (auto x : invalidTestPuzzles)
        {
            CHECK(SUDOKU_RC_SUCCESS == Sudoku_InitializeFromArray(&p, x.c_str()));
            CHECK(SUDOKU_RC_ERROR == Sudoku_Check(&p));
        }
    }
}

TEST_CASE("Generate Row Mask")
{
    struct SudokuPuzzle_S p;

    (void)Sudoku_InitializePuzzle(&p);

    CHECK((uint32_t)SUDOKU_MASK_ALL == p.row_candidates[0]);

    CHECK(0 == generateRowMask(&p, 0));

    CHECK((uint32_t)SUDOKU_MASK_ALL == p.row_candidates[0]);

    for (int i = 0; i < NUM_COLS; i++)
    {
        (void)Sudoku_SetValue(&p, 0, i, i + 1);
    }

    CHECK(1 == generateRowMask(&p, 0));

    CHECK((uint32_t)SUDOKU_MASK_NONE == p.row_candidates[0]);
}

TEST_CASE("Generate Row Masks in Puzzle")
{
    struct SudokuPuzzle_S p;

    (void)Sudoku_InitializePuzzle(&p);

    for (Sudoku_Row_Index_T row = 0; row < NUM_ROWS; row++)
    {
        for (int i = 0; i < NUM_COLS; i++)
        {
            (void)Sudoku_SetValue(&p, row, i, i + 1);
        }
    }

    CHECK(9 == generateRowMasks(&p));

    for (Sudoku_Row_Index_T row = 0; row < NUM_ROWS; row++)
    {
        CHECK((uint32_t)SUDOKU_MASK_NONE == p.row_candidates[row]);
    }
}

TEST_CASE("Generate Column Mask")
{
    struct SudokuPuzzle_S p;

    (void)Sudoku_InitializePuzzle(&p);

    CHECK((uint32_t)SUDOKU_MASK_ALL == p.col_candidates[0]);

    CHECK(0 == generateColumnMask(&p, 0));

    CHECK((uint32_t)SUDOKU_MASK_ALL == p.col_candidates[0]);

    for (int i = 0; i < NUM_ROWS; i++)
    {
        (void)Sudoku_SetValue(&p, i, 0, i + 1);
    }

    CHECK(1 == generateColumnMask(&p, 0));

    CHECK((uint32_t)SUDOKU_MASK_NONE == p.col_candidates[0]);
}

TEST_CASE("Generate Column Masks in Puzzle")
{
    struct SudokuPuzzle_S p;

    (void)Sudoku_InitializePuzzle(&p);

    for (Sudoku_Column_Index_T col = 0; col < NUM_COLS; col++)
    {
        for (int i = 0; i < NUM_ROWS; i++)
        {
            (void)Sudoku_SetValue(&p, i, col, i + 1);
        }
    }

    CHECK(9 == generateColumnMasks(&p));

    for (Sudoku_Row_Index_T col = 0; col < NUM_COLS; col++)
    {
        CHECK((uint32_t)SUDOKU_MASK_NONE == p.col_candidates[col]);
    }
}
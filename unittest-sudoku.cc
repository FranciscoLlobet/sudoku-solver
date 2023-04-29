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
        CHECK(SUDOKU_RC_NULL_POINTER == Sudoku_SetValue((SudokuPuzzle_P)NULL, 0, 0, 1));
    }
    SUBCASE("Error Invalid Value")
    {
        CHECK(SUDOKU_RC_INVALID_VALUE == Sudoku_SetValue(&p, 0, 0, 10));
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
        CHECK(SUDOKU_RC_NULL_POINTER == Sudoku_GetValue((SudokuPuzzle_P)NULL, 0, 0));
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

    CHECK(SUDOKU_RC_NULL_POINTER == Sudoku_InitializeFromArray(&p, NULL));
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
        CHECK(SUDOKU_RC_SUCCESS == Sudoku_InitializeFromArray(&p, validTestPuzzles[3].c_str()));
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

TEST_CASE("Generate Subgrid Mask")
{
    struct SudokuPuzzle_S p;
    SUBCASE("Work with initialized puzzle")
    {
        Sudoku_InitializePuzzle(&p);

        generateSubGridMask(&p, 0, 0);
        CHECK(SUDOKU_MASK_ALL == p.sub_candidates[0][0]);
    }
    SUBCASE("Generate Subgrid mask")
    {
        (void)Sudoku_InitializeFromArray(&p, subgridTest[0].c_str());

        generateSubGridMask(&p, 0, 0);

        CHECK(SUDOKU_MASK_NONE == p.sub_candidates[0][0]); // No candidates left
    }
    SUBCASE("Generate incomplete mask")
    {
        (void)Sudoku_InitializeFromArray(&p, subgridTest[1].c_str());

        generateSubGridMask(&p, 0, 0);

        CHECK(SUDOKU_MASK_4 == p.sub_candidates[0][0]); // One Candidate Left
    }
}

TEST_CASE("Generate Subgrid Masks in puzzle")
{
    struct SudokuPuzzle_S p;

    SUBCASE("Test empty grids")
    {
        Sudoku_InitializePuzzle(&p);
        generateSubgridMasks(&p);

        for (Sudoku_Row_Index_T sub_row = 0; sub_row < NUM_SUBGRID_ROWS; sub_row++)
        {
            for (Sudoku_Column_Index_T sub_col = 0; sub_col < NUM_SUBGRID_COLS; sub_col++)
            {
                CHECK(SUDOKU_MASK_ALL == p.sub_candidates[sub_row][sub_col]);
            }
        }
    }
}

TEST_CASE("Prune Cells")
{
    struct SudokuPuzzle_S p;

    (void)Sudoku_InitializePuzzle(&p);

    /* Set all columns except the 5th */
    for (int i = 0; i < NUM_COLS; i++)
    {
        if (i != 4)
            (void)Sudoku_SetValue(&p, 0, i, i + 1);
    }

    CHECK(0 == Sudoku_GetValue(&p, 0, 4));

    // Get row mask
    CHECK(0 != generateColumnMasks(&p));
    CHECK(0 != generateRowMasks(&p));
    CHECK(0 != generateSubgridMasks(&p));

    // Update Cell Mask

    CHECK((SUDOKU_BIT_VALUE_5) == p.row_candidates[0]);
    CHECK((SUDOKU_BIT_VALUE_ALL) == p.col_candidates[4]);

    // Update Cell mask
    CHECK(1 == generateCellMask(&p, 0, 4)); /* Cell only contains one element */

    CHECK(SUDOKU_BIT_VALUE_5 == p.grid[0][4].candidates);
    CHECK(SUDOKU_RC_PRUNE == updateCellCandidates(&p, 0, 4));
    CHECK(SUDOKU_BIT_NO_VALUE == p.grid[0][4].candidates);
    CHECK(5 == Sudoku_GetValue(&p, 0, 4));
}

TEST_CASE("Update puzzle candidates")
{
    struct SudokuPuzzle_S p;

    // Test valid puzzles
    SUBCASE("Already Solved Puzzle")
    {
        CHECK(SUDOKU_RC_SUCCESS == Sudoku_InitializeFromArray(&p, validTestPuzzles[0].c_str()));
        CHECK(SUDOKU_RC_SUCCESS == Sudoku_Check(&p));

        // Test pruning mechanism
        CHECK(0 != generateColumnMasks(&p));
        CHECK(0 != generateRowMasks(&p));
        CHECK(0 != generateSubgridMasks(&p));
        CHECK(0 == generatePuzzleCellMasks(&p));

        // Second running
        CHECK(0 == generateCandidateMasks(&p));
        CHECK(0 == updatePuzzleCandidates(&p));
        CHECK(SUDOKU_RC_SUCCESS == Sudoku_Check(&p));
    }
    SUBCASE("Almost Solved Puzzle")
    {
        CHECK(SUDOKU_RC_SUCCESS == Sudoku_InitializeFromArray(&p, validTestPuzzles[1].c_str()));
        CHECK(SUDOKU_RC_PRUNE == Sudoku_Check(&p));

        // Test pruning mechanism
        CHECK(0 != generateColumnMasks(&p));
        CHECK(0 != generateRowMasks(&p));
        CHECK(0 != generateSubgridMasks(&p));
        CHECK(0 != generatePuzzleCellMasks(&p));
        CHECK(0 == generateCandidateMasks(&p));

        CHECK(1 == updatePuzzleCandidates(&p));
        CHECK(SUDOKU_BIT_NO_VALUE == p.grid[4][4].candidates);
        CHECK(SUDOKU_BIT_VALUE_3 == p.grid[4][4].value);
        CHECK(SUDOKU_RC_SUCCESS == Sudoku_Check(&p));
        
        // Testing another run
        CHECK(0 == generateCandidateMasks(&p));
        CHECK(0 == updatePuzzleCandidates(&p));
    }
    SUBCASE("Check Naked Singles")
    {
        CHECK(SUDOKU_RC_SUCCESS == Sudoku_InitializeFromArray(&p, validTestPuzzles[2].c_str()));
        CHECK(SUDOKU_RC_PRUNE == Sudoku_Check(&p));

        CHECK(0 != generateColumnMasks(&p));
        CHECK(0 != generateRowMasks(&p));
        CHECK(0 != generateSubgridMasks(&p));
        CHECK(0 != generatePuzzleCellMasks(&p));

        CHECK(0 != updatePuzzleCandidates(&p)); // This solves the puzzle
        CHECK(SUDOKU_RC_SUCCESS == Sudoku_Check(&p));

        CHECK(0 != generateColumnMasks(&p));
        CHECK(0 != generateRowMasks(&p));
        CHECK(0 != generateSubgridMasks(&p));
        CHECK(0 == generatePuzzleCellMasks(&p));
        CHECK(0 == updatePuzzleCandidates(&p));
        CHECK(SUDOKU_RC_SUCCESS == Sudoku_Check(&p));


    }
    SUBCASE("Check Hidden Singles")
    {
        CHECK(SUDOKU_RC_SUCCESS == Sudoku_InitializeFromArray(&p, validTestPuzzles[3].c_str()));
        CHECK(SUDOKU_RC_PRUNE == Sudoku_Check(&p));

        CHECK(0 != generateColumnMasks(&p));
        CHECK(0 != generateRowMasks(&p));
        CHECK(0 != generateSubgridMasks(&p));
        CHECK(0 != generatePuzzleCellMasks(&p));

        CHECK(0 == updatePuzzleCandidates(&p)); // First run does not update puzzle candidates
        CHECK(SUDOKU_RC_PRUNE == Sudoku_Check(&p)); // Just one iteration is not enough
    }
        SUBCASE("Check Hidden Singles2")
    {
        CHECK(SUDOKU_RC_SUCCESS == Sudoku_InitializeFromArray(&p, validTestPuzzles[3].c_str()));
        CHECK(SUDOKU_RC_PRUNE == Sudoku_Check(&p));
        
        int changes = 0;
        do{
            changes = generateColumnMasks(&p) + generateRowMasks(&p) + generateSubgridMasks(&p) + generatePuzzleCellMasks(&p) + updatePuzzleCandidates(&p);
        }
        while((changes > 0));
        
        CHECK(SUDOKU_RC_PRUNE == Sudoku_Check(&p)); // Just one iteration is not enough
    }
}

TEST_CASE("Remove Candidate Test")
{
    struct SudokuPuzzle_S p;

    CHECK(SUDOKU_RC_SUCCESS == Sudoku_InitializePuzzle(&p));

    CHECK(SUDOKU_MASK_ALL == p.grid[0][0].candidates);
    CHECK(9 == countCandidatesInCell(&p, 0, 0));

    removeCandidate(&p, 0, 0, SUDOKU_MASK_1);
    
    CHECK((SUDOKU_MASK_ALL & ~SUDOKU_MASK_1) == p.grid[0][0].candidates);
    CHECK(8 == countCandidatesInCell(&p, 0, 0));
}

TEST_CASE("Test Candidate Count strategy")
{
    struct SudokuPuzzle_S p;
    CHECK(SUDOKU_RC_SUCCESS == Sudoku_InitializePuzzle(&p));

    //generateCandidateMasks(&p);

    countCandidatesInPuzzle(&p); // Count the candidates
    countCandidateValues(&p);
    countCandidatesInRows(&p);
    countCandidatesInCols(&p);

    for (size_t idx = 0; idx < NUM_CANDIDATES; idx++)
    {
        CHECK(SUDOKU_MASK_ALL == p.row_candidates[idx]);
        CHECK(SUDOKU_MASK_ALL == p.col_candidates[idx]);
        CHECK(9 == p.n_row_candidates[idx]);
        CHECK(9 == p.n_col_candidates[idx]);
    }
    for(size_t r = 0; r < NUM_ROWS; r++)
    {
        for(size_t c = 0; c < NUM_COLS; c++)
        {
            uint32_t score = p.n_candidates[r][c] + p.n_row_candidates[r] + p.n_col_candidates[c];
            CHECK(9*3 == score);
        }
    }



}


TEST_CASE("Select candidate with lowest amount ")
{
    struct SudokuPuzzle_S p;
    Sudoku_Row_Index_T row=0;
    Sudoku_Column_Index_T col = 0;
    Sudoku_BitValues_T val = SUDOKU_BIT_INVALID_VALUE;

    CHECK(SUDOKU_RC_SUCCESS == Sudoku_InitializePuzzle(&p));
    val = Sudoku_SelectCandidate(&p, &row, &col);
    CHECK((Sudoku_Row_Index_T)0 == row);
    CHECK((Sudoku_Column_Index_T)0 == col);
    CHECK(SUDOKU_BIT_VALUE_1 == val);

    removeCandidate(&p, 4,4, SUDOKU_MASK_1);
    
    val = Sudoku_SelectCandidate(&p, &row, &col);
    
    CHECK((Sudoku_Row_Index_T)4 == row);
    CHECK((Sudoku_Column_Index_T)4 == col);
    CHECK(SUDOKU_BIT_VALUE_2 == val);
}
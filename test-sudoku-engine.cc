
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest/doctest.h"

#include "sudoku_engine.h"

extern "C"
{
#include "sudoku_engine.c"
}

const char *sudokuTestArrays[] = {
    "974236158638591742125487936316754289742918563589362417867125394253649871491873625", /* Array 0 */
    "2564891733746159829817234565932748617128.6549468591327635147298127958634849362715", /* Array 1 */
    "3.542.81.4879.15.6.29.5637485.793.416132.8957.74.6528.2413.9.655.867.192.965124.8", /* Array 2 */
    "..2.3...8.....8....31.2.....6..5.27..1.....5.2.4.6..31....8.6.5.......13..531.4..", /* Array 3*/
    "11...............................................................................", /* Array 4 */
    "1........1.......................................................................", /* Array 5 */
    "1.........1.........1............................................................", /* Array 6 */
};



TEST_CASE("Correct Row. Now Candidates left")
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

TEST_CASE("Correct Subgrid. Now Candidates left")
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

TEST_CASE("Test Pruning Valid Case")
{
    struct SudokuPuzzle_S p;
    enum Sudoku_RC_E rc;

    Sudoku_InitializePuzzle(&p);

    Sudoku_InitializeFromArray(&p, sudokuTestArrays[3]); /* Invalid Sudoku Puzzle*/

    rc = PrunePuzzle(&p);

    CHECK(rc == SUDOKU_RC_SUCCESS);
}

TEST_CASE("Test Pruning Invalid case")
{
    struct SudokuPuzzle_S p;
    enum Sudoku_RC_E rc;

    Sudoku_InitializePuzzle(&p);

    Sudoku_InitializeFromArray(&p, sudokuTestArrays[4]); /* Invalid Sudoku Puzzle*/

    rc = PrunePuzzle(&p);

    CHECK(rc == SUDOKU_RC_ERROR);
}

TEST_CASE("Test Solve Invalid case")
{
    struct SudokuPuzzle_S p;
    enum Sudoku_RC_E rc;

    Sudoku_InitializePuzzle(&p);

    Sudoku_InitializeFromArray(&p, sudokuTestArrays[4]); /* Invalid Sudoku Puzzle*/

    rc = Solve(&p, 0);

    CHECK(rc == SUDOKU_RC_ERROR);
}

TEST_CASE("Test Solve Invalid case 2")
{
    struct SudokuPuzzle_S p;
    enum Sudoku_RC_E rc;

    Sudoku_InitializePuzzle(&p);

    Sudoku_InitializeFromArray(&p, sudokuTestArrays[5]); /* Invalid Sudoku Puzzle*/

    rc = Solve(&p, 0);

    CHECK(rc == SUDOKU_RC_ERROR);
}

TEST_CASE("Test Solve Invalid case 3")
{
    struct SudokuPuzzle_S p;
    enum Sudoku_RC_E rc;

    Sudoku_InitializePuzzle(&p);

    Sudoku_InitializeFromArray(&p, sudokuTestArrays[6]); /* Invalid Sudoku Puzzle*/

    rc = Solve(&p, 0);

    CHECK(rc == SUDOKU_RC_ERROR);
}

TEST_CASE("Test Solve Valid Case")
{
    struct SudokuPuzzle_S p;
    enum Sudoku_RC_E rc;

    SUBCASE("Test Naked Singles")
    {
        Sudoku_InitializePuzzle(&p);

        Sudoku_InitializeFromArray(&p, sudokuTestArrays[2]); /* Valid Sudoku Puzzle*/

        rc = Solve(&p, 0);

        CHECK(rc == SUDOKU_RC_SUCCESS);
    }
    SUBCASE("Test Hidden Singles")
    {
        Sudoku_InitializePuzzle(&p);

        Sudoku_InitializeFromArray(&p, sudokuTestArrays[3]); /* Valid Sudoku Puzzle*/

        rc = Solve(&p, 0);

        CHECK(rc == SUDOKU_RC_SUCCESS);
    }
}

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

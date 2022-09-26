#ifdef __cplusplus
extern "C"
{
#endif

#include "sudoku.h"

#include <ctype.h>

    struct SudokuCell_S
    {
        uint32_t value;      /* Value Mask */
        uint32_t candidates; /* Candidate mask*/
    };

    struct SudokuPuzzle_S
    {
        struct SudokuCell_S grid[NUM_ROWS][NUM_COLS];
        uint32_t row_candidates[NUM_ROWS];
        uint32_t col_candidates[NUM_COLS];
        uint32_t sub_candidates[NUM_SUBGRID_ROWS][NUM_SUBGRID_COLS];
    };

    /* Initialize Puzzle */
    Sudoku_RC_T Sudoku_InitializePuzzle(SudokuPuzzle_P p)
    {
        Sudoku_RC_T ret = SUDOKU_RC_SUCCESS;
        if (NULL == p)
        {
            return SUDOKU_RC_ERROR; // Null pointer
        }

        /* Clean the puzzle */
        (void)memset(p, 0, sizeof(struct SudokuPuzzle_S));

        /* Initialize main grid */
        for (Sudoku_Row_Index_T row = 0; row < (Sudoku_Row_Index_T)NUM_ROWS; row++)
        {
            for (Sudoku_Column_Index_T col = 0; col < (Sudoku_Column_Index_T)NUM_COLS; col++)
            {
                p->grid[row][col].candidates = (uint32_t)SUDOKU_MASK_ALL;
                p->grid[row][col].value = (uint32_t)SUDOKU_BIT_NO_VALUE;
            }
        }

        /* Initialize Row candidates*/
        for (Sudoku_Row_Index_T row = 0; row < (Sudoku_Row_Index_T)NUM_COLS; row++)
        {
            p->row_candidates[row] = (uint32_t)SUDOKU_MASK_ALL;
        }

        /* Initialize Col Candidates */
        for (Sudoku_Column_Index_T col = 0; col < (Sudoku_Column_Index_T)NUM_COLS; col++)
        {
            p->col_candidates[col] = (uint32_t)SUDOKU_MASK_ALL;
        }

        /* Initialize Subgrid Candidates */
        for (Sudoku_Row_Index_T sub_row = 0; sub_row < NUM_SUBGRID_ROWS; sub_row++)
        {
            for (Sudoku_Row_Index_T sub_col = 0; sub_col < NUM_SUBGRID_COLS; sub_col++)
            {
                p->sub_candidates[sub_row][sub_col] = (uint32_t)SUDOKU_MASK_ALL;
            }
        }

        return ret;
    }

    /* Set Value */
    Sudoku_RC_T Sudoku_SetValue(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, int val)
    {
        Sudoku_RC_T ret = SUDOKU_RC_SUCCESS;

        if (NULL == p)
        {
            return SUDOKU_RC_ERROR;
        }
        else if (col >= NUM_COLS)
        {
            return SUDOKU_RC_ERROR;
        }
        else if (row >= NUM_ROWS)
        {
            return SUDOKU_RC_ERROR;
        }

        if (val == 0)
        {
            p->grid[row][col].candidates = (uint32_t)SUDOKU_MASK_ALL; /* No Candidate left*/
            p->grid[row][col].value = (uint32_t)SUDOKU_BIT_NO_VALUE;
        }
        else if (val <= 9)
        {
            p->grid[row][col].candidates = (uint32_t)SUDOKU_MASK_NONE; /* No Candidate left*/
            p->grid[row][col].value = 1 << ((unsigned int)val - 1);
        }
        else
        {
            p->grid[row][col].candidates = (uint32_t)SUDOKU_MASK_NONE;
            p->grid[row][col].value = (uint32_t)SUDOKU_BIT_INVALID_VALUE;
            return SUDOKU_RC_ERROR;
        }
        return ret;
    }

    /* Get Value */
    int Sudoku_GetValue(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col)
    {
        Sudoku_RC_T ret = SUDOKU_RC_SUCCESS;
        unsigned int value = SUDOKU_INVALID_VALUE;

        if (NULL == p)
        {
            return SUDOKU_INVALID_VALUE;
        }
        else if (col >= NUM_COLS)
        {
            return SUDOKU_INVALID_VALUE;
        }
        else if (row >= NUM_ROWS)
        {
            return SUDOKU_INVALID_VALUE;
        }

        switch ((enum SudokuBitValues_E)(p->grid[row][col].value))
        {
        case SUDOKU_BIT_NO_VALUE:
            value = SUDOKU_NO_VALUE;
            break;
        case SUDOKU_BIT_VALUE_1:
            value = SUDOKU_VALUE_1;
            break;
        case SUDOKU_BIT_VALUE_2:
            value = SUDOKU_VALUE_2;
            break;
        case SUDOKU_BIT_VALUE_3:
            value = SUDOKU_VALUE_3;
            break;
        case SUDOKU_BIT_VALUE_4:
            value = SUDOKU_VALUE_4;
            break;
        case SUDOKU_BIT_VALUE_5:
            value = SUDOKU_VALUE_5;
            break;
        case SUDOKU_BIT_VALUE_6:
            value = SUDOKU_VALUE_6;
            break;
        case SUDOKU_BIT_VALUE_7:
            value = SUDOKU_VALUE_7;
            break;
        case SUDOKU_BIT_VALUE_8:
            value = SUDOKU_VALUE_8;
            break;
        case SUDOKU_BIT_VALUE_9:
            value = SUDOKU_VALUE_9;
            break;
        default:
            value = SUDOKU_INVALID_VALUE;
            break;
        }

        return value;
    }

    static Sudoku_RC_T checkRows(SudokuPuzzle_P p)
    {
        Sudoku_RC_T ret = SUDOKU_RC_SUCCESS;

        for (Sudoku_Row_Index_T row = 0; row < NUM_ROWS; row++)
        {
            uint32_t val = SUDOKU_BIT_NO_VALUE;
            for (Sudoku_Column_Index_T col = 0; col < NUM_COLS; col++)
            {
                if ((val & p->grid[row][col].value) != 0)
                {
                    return SUDOKU_RC_ERROR;
                }
                else
                {
                    val |= p->grid[row][col].value;
                }
            }
        }

        return ret;
    }

    static Sudoku_RC_T checkCols(SudokuPuzzle_P p)
    {
        Sudoku_RC_T ret = SUDOKU_RC_SUCCESS;

        for (Sudoku_Column_Index_T col = 0; col < NUM_COLS; col++)
        {
            uint32_t val = SUDOKU_BIT_NO_VALUE;
            for (Sudoku_Row_Index_T row = 0; row < NUM_ROWS; row++)
            {
                if ((val & p->grid[row][col].value) != 0)
                {
                    return SUDOKU_RC_ERROR;
                }
                else
                {
                    val |= p->grid[row][col].value;
                }
            }
        }

        return ret;
    }

    static Sudoku_RC_T checkSubgrid(SudokuPuzzle_P p)
    {
        Sudoku_RC_T ret = SUDOKU_RC_SUCCESS;

        for (Sudoku_Row_Index_T sub_row = 0; sub_row < NUM_SUBGRID_ROWS; sub_row++)
        {
            for (Sudoku_Column_Index_T sub_col = 0; sub_col < NUM_SUBGRID_COLS; sub_col++)
            {
                uint32_t val = SUDOKU_BIT_NO_VALUE;
                for (unsigned int i = 0; i < NUM_SUBGRID_ROWS; i++)
                {
                    for (unsigned int j = 0; j < NUM_SUBGRID_COLS; j++)
                    {
                        uint32_t current_val = p->grid[3 * sub_row + i][3 * sub_col + j].value;
                        if (val & current_val)
                        {
                            return SUDOKU_RC_ERROR;
                        }
                        else
                        {
                            val |= current_val;
                        }
                    }
                }
            }
        }
        return ret;
    }

    static Sudoku_RC_T checkEmptyVals(SudokuPuzzle_P p)
    {
        Sudoku_RC_T ret = SUDOKU_RC_SUCCESS;
        unsigned int empty_val = 0;
        unsigned int invalid_val = 0;

        for (Sudoku_Row_Index_T row = 0; row < NUM_ROWS; row++)
        {
            for (Sudoku_Column_Index_T col = 0; col < NUM_COLS; col++)
            {
                uint32_t val = p->grid[row][col].value;
                if (val == SUDOKU_BIT_NO_VALUE)
                {
                    empty_val++;
                }
                else if (val == SUDOKU_BIT_INVALID_VALUE)
                {
                    invalid_val++;
                }
            }
        }

        if (invalid_val)
        {
            ret = SUDOKU_RC_ERROR;
        }
        else if (empty_val)
        {
            ret = SUDOKU_RC_PRUNE;
        }

        return ret;
    }

    Sudoku_RC_T Sudoku_Check(SudokuPuzzle_P p)
    {
        if (NULL == p)
        {
            return SUDOKU_RC_ERROR;
        }

        Sudoku_RC_T ret = checkRows(p);

        if (SUDOKU_RC_SUCCESS == ret)
        {
            ret = checkCols(p);
        }

        if (SUDOKU_RC_SUCCESS == ret)
        {
            ret = checkSubgrid(p);
        }

        if (SUDOKU_RC_SUCCESS == ret)
        {
            ret = checkEmptyVals(p);
        }

        return ret;
    }

    static int generateRowMask(SudokuPuzzle_P p, Sudoku_Row_Index_T row)
    {
        int change = 0;
        uint32_t mask = (uint32_t)SUDOKU_BIT_NO_VALUE;
        uint32_t old_mask = p->row_candidates[row];

        for (Sudoku_Column_Index_T col = 0; col < NUM_COLS; col++)
        {
            mask |= p->grid[row][col].value;
        }

        p->row_candidates[row] = (~mask & SUDOKU_MASK_ALL);

        if (old_mask != p->row_candidates[row])
        {
            change = 1;
        }
        return change;
    }

    static int generateColumnMask(SudokuPuzzle_P p, Sudoku_Column_Index_T col)
    {
        int change = 0;
        uint32_t mask = (uint32_t)SUDOKU_BIT_NO_VALUE;
        uint32_t old_mask = p->col_candidates[col];

        for (Sudoku_Row_Index_T row = 0; row < NUM_ROWS; row++)
        {
            mask |= p->grid[row][col].value;
        }

        p->col_candidates[col] = (~mask & SUDOKU_MASK_ALL);

        if (old_mask != p->col_candidates[col])
        {
            change = 1;
        }
        return change;
    }

    static int generateSubGridMask(SudokuPuzzle_P p, Sudoku_Row_Index_T sub_row, Sudoku_Column_Index_T sub_col)
    {
        int change = 0;

        return change;
    }

    static int generateRowMasks(SudokuPuzzle_P p)
    {
        int ret = 0;

        for (Sudoku_Row_Index_T row = 0; row < NUM_ROWS; row++)
        {
            ret += generateRowMask(p, row);
        }

        return ret;
    }

    static int generateColumnMasks(SudokuPuzzle_P p)
    {
        int ret = 0;

        for (Sudoku_Column_Index_T col = 0; col < NUM_COLS; col++)
        {
            ret += generateColumnMask(p, col);
        }

        return ret;
    }

    static int generateSubgridMasks(SudokuPuzzle_P p)
    {
        int ret = 0;


        return ret;
    }

    Sudoku_RC_T Sudoku_InitializeFromArray(SudokuPuzzle_P p, const char *sudoku_array)
    {
        if (NULL == sudoku_array)
        {
            return SUDOKU_RC_ERROR;
        }

        Sudoku_RC_T rc = Sudoku_InitializePuzzle(p);

        if (SUDOKU_RC_SUCCESS == rc)
        {
            for (unsigned int row = 0; row < NUM_ROWS; row++)
            {
                for (unsigned int col = 0; col < NUM_COLS; col++)
                {
                    char char_value[2];

                    (void)strncpy_s(char_value, sizeof(char_value), sudoku_array + row * NUM_COLS + col, 1);

                    if (isdigit(char_value[0]))
                    {
                        int val = atoi(char_value);
                        if (val != 0)
                        {
                            rc = Sudoku_SetValue(p, row, col, val);
                        }
                    }
                }
            }
        }
        return rc;
    }

    int Sudoku_PrintPuzzle(SudokuPuzzle_P p)
    {
        int ret = 0;
        if (NULL == p)
            return -1;

        int i;

        for (i = 0; i < NUM_ROWS; i++)
        {
            if (!(i % 3))
                printf("-------------------\n");
            printf("|%d %d %d|%d %d %d|%d %d %d|\n",
                   Sudoku_GetValue(p, i, 0),
                   Sudoku_GetValue(p, i, 1),
                   Sudoku_GetValue(p, i, 2),
                   Sudoku_GetValue(p, i, 3),
                   Sudoku_GetValue(p, i, 4),
                   Sudoku_GetValue(p, i, 5),
                   Sudoku_GetValue(p, i, 6),
                   Sudoku_GetValue(p, i, 7),
                   Sudoku_GetValue(p, i, 8));
        }
        printf("-------------------\n");

        return ret;
    }

#ifdef __cplusplus
}
#endif

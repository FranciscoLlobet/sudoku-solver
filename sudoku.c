#ifdef __cplusplus
extern "C"
{
#endif

#include "_sudoku.h"

#include <ctype.h>

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

    Sudoku_RC_T Sudoku_SetValueUsingBitmask(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, Sudoku_BitValues_T value)
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
        
        p->grid[row][col].candidates = (uint32_t)SUDOKU_MASK_NONE;
        p->grid[row][col].value = (uint32_t)value;

        return ret;
    }

    static enum SudokuValues_E convertMaskToValue(uint32_t val)
    {
        enum SudokuValues_E value = SUDOKU_NO_VALUE;

        switch ((enum SudokuBitValues_E)(val))
        {
        case SUDOKU_BIT_INVALID_VALUE:
            value = SUDOKU_INVALID_VALUE;
            break;
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
            value = SUDOKU_NOT_EXCLUSIVE_VALUE;
            break;
        }

        return value;
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

        return (int)convertMaskToValue(p->grid[row][col].value);
    }

    Sudoku_BitValues_T Sudoku_GetCandidates(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col)
    {
        Sudoku_RC_T ret = SUDOKU_RC_SUCCESS;
        unsigned int value = SUDOKU_INVALID_VALUE;

        if (NULL == p)
        {
            return SUDOKU_BIT_INVALID_VALUE;
        }
        else if (col >= NUM_COLS)
        {
            return SUDOKU_BIT_INVALID_VALUE;
        }
        else if (row >= NUM_ROWS)
        {
            return SUDOKU_BIT_INVALID_VALUE;
        }

        return (Sudoku_BitValues_T)convertMaskToValue(p->grid[row][col].candidates);
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
        Sudoku_Row_Index_T start_row = 3 * sub_row;
        Sudoku_Column_Index_T start_col = 3 * sub_col;
        uint32_t mask = (uint32_t)SUDOKU_BIT_NO_VALUE;
        uint32_t old_mask = p->sub_candidates[sub_row][sub_col];

        for (Sudoku_Row_Index_T row = 0; row < NUM_SUBGRID_ROWS; row++)
        {
            for (Sudoku_Column_Index_T col = 0; col < NUM_SUBGRID_COLS; col++)
            {
                mask |= p->grid[start_row + row][start_col + col].value;
            }
        }

        p->sub_candidates[sub_row][sub_col] = (~mask & SUDOKU_MASK_ALL);

        if (old_mask != p->sub_candidates[sub_row][sub_col])
        {
            change = 1;
        }

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

        for (Sudoku_Row_Index_T sub_row = 0; sub_row < NUM_SUBGRID_ROWS; sub_row++)
        {
            for (Sudoku_Column_Index_T sub_col = 0; sub_col < NUM_SUBGRID_COLS; sub_col++)
            {
                ret += generateSubGridMask(p, sub_row, sub_col);
            }
        }

        return ret;
    }

    static int generateCellMask(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col)
    {
        int change = 0; // No change

        uint32_t row_mask = p->row_candidates[row]; // Row Candidates
        uint32_t col_mask = p->col_candidates[col]; // Column Candidate
        uint32_t sub_mask = p->sub_candidates[row / 3][col / 3];

        uint32_t cell_mask = p->grid[row][col].candidates; // Own Cell mask

        p->grid[row][col].candidates = (row_mask & col_mask & sub_mask & cell_mask);

        if (cell_mask != p->grid[row][col].candidates)
        {
            change = 1; // Change
        }

        return change;
    }

    static int generatePuzzleCellMasks(SudokuPuzzle_P p)
    {
        int change = 0;

        for (Sudoku_Row_Index_T row = 0; row < NUM_ROWS; row++)
        {
            for (Sudoku_Column_Index_T col = 0; col < NUM_COLS; col++)
            {
                change += generateCellMask(p, row, col);
            }
        }

        return change;
    }

    static int generateCandidateMasks(SudokuPuzzle_P p)
    {
        int changes = 0;

        do
        {
            changes = generateRowMasks(p) + generateColumnMasks(p) + generateSubgridMasks(p) +  generatePuzzleCellMasks(p);
        } while (changes > 0);
        
        return changes;
    }


    static int updateCellCandidates(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col)
    {
        int change = (int)SUDOKU_RC_SUCCESS;

        switch (convertMaskToValue(p->grid[row][col].candidates))
        {
        case SUDOKU_INVALID_VALUE:
            p->grid[row][col].value = (uint32_t)SUDOKU_MASK_INVALID;
            p->grid[row][col].candidates = (uint32_t)SUDOKU_MASK_INVALID;
            change = (int)SUDOKU_RC_ERROR;
            break;
        case SUDOKU_NO_VALUE:
            if (p->grid[row][col].value == (uint32_t)SUDOKU_MASK_NONE)
            {
                p->grid[row][col].value = (uint32_t)SUDOKU_MASK_INVALID;
                p->grid[row][col].candidates = (uint32_t)SUDOKU_MASK_INVALID;
                change = (int)SUDOKU_RC_ERROR;
            }
            else
            {
                change = (int)SUDOKU_RC_SUCCESS;
            }
            break;
        case SUDOKU_NOT_EXCLUSIVE_VALUE:
            change = (int)SUDOKU_RC_SUCCESS;
            break;
        default:
            p->grid[row][col].value = p->grid[row][col].candidates;
            p->grid[row][col].candidates = (uint32_t)SUDOKU_MASK_NONE;
            change = (int)SUDOKU_RC_PRUNE;
            break;
        }

        return change;
    }

    static int updatePuzzleCandidates(SudokuPuzzle_P p)
    {
        int change = 0;
        int prune_counter = 0;

        for (Sudoku_Row_Index_T row = 0; row < NUM_ROWS; row++)
        {
            for (Sudoku_Column_Index_T col = 0; col < NUM_COLS; col++)
            {
                change = updateCellCandidates(p, row, col);
                if (change >= 0)
                    prune_counter += change;
                else
                    return (int)SUDOKU_RC_ERROR;
            }
        }

        return prune_counter;
    }

    static Sudoku_RC_T removeCandidate(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, uint32_t candidate)
    {
        Sudoku_RC_T ret = SUDOKU_RC_SUCCESS;

        p->grid[row][col].candidates &= (SUDOKU_MASK_ALL & ~candidate);

        return ret;
    }

    Sudoku_RC_T Sudoku_RemoveCandidate(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, Sudoku_BitValues_T candidate)
    {
        if (NULL == p)
        {
            return SUDOKU_RC_ERROR;
        }

        return removeCandidate(p, row, col, (uint32_t)candidate);
    }

    static int countCandidatesInMask(uint32_t mask)
    {
        int count = 0;
        for (unsigned int i = 0; i < NUM_CANDIDATES; i++)
        {
            if (mask & (1 << i))
                count++;
        }
        return count;
    }

    int countCandidatesInCell(SudokuPuzzle_P p, unsigned int row, unsigned int col)
    {
        return countCandidatesInMask(p->grid[row][col].candidates);
    }

    Sudoku_RC_T countCandidatesInPuzzle(SudokuPuzzle_P p)
    {
        Sudoku_RC_T ret = SUDOKU_RC_SUCCESS;
        for (Sudoku_Row_Index_T row = 0; row < NUM_ROWS; row++)
        {
            for (Sudoku_Column_Index_T col = 0; col < NUM_ROWS; col++)
            {
                p->n_candidates[row][col] = countCandidatesInCell(p, row, col);
            }
        }

        return ret;
    }

    Sudoku_BitValues_T Sudoku_SelectCandidate(SudokuPuzzle_P p, Sudoku_Row_Index_T *row, Sudoku_Column_Index_T *col)
    {
        Sudoku_BitValues_T val = SUDOKU_BIT_INVALID_VALUE;
        if ((NULL == p) || (NULL == row) || (NULL == col))
        {
            return SUDOKU_BIT_INVALID_VALUE;
        }

        (void)countCandidatesInPuzzle(p); // Count the candidates

        for (unsigned int n_cand = 2; n_cand <= NUM_CANDIDATES; n_cand++)
        {
            for (*row = 0; *row < NUM_ROWS; (*row)++)
            {
                for (*col = 0; *col < NUM_ROWS; (*col)++)
                {
                    if (n_cand == p->n_candidates[*row][*col])
                    {
                        for (size_t i = 0; i < NUM_CANDIDATES; i++)
                        {
                            val = (Sudoku_BitValues_T)((uint32_t)(1 << i));
                            if (p->grid[*row][*col].candidates & (uint32_t)val)
                            {
                                return val;
                            }
                        }
                    }
                }
            }
        }

        return val;
    }

    Sudoku_RC_T Sudoku_PrunePuzzle(SudokuPuzzle_P p)
    {
        Sudoku_RC_T ret = SUDOKU_RC_SUCCESS;

        int changes = 0;
        do{
            changes = generateColumnMasks(p) + generateRowMasks(p) + generateSubgridMasks(p) + generatePuzzleCellMasks(p) + updatePuzzleCandidates(p);
        }
        while((changes > 0));

        return Sudoku_Check(p);
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

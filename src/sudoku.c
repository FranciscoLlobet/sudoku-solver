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

        /* Initialize main grid, row candidates, and col candidates */
        for (Sudoku_Row_Index_T row = 0; row < (Sudoku_Row_Index_T)NUM_ROWS; row++)
        {
            p->row_candidates[row] = (uint32_t)SUDOKU_MASK_ALL;

            for (Sudoku_Column_Index_T col = 0; col < (Sudoku_Column_Index_T)NUM_COLS; col++)
            {
                p->grid[row][col].candidates = (uint32_t)SUDOKU_MASK_ALL;
                p->grid[row][col].value = (uint32_t)SUDOKU_BIT_NO_VALUE;

                if (row < NUM_SUBGRID_ROWS && col < NUM_SUBGRID_COLS)
                {
                    p->sub_candidates[row][col] = (uint32_t)SUDOKU_MASK_ALL;
                }

                if (row == 0)
                {
                    p->col_candidates[col] = (uint32_t)SUDOKU_MASK_ALL;
                }
            }
        }

        return ret;
    }

    /* Set Value */
    Sudoku_RC_T Sudoku_SetValue(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, int val)
    {
        if (p == (SudokuPuzzle_P)NULL)
        {
            return SUDOKU_RC_NULL_POINTER;
        }
        else if (col >= (Sudoku_Column_Index_T)NUM_COLS)
        {
            return SUDOKU_RC_INVALID_INPUT;
        }
        else if (row >= (Sudoku_Row_Index_T)NUM_ROWS)
        {
            return SUDOKU_RC_INVALID_INPUT;
        }

        if (val == 0)
        {
            p->grid[row][col].candidates = (uint32_t)SUDOKU_MASK_ALL; /* No candidate left */
            p->grid[row][col].value = (uint32_t)SUDOKU_BIT_NO_VALUE;
        }
        else if (val <= 9)
        {
            p->grid[row][col].candidates = (uint32_t)SUDOKU_MASK_NONE; /* No candidate left */
            p->grid[row][col].value = 1 << ((unsigned int)val - 1);
        }
        else
        {
            p->grid[row][col].candidates = (uint32_t)SUDOKU_MASK_NONE;
            p->grid[row][col].value = (uint32_t)SUDOKU_BIT_INVALID_VALUE;
            return SUDOKU_RC_INVALID_VALUE;
        }
        return SUDOKU_RC_SUCCESS;
    }

    Sudoku_RC_T Sudoku_SetValueUsingBitmask(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, Sudoku_BitValues_T value)
    {
        if (p == (SudokuPuzzle_P)NULL)
        {
            return SUDOKU_RC_NULL_POINTER;
        }
        else if (col >= (Sudoku_BitValues_T)NUM_COLS)
        {
            return SUDOKU_RC_INVALID_INPUT;
        }
        else if (row >= (Sudoku_Row_Index_T)NUM_ROWS)
        {
            return SUDOKU_RC_INVALID_INPUT;
        }

        /* This method does not check for the actual value, since it assumes that the Sudoku_BitValues_T enum is used */
        p->grid[row][col].candidates = (uint32_t)SUDOKU_MASK_NONE;
        p->grid[row][col].value = (uint32_t)value;

        return SUDOKU_RC_SUCCESS;
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
        /* Negative values are returned in case there is an error in the input parameters */
        if (NULL == p)
        {
            return (int)SUDOKU_RC_NULL_POINTER;
        }
        else if (col >= NUM_COLS)
        {
            return (int)SUDOKU_RC_INVALID_INPUT;
        }
        else if (row >= NUM_ROWS)
        {
            return (int)SUDOKU_RC_INVALID_INPUT;
        }

        return (int)convertMaskToValue(p->grid[row][col].value);
    }

    /* Probably not used? */
    int Sudoku_GetCandidates(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col)
    {
        /* Get the candidates as bitmask */
        /* */
        if (NULL == p)
        {
            return (int)SUDOKU_RC_NULL_POINTER;
        }
        else if (col >= NUM_COLS)
        {
            return (int)SUDOKU_RC_INVALID_INPUT;
        }
        else if (row >= NUM_ROWS)
        {
            return (int)SUDOKU_RC_INVALID_INPUT;
        }

        return (int)convertMaskToValue(p->grid[row][col].candidates);
    }

    static Sudoku_RC_T checkRows(SudokuPuzzle_P p)
    {
        if (NULL == p)
        {
            return SUDOKU_RC_NULL_POINTER;
        }

        for (Sudoku_Row_Index_T row = 0; row < NUM_ROWS; row++)
        {
            uint32_t rowValues = SUDOKU_BIT_NO_VALUE;
            for (Sudoku_Column_Index_T col = 0; col < NUM_COLS; col++)
            {
                uint32_t cellValue = p->grid[row][col].value;
                if ((rowValues & cellValue) != 0)
                {
                    return SUDOKU_RC_ERROR;
                }
                rowValues |= cellValue;
            }
        }

        return SUDOKU_RC_SUCCESS;
    }

    static Sudoku_RC_T checkCols(SudokuPuzzle_P p)
    {
        if (NULL == p)
        {
            return SUDOKU_RC_NULL_POINTER;
        }

        for (Sudoku_Column_Index_T col = 0; col < NUM_COLS; col++)
        {
            uint32_t colValues = SUDOKU_BIT_NO_VALUE;
            for (Sudoku_Row_Index_T row = 0; row < NUM_ROWS; row++)
            {
                uint32_t cellValue = p->grid[row][col].value;
                if ((colValues & cellValue) != 0)
                {
                    return SUDOKU_RC_ERROR;
                }
                colValues |= cellValue;
            }
        }

        return SUDOKU_RC_SUCCESS;
    }

    static Sudoku_RC_T checkSubgrid(SudokuPuzzle_P p)
    {
        if (NULL == p)
        {
            return SUDOKU_RC_NULL_POINTER;
        }

        for (Sudoku_Row_Index_T sub_row = 0; sub_row < NUM_SUBGRID_ROWS; sub_row++)
        {
            for (Sudoku_Column_Index_T sub_col = 0; sub_col < NUM_SUBGRID_COLS; sub_col++)
            {
                uint32_t subgridValues = SUDOKU_BIT_NO_VALUE;
                for (size_t i = 0; i < NUM_SUBGRID_ROWS; i++)
                {
                    for (size_t j = 0; j < NUM_SUBGRID_COLS; j++)
                    {
                        uint32_t cellValue = p->grid[3 * sub_row + i][3 * sub_col + j].value;
                        if (subgridValues & cellValue)
                        {
                            return SUDOKU_RC_ERROR;
                        }
                        subgridValues |= cellValue;
                    }
                }
            }
        }

        return SUDOKU_RC_SUCCESS;
    }

    /**
     * @brief Check the Sudoku grid for empty and invalid values.
     *
     * This function iterates through each cell in the grid, counts the
     * number of empty and invalid values, and returns an appropriate
     * Sudoku return code based on the count.
     *
     * @param[in] p Pointer to the SudokuPuzzle structure.
     *
     * @return SUDOKU_RC_SUCCESS if there are no empty or invalid values,
     *         SUDOKU_RC_ERROR if there are any invalid values,
     *         SUDOKU_RC_PRUNE if there are any empty values but no invalid values.
     */
    static Sudoku_RC_T checkEmptyVals(SudokuPuzzle_P p)
    {
        Sudoku_RC_T ret = SUDOKU_RC_SUCCESS;
        unsigned int empty_val = 0;
        unsigned int invalid_val = 0;

        // Iterate through each row and column
        for (Sudoku_Row_Index_T row = 0; row < NUM_ROWS; row++)
        {
            for (Sudoku_Column_Index_T col = 0; col < NUM_COLS; col++)
            {
                uint32_t val = p->grid[row][col].value;

                // Check for empty values
                if (val == SUDOKU_BIT_NO_VALUE)
                {
                    empty_val++;
                }
                // Check for invalid values
                else if (val == SUDOKU_BIT_INVALID_VALUE)
                {
                    invalid_val++;
                }
            }
        }

        // Set the return code based on the number of empty and invalid values
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

    /**
     * @brief Generates a row mask for a given Sudoku puzzle and row index.
     *
     * @param p Pointer to a SudokuPuzzle_P object.
     * @param row Row index to generate the mask for.
     * @return 1 if the row mask changed, 0 otherwise.
     */
    static int generateRowMask(SudokuPuzzle_P p, Sudoku_Row_Index_T row)
    {
        uint32_t mask = SUDOKU_BIT_NO_VALUE;
        uint32_t old_mask = p->row_candidates[row];

        for (Sudoku_Column_Index_T col = 0; col < (Sudoku_Column_Index_T)NUM_COLS; ++col)
        {
            mask |= p->grid[row][col].value;
        }

        p->row_candidates[row] = (~mask & SUDOKU_MASK_ALL);

        return old_mask != p->row_candidates[row];
    }

    /**
     * @brief Generates a column mask for a given Sudoku puzzle and column index.
     *
     * @param p Pointer to a SudokuPuzzle_P object.
     * @param col Column index to generate the mask for.
     * @return 1 if the column mask changed, 0 otherwise.
     */
    static int generateColumnMask(SudokuPuzzle_P p, Sudoku_Column_Index_T col)
    {
        uint32_t mask = SUDOKU_BIT_NO_VALUE;
        uint32_t old_mask = p->col_candidates[col];

        for (Sudoku_Row_Index_T row = 0; row < (Sudoku_Row_Index_T)NUM_ROWS; ++row)
        {
            mask |= p->grid[row][col].value;
        }

        p->col_candidates[col] = (~mask & SUDOKU_MASK_ALL);

        return old_mask != p->col_candidates[col];
    }

    /**
     * @brief Generates a subgrid mask for a given Sudoku puzzle and subgrid indices.
     *
     * @param p Pointer to a SudokuPuzzle_P object.
     * @param sub_row Subgrid row index to generate the mask for.
     * @param sub_col Subgrid column index to generate the mask for.
     * @return 1 if the subgrid mask changed, 0 otherwise.
     */
    static int generateSubGridMask(SudokuPuzzle_P p, Sudoku_Row_Index_T sub_row, Sudoku_Column_Index_T sub_col)
    {
        Sudoku_Row_Index_T start_row = 3 * sub_row;
        Sudoku_Column_Index_T start_col = 3 * sub_col;
        uint32_t mask = SUDOKU_BIT_NO_VALUE;
        uint32_t old_mask = p->sub_candidates[sub_row][sub_col];

        for (Sudoku_Row_Index_T row = 0; row < (Sudoku_Row_Index_T)NUM_SUBGRID_ROWS; ++row)
        {
            for (Sudoku_Column_Index_T col = 0; col < (Sudoku_Column_Index_T)NUM_SUBGRID_COLS; ++col)
            {
                mask |= p->grid[start_row + row][start_col + col].value;
            }
        }

        p->sub_candidates[sub_row][sub_col] = (~mask & SUDOKU_MASK_ALL);

        return old_mask != p->sub_candidates[sub_row][sub_col];
    }

    /**
     * @brief Generates row masks for a given Sudoku puzzle.
     *
     * @param p Pointer to a SudokuPuzzle_P object.
     * @return The number of row masks changed.
     */
    static int generateRowMasks(SudokuPuzzle_P p)
    {
        int ret = 0;

        for (Sudoku_Row_Index_T row = 0; row < NUM_ROWS; ++row)
        {
            ret += generateRowMask(p, row);
        }

        return ret;
    }

    /**
     * @brief Generates column masks for a given Sudoku puzzle.
     *
     * @param p Pointer to a SudokuPuzzle_P object.
     * @return The number of column masks changed.
     */
    static int generateColumnMasks(SudokuPuzzle_P p)
    {
        int ret = 0;

        for (Sudoku_Column_Index_T col = 0; col < NUM_COLS; ++col)
        {
            ret += generateColumnMask(p, col);
        }

        return ret;
    }

    /**
     * @brief Generates subgrid masks for a given Sudoku puzzle.
     *
     * @param p Pointer to a SudokuPuzzle_P object.
     * @return The number of subgrid masks changed.
     */
    static int generateSubgridMasks(SudokuPuzzle_P p)
    {
        int ret = 0;

        for (Sudoku_Row_Index_T sub_row = 0; sub_row < NUM_SUBGRID_ROWS; ++sub_row)
        {
            for (Sudoku_Column_Index_T sub_col = 0; sub_col < NUM_SUBGRID_COLS; ++sub_col)
            {
                ret += generateSubGridMask(p, sub_row, sub_col);
            }
        }

        return ret;
    }

    /**
     * @brief Generates the mask for a single cell in a Sudoku puzzle.
     *
     * @param p Pointer to a SudokuPuzzle_P object.
     * @param row The row index of the cell.
     * @param col The column index of the cell.
     * @return 1 if the cell mask has changed, 0 otherwise.
     */
    static int generateCellMask(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col)
    {
        uint32_t row_mask = p->row_candidates[row];
        uint32_t col_mask = p->col_candidates[col];
        uint32_t sub_mask = p->sub_candidates[row / 3][col / 3];
        uint32_t cell_mask = p->grid[row][col].candidates;

        p->grid[row][col].candidates = (row_mask & col_mask & sub_mask & cell_mask);

        return cell_mask != p->grid[row][col].candidates;
    }

    /**
     * @brief Generates the masks for all cells in a Sudoku puzzle.
     *
     * @param p Pointer to a SudokuPuzzle_P object.
     * @return The number of cell masks changed.
     */
    static int generatePuzzleCellMasks(SudokuPuzzle_P p)
    {
        int change = 0;

        for (Sudoku_Row_Index_T row = 0; row < NUM_ROWS; ++row)
        {
            for (Sudoku_Column_Index_T col = 0; col < NUM_COLS; ++col)
            {
                change += generateCellMask(p, row, col);
            }
        }

        return change;
    }

    /**
     * @brief Generates candidate masks for a Sudoku puzzle.
     *
     * This function iteratively generates masks for rows, columns, subgrids, and cells until no changes occur.
     *
     * @param p Pointer to a SudokuPuzzle_P object.
     * @return The number of changes in the last iteration.
     */
    static int generateCandidateMasks(SudokuPuzzle_P p)
    {
        int changes;

        do
        {
            changes = generateRowMasks(p) + generateColumnMasks(p) + generateSubgridMasks(p) + generatePuzzleCellMasks(p);
        } while (changes > 0);

        return changes;
    }

    /**
     * @brief Updates the cell candidates of a Sudoku puzzle.
     *
     * This function updates the cell candidates based on the current mask value.
     *
     * @param p Pointer to a SudokuPuzzle_P object.
     * @param row Row index of the cell.
     * @param col Column index of the cell.
     * @return SUDOKU_RC_SUCCESS if successful, SUDOKU_RC_ERROR if an error occurs, or SUDOKU_RC_PRUNE if the candidates were pruned.
     */
    static int updateCellCandidates(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col)
    {
        int change = SUDOKU_RC_SUCCESS;
        enum SudokuValues_E cell_value = convertMaskToValue(p->grid[row][col].candidates);

        switch (cell_value)
        {
        case SUDOKU_INVALID_VALUE:
            p->grid[row][col].value = SUDOKU_MASK_INVALID;
            p->grid[row][col].candidates = SUDOKU_MASK_INVALID;
            change = SUDOKU_RC_ERROR;
            break;
        case SUDOKU_NO_VALUE:
            if (p->grid[row][col].value == SUDOKU_MASK_NONE)
            {
                p->grid[row][col].value = SUDOKU_MASK_INVALID;
                p->grid[row][col].candidates = SUDOKU_MASK_INVALID;
                change = SUDOKU_RC_ERROR;
            }
            else
            {
                change = SUDOKU_RC_SUCCESS;
            }
            break;
        case SUDOKU_NOT_EXCLUSIVE_VALUE:
            change = SUDOKU_RC_SUCCESS;
            break;
        default:
            p->grid[row][col].value = p->grid[row][col].candidates;
            p->grid[row][col].candidates = SUDOKU_MASK_NONE;
            change = SUDOKU_RC_PRUNE;
            break;
        }

        return change;
    }

    /**
     * @brief Updates the candidates of all cells in a Sudoku puzzle.
     *
     * This function iterates through all cells in the puzzle and updates their candidates.
     * It also counts the number of pruned candidates and returns the result.
     *
     * @param p Pointer to a SudokuPuzzle_P object.
     * @return Number of pruned candidates, or SUDOKU_RC_ERROR on failure.
     */
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
                {
                    prune_counter += change;
                }
                else
                {
                    return SUDOKU_RC_ERROR; // Think about returning change (?)
                }
            }
        }

        return prune_counter;
    }

    /**
     * @brief Removes a candidate from a cell's candidate mask.
     *
     * @param p Pointer to a SudokuPuzzle_P object.
     * @param row Row index.
     * @param col Column index.
     * @param candidate Candidate to remove (as a bitmask).
     * @return SUDOKU_RC_SUCCESS on success.
     */
    static Sudoku_RC_T removeCandidate(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, uint32_t candidate)
    {
        p->grid[row][col].candidates &= (SUDOKU_MASK_ALL & ~candidate);
        return SUDOKU_RC_SUCCESS;
    }

    /**
     * @brief Removes a candidate from a cell's candidate mask (public version).
     *
     * @param p Pointer to a SudokuPuzzle_P object.
     * @param row Row index.
     * @param col Column index.
     * @param candidate Candidate to remove (as a Sudoku_BitValues_T value).
     * @return SUDOKU_RC_SUCCESS on success, SUDOKU_RC_ERROR if the puzzle pointer is NULL.
     */
    Sudoku_RC_T Sudoku_RemoveCandidate(SudokuPuzzle_P p, Sudoku_Row_Index_T row, Sudoku_Column_Index_T col, Sudoku_BitValues_T candidate)
    {
        if (p == NULL)
        {
            return SUDOKU_RC_ERROR;
        }

        return removeCandidate(p, row, col, (uint32_t)candidate);
    }

    /**
     * @brief Counts the number of candidates in a given bitmask.
     *
     * @param mask Bitmask containing candidates.
     * @return The number of candidates in the mask.
     */
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

    /**
     * @brief Counts the number of candidates in a specific cell of the puzzle.
     *
     * @param p Pointer to a SudokuPuzzle_P object.
     * @param row Row index.
     * @param col Column index.
     * @return The number of candidates in the specified cell.
     */
    int countCandidatesInCell(SudokuPuzzle_P p, unsigned int row, unsigned int col)
    {
        return countCandidatesInMask(p->grid[row][col].candidates);
    }

    /**
     * @brief Fills the n_candidates field of the puzzle structure with the number of candidates for each cell.
     *
     * @param p Pointer to a SudokuPuzzle_P object.
     * @return SUDOKU_RC_SUCCESS on success.
     */
    Sudoku_RC_T countCandidatesInPuzzle(SudokuPuzzle_P p)
    {
        for (Sudoku_Row_Index_T row = 0; row < NUM_ROWS; row++)
        {
            for (Sudoku_Column_Index_T col = 0; col < NUM_COLS; col++)
            {
                p->n_candidates[row][col] = countCandidatesInCell(p, row, col);
            }
        }

        return SUDOKU_RC_SUCCESS;
    }


    Sudoku_BitValues_T Sudoku_SelectCandidate(SudokuPuzzle_P p, Sudoku_Row_Index_T *row, Sudoku_Column_Index_T *col)
    {
        if (p == NULL || row == NULL || col == NULL)
            {
                return SUDOKU_BIT_INVALID_VALUE;
            }

        countCandidatesInPuzzle(p); // Count the candidates

        // Get number of candidates per cell (prio 1) -> generate prio queue
        // Get number of candidates per value (prio 2) -> generate prio queue

        for (unsigned int n_cand = 2; n_cand <= NUM_CANDIDATES; n_cand++)
        {
            for (*row = 0; *row < NUM_ROWS; (*row)++)
            {
                for (*col = 0; *col < NUM_COLS; (*col)++)
                {
                    if (n_cand == p->n_candidates[*row][*col])
                    {
                        uint32_t cell_candidates = p->grid[*row][*col].candidates;
                        for (size_t i = 0; i < NUM_CANDIDATES; i++)
                        {
                            uint32_t val = (1 << i);
                            if (cell_candidates & val)
                            {
                                return (Sudoku_BitValues_T)val;
                            }
                        }
                    }
                }
            }
        }

        return SUDOKU_BIT_INVALID_VALUE;
    }

    /**
     * @brief Prunes the Sudoku puzzle by iteratively generating candidate masks and updating cell candidates.
     *
     * This function repeatedly calls the functions to generate column masks, row masks, subgrid masks, and
     * puzzle cell masks, and updates the puzzle's candidates until no more changes are detected.
     * After pruning, the function checks if the Sudoku puzzle is solved or if an error has occurred.
     *
     * @param[in] p Pointer to a Sudoku puzzle structure
     * @return A Sudoku return code indicating the status of the puzzle after pruning (SUDOKU_RC_SUCCESS, SUDOKU_RC_SOLVED, or SUDOKU_RC_ERROR)
     */
    Sudoku_RC_T Sudoku_PrunePuzzle(SudokuPuzzle_P p)
    {
        Sudoku_RC_T ret = SUDOKU_RC_SUCCESS;
        int changes;

        do
        {
            changes = 0;
            changes += generateColumnMasks(p);
            changes += generateRowMasks(p);
            changes += generateSubgridMasks(p);
            changes += generatePuzzleCellMasks(p);
            changes += updatePuzzleCandidates(p);
        } while (changes > 0);

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

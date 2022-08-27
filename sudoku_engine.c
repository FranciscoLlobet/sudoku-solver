
#include "sudoku.h"
#include "sudoku_engine.h"

/* Static Method reference */

/* Get References */
static SudokuCell_P getCellReference(SudokuPuzzle_P puzzle, unsigned int row, unsigned int col);
static SudokuCell_P getRowReference(SudokuPuzzle_P puzzle, unsigned int row);
static SudokuCell_P getColumnReference(SudokuPuzzle_P puzzle, unsigned int col);
static SudokuCell_P getSubgridReference(SudokuPuzzle_P puzzle, unsigned int sub_row, unsigned int sub_col);


/* Set Values */
static enum SudokuValues_E setCellValue(SudokuCell_P cell, enum SudokuValues_E val);


/* Get Values */
static uint32_t getCellMask(SudokuCell_P cell);
static uint32_t getMaskFromValue(int val);
static enum SudokuValues_E getCellValue(SudokuCell_P cell);

/* Initialize Values */
static enum Sudoku_RC_E initializeCell(SudokuCell_P cell, enum CellType_E cell_type);
static enum Sudoku_RC_E initializeGrid(SudokuPuzzle_P puzzle);
static enum Sudoku_RC_E initializeRows(SudokuPuzzle_P puzzle);
static enum Sudoku_RC_E initializeColumns(SudokuPuzzle_P puzzle);
static enum Sudoku_RC_E initializeSubgrid(SudokuPuzzle_P puzzle);

/* Calculate Row Mask */

/* Calculate Column Mask */

/* Calculate Subgrid Mask */
static enum Sudoku_RC_E generateColumnMask(SudokuPuzzle_P puzzle, unsigned int col);
static enum Sudoku_RC_E generateRowMask(SudokuPuzzle_P puzzle, unsigned int row);
static enum Sudoku_RC_E generateSubCellMask(SudokuPuzzle_P puzzle, int sub_row, int sub_col);



/* Support */

/* Count Candidates in mask */
static int countCandidatesInMask(uint32_t mask);

/* Get value from mask */
static enum SudokuValues_E getValueFromMask(uint32_t cell_mask);


/* Get the Cell Reference */
static SudokuCell_P getCellReference(SudokuPuzzle_P puzzle, unsigned int row, unsigned int col)
{
    return &(puzzle->grid[row][col]);
}


static SudokuCell_P getRowReference(SudokuPuzzle_P puzzle, unsigned int row)
{
    if(NULL == puzzle)
        return (SudokuCell_P)NULL;
    if(NUM_ROWS <= row)
        return (SudokuCell_P)NULL;

    return &(puzzle->row[row]);
}

static SudokuCell_P getColumnReference(SudokuPuzzle_P puzzle, unsigned int col)
{
    if(NULL == puzzle)
        return (SudokuCell_P)NULL;
    if(NUM_COLS <= col)
        return (SudokuCell_P)NULL;

    return &(puzzle->col[col]);
}

static SudokuCell_P getSubgridReference(SudokuPuzzle_P puzzle, unsigned int sub_row, unsigned int sub_col)
{
    if(NULL == puzzle)
        return (SudokuCell_P)NULL;
    if((NUM_SUBGRID_ROWS <= sub_row) || (NUM_SUBGRID_COLS <= sub_col))
        return (SudokuCell_P)NULL;

    return &(puzzle->sub[sub_row][sub_col]);
}



/* Set a Cell Value. */
static enum SudokuValues_E setCellValue(SudokuCell_P cell, enum SudokuValues_E val)
{
    enum SudokuValues_E ret = SUDOKU_INVALID_VALUE;
    if(NULL == cell)
    {
        return SUDOKU_INVALID_VALUE;
    }

    if(SUDOKU_NO_VALUE != val)
    {
        cell->value = (int)val;
        cell->bit_value = getMaskFromValue(val);
        cell->candidates = SUDOKU_MASK_NONE;
        cell->n_candidate = SUDOKU_NO_VALUE;
        ret = val;
    }

    return ret;
}



enum SudokuValues_E SetCellValueInPuzzle(SudokuPuzzle_P puzzle, enum SudokuValues_E val, unsigned int row, unsigned int col)
{
    return setCellValue(getCellReference(puzzle, row, col), val);
}

static int countCandidatesInMask(uint32_t mask)
{
    int count = 0;
    for(unsigned int i = 0; i < NUM_CANDIDATES; i++)
    {
        if( mask & (1<<i) ) count++;
    }
    return count;
}

static enum Sudoku_RC_E setCellMask(SudokuCell_P cell, uint32_t mask)
{
    if(NULL == cell) return SUDOKU_RC_ERROR;

    cell->candidates = (mask & SUDOKU_MASK_ALL);
    cell->n_candidate = countCandidatesInMask(mask);

    return SUDOKU_RC_SUCCESS;
}

int GetCandidatesInCell(SudokuPuzzle_P p, unsigned int row, unsigned int col)
{
    return countCandidatesInMask(getCellMask(getCellReference(p, row, col)));
}

// Functions
int CountElementsInGrid(SudokuPuzzle_P p)
{
    int count = 0;
    if(NULL == p) return (int)SUDOKU_RC_ERROR;

    for(unsigned int row = 0; row < NUM_ROWS; row++)
    {
        for(unsigned col = 0; col < NUM_ROWS; col++)
        {
            int value = (int)GetCellValueInPuzzle(p, row, col);

            if(value > (int)SUDOKU_NO_VALUE) count++;
            else if(value == SUDOKU_INVALID_VALUE)
            {
                count = (int)SUDOKU_INVALID_VALUE;
            }
            if(0 > count) break;
        }

        if(0 > count) break;
    }

    return count;
}

static enum Sudoku_RC_E validateGrid(SudokuPuzzle_P p)
{
    enum Sudoku_RC_E ret = SUDOKU_RC_SUCCESS;
    enum SudokuValues_E value = SUDOKU_NO_VALUE;

    int count = 0;

    for(unsigned int row = 0; row < NUM_ROWS; row++)
    {
        for(unsigned int col = 0; col < NUM_COLS; col++)
        {
            value = GetCellValueInPuzzle(p, row, col);
            if(SUDOKU_NO_VALUE == value)
            {
                count++;
            }
            else if(SUDOKU_INVALID_VALUE == value)
            {
                return SUDOKU_RC_ERROR;
            }
        }
    }

    if(count == 0)
    {
        ret = SUDOKU_RC_SUCCESS; // Sudoku Solved
    }
    else if(count > 0)
    {
        ret = SUDOKU_RC_PRUNE; // Sudoku is not solved yet
    }

    return ret;
}


static uint32_t getCellMask(SudokuCell_P cell)
{
    return cell->candidates;
}

static enum SudokuValues_E getCellValue(SudokuCell_P cell)
{
    return (enum SudokuValues_E)cell->value;
}

enum SudokuValues_E GetCellValueInPuzzle(SudokuPuzzle_P puzzle, unsigned int row, unsigned int col)
{
    return(getCellValue(getCellReference(puzzle, row, col)));
}

static enum Sudoku_RC_E initializeCell(SudokuCell_P cell, enum CellType_E cell_type)
{
    if(NULL == cell) return SUDOKU_RC_ERROR;

    cell->value = SUDOKU_NO_VALUE;
    cell->cell_type = cell_type;
    cell->candidates = SUDOKU_MASK_ALL;
    cell->n_candidate = NUM_CANDIDATES;

    return SUDOKU_RC_SUCCESS;
}

static enum Sudoku_RC_E initializeGrid(SudokuPuzzle_P puzzle)
{
    enum Sudoku_RC_E ret = SUDOKU_RC_SUCCESS;

    if(NULL == puzzle) return SUDOKU_RC_ERROR;

    /* Initialize Cell Grid */
    for(unsigned int i = 0; i < NUM_ROWS; i++)
    {
        for(unsigned int j = 0; j < NUM_COLS; j++)
        {
            ret = initializeCell(getCellReference(puzzle,i,j), CELL_TYPE_SIMPLE_CELL);
            if(ret != SUDOKU_RC_SUCCESS) break;
        }

        if(ret != SUDOKU_RC_SUCCESS) break;
    }

    return ret;
}

static enum Sudoku_RC_E initializeRows(SudokuPuzzle_P puzzle)
{
    enum Sudoku_RC_E ret = SUDOKU_RC_SUCCESS;

    if(NULL == puzzle) return SUDOKU_RC_ERROR;

    unsigned int r = 0;
    for(r = 0; r < NUM_ROWS; r++)
    {
        ret = initializeCell(getRowReference(puzzle,r), CELL_TYPE_ROW);
        if(ret != SUDOKU_RC_SUCCESS) break;
    }

    return ret;
}

static enum Sudoku_RC_E initializeColumns(SudokuPuzzle_P puzzle)
{
    enum Sudoku_RC_E ret = SUDOKU_RC_SUCCESS;

    if(NULL == puzzle) return SUDOKU_RC_ERROR;

    for(unsigned int c = 0; c < NUM_COLS; c++)
    {
        ret = initializeCell(getColumnReference(puzzle,c), CELL_TYPE_COL);
        if(ret != SUDOKU_RC_SUCCESS) break;
    }

    return ret;
}

static enum Sudoku_RC_E initializeSubgrid(SudokuPuzzle_P puzzle)
{
    enum Sudoku_RC_E ret = SUDOKU_RC_SUCCESS;

    if(NULL == puzzle) return SUDOKU_RC_ERROR;

    /* Initialize Cell Grid */
    for(unsigned int i = 0; i < NUM_SUBGRID_ROWS; i++)
    {
        for(unsigned int j = 0; j < NUM_SUBGRID_COLS; j++)
        {
            ret = initializeCell(getSubgridReference(puzzle,i,j), CELL_TYPE_SUB);
            if(ret != SUDOKU_RC_SUCCESS) break;
        }

        if(ret != SUDOKU_RC_SUCCESS) break;
    }

    return ret;
}

enum Sudoku_RC_E InitializePuzzle(SudokuPuzzle_P puzzle)
{
    enum Sudoku_RC_E ret = SUDOKU_RC_SUCCESS;
    if(NULL == puzzle) return SUDOKU_RC_ERROR;

    ret = initializeGrid(puzzle);

    if(ret == SUDOKU_RC_SUCCESS)
    {
        ret = initializeRows(puzzle);
    }
    if(ret == SUDOKU_RC_SUCCESS)
    {
        ret = initializeColumns(puzzle);
    }
    if(ret == SUDOKU_RC_SUCCESS)
    {
        ret = initializeSubgrid(puzzle);
    }

    return ret;
}


/* Generate the row mask */
static enum Sudoku_RC_E generateRowMask(SudokuPuzzle_P puzzle, unsigned int row)
{
    uint32_t row_mask = getCellMask(getRowReference(puzzle, row));
    uint32_t row_mask_cand = SUDOKU_MASK_NONE;
    enum Sudoku_RC_E ret = SUDOKU_RC_SUCCESS; /* No candidate */

    for(unsigned int col = 0; col < NUM_COLS; col++)
    {
        int value = GetCellValueInPuzzle(puzzle, row, col);

        if(value != SUDOKU_NO_VALUE)
        {
            row_mask_cand |= getMaskFromValue(value);
        }
    }

    row_mask_cand = (SUDOKU_MASK_ALL & ~row_mask_cand);

    if(row_mask != row_mask_cand)
    {
        (void)setCellMask(getRowReference(puzzle, row), row_mask_cand);
        ret = SUDOKU_RC_PRUNE;
    }

    return ret;
}

/* Generate the column mask */
static enum Sudoku_RC_E generateColumnMask(SudokuPuzzle_P puzzle, unsigned int col)
{
    uint32_t old_mask = getCellMask(getColumnReference(puzzle, col));
    uint32_t new_mask = SUDOKU_MASK_NONE;
    enum Sudoku_RC_E ret = SUDOKU_RC_SUCCESS; /* No candidate */

    for(unsigned int row=0; row < NUM_ROWS; row++)
    {
        int value = GetCellValueInPuzzle(puzzle, row, col);

        if(value != SUDOKU_NO_VALUE)
        {
            new_mask |= getMaskFromValue(value);
        }
    }

    new_mask = (SUDOKU_MASK_ALL & ~new_mask);

    if(old_mask != new_mask)
    {
        (void)setCellMask(getColumnReference(puzzle, col), new_mask);
        ret = SUDOKU_RC_PRUNE;
    }

    return ret;
}

static enum Sudoku_RC_E generateSubCellMask(SudokuPuzzle_P puzzle, int sub_row, int sub_col)
{
    uint32_t old_mask = getCellMask(getSubgridReference(puzzle, sub_row, sub_col));
    uint32_t new_mask = SUDOKU_MASK_NONE;
    enum Sudoku_RC_E ret = SUDOKU_RC_SUCCESS; /* No candidate */

    for(unsigned int row = 0; row < NUM_SUBGRID_ROWS; row++)
    {
        for(unsigned int col = 0; col < NUM_SUBGRID_COLS; col++)
        {
            int value = GetCellValueInPuzzle(puzzle, (3*sub_row + row), (3*sub_col + col));

            if(value != SUDOKU_NO_VALUE)
            {
                new_mask |= getMaskFromValue(value);
            }
        }
    }

    new_mask = (SUDOKU_MASK_ALL & ~new_mask);

    if(old_mask != new_mask)
    {
        (void)setCellMask(getSubgridReference(puzzle, sub_row, sub_col), new_mask);
        ret = SUDOKU_RC_PRUNE;
    }

    return ret;
}


static int updateRowMasks(SudokuPuzzle_P p)
{
    int row_changes = 0;

    for(unsigned int row = 0; row < NUM_ROWS; row++)
    {
        int ret = generateRowMask(p, row);
        if(SUDOKU_RC_PRUNE == ret) row_changes++;
        else if (SUDOKU_RC_ERROR == ret) return (int)SUDOKU_RC_ERROR;
    }

    return row_changes;
}

static int updateColumnMasks(SudokuPuzzle_P p)
{
    int col_changes = 0;

    for(unsigned int col = 0; col < NUM_COLS; col++)
    {
        int ret = generateColumnMask(p, col);
        if(SUDOKU_RC_PRUNE == ret) col_changes++;
        else if(SUDOKU_RC_ERROR == ret) return (int)SUDOKU_RC_ERROR;
    }

    return col_changes;
}

static int updateSubGridMasks(SudokuPuzzle_P p)
{
    int sub_col = 0;
    int sub_row = 0;
    int ret;
    int changes = 0;

    for(sub_row = 0; sub_row < NUM_SUBGRID_ROWS; sub_row++)
    {
        for(sub_col = 0; sub_col < NUM_SUBGRID_COLS; sub_col++)
        {
            ret = generateSubCellMask(p, sub_row, sub_col);
            if(SUDOKU_RC_PRUNE == ret) changes++;
            else if(SUDOKU_RC_ERROR == ret) return (int)SUDOKU_RC_ERROR;
        }
    }

    return changes;
}

static int updateGridMasks(SudokuPuzzle_P p)
{
    int row = 0;
    int col = 0;
    int changes = 0;

    /* Generate Grid Element Masks*/
    for(row = 0; row < NUM_ROWS; row++)
    {
        for(col = 0; col < NUM_COLS; col++)
        {
            if(SUDOKU_NO_VALUE == GetCellValueInPuzzle(p, row, col))
            {
                uint32_t old_mask = getCellMask(getCellReference(p, row, col));
                uint32_t col_mask = getCellMask(getColumnReference(p, col));
                uint32_t row_mask = getCellMask(getRowReference(p, row));
                uint32_t sub_mask = getCellMask(getSubgridReference(p, row/3, col/3));

                uint32_t new_cell_mask = (col_mask & row_mask & sub_mask & old_mask) ;

                if(old_mask != new_cell_mask)
                {
                    changes++;
                    setCellMask(getCellReference(p,row,col), new_cell_mask);
                }
            }
        }
    }

    return changes;
}



/* Support Functions */

/* Extract value from mask */
static enum SudokuValues_E getValueFromMask(uint32_t cell_mask)
{
    int value = SUDOKU_INVALID_VALUE;

    if(cell_mask == SUDOKU_MASK_NONE)
    {
        value = SUDOKU_NO_VALUE;
    }
    else if(cell_mask == SUDOKU_MASK_1)
    {
        value = SUDOKU_VALUE_1;
    }
    else if(cell_mask == SUDOKU_MASK_2)
    {
        value = SUDOKU_VALUE_2;
    }
    else if(cell_mask == SUDOKU_MASK_3)
    {
        value = SUDOKU_VALUE_3;
    }
    else if(cell_mask == SUDOKU_MASK_4)
    {
        value = SUDOKU_VALUE_4;
    }
    else if(cell_mask == SUDOKU_MASK_5)
    {
        value = SUDOKU_VALUE_5;
    }
    else if(cell_mask == SUDOKU_MASK_6)
    {
        value = SUDOKU_VALUE_6;
    }
    else if(cell_mask == SUDOKU_MASK_7)
    {
        value = SUDOKU_VALUE_7;
    }
    else if(cell_mask == SUDOKU_MASK_8)
    {
        value = SUDOKU_VALUE_8;
    }
    else if(cell_mask == SUDOKU_MASK_9)
    {
        value = SUDOKU_VALUE_9;
    }
    else
    {
        value = SUDOKU_INVALID_VALUE;
    }

    return value;
}

static uint32_t getFirstCandidate(uint32_t val)
{
    uint32_t ret = 0;

    for(unsigned int i = 0; i < NUM_CANDIDATES; i++)
    {
        if( val & (1<<i)) return (uint32_t)(i+1);
    }

    return ret;
}

static uint32_t getMaskFromValue(int val)
{
    return (1 << (val-1));
}

static int updateMasks(SudokuPuzzle_P p)
{
    int row_changes = 0;
    int col_changes = 0;
    int sub_changes = 0;
    int mask_changes = 0;
    int grid_changes = 0;
    static enum Sudoku_RC_E ret = SUDOKU_RC_SUCCESS;

    /* Generate Row Mask */
    row_changes = updateRowMasks(p);
    if(row_changes < 0)
    {
        ret = SUDOKU_RC_ERROR;
    }

    /* Generate Column Mask */
    if(SUDOKU_RC_SUCCESS == ret)
    {
        col_changes = updateColumnMasks(p);
        if(col_changes < 0)
        {
            ret = SUDOKU_RC_ERROR;
        }
    }

    /* Generate Sub Grid mask */
    if(SUDOKU_RC_SUCCESS == ret)
    {
        sub_changes = updateSubGridMasks(p);
        if(sub_changes < 0)
        {
            ret = SUDOKU_RC_ERROR;
        }
    }

    if(SUDOKU_RC_SUCCESS == ret)
    {
        grid_changes = updateGridMasks(p);
        if(grid_changes < 0)
        {
            ret = SUDOKU_RC_ERROR;
        }
    }

    if(SUDOKU_RC_SUCCESS == ret)
    {
        mask_changes = grid_changes + row_changes + col_changes + sub_changes;
    }
    else
    {
        mask_changes = (int)SUDOKU_RC_ERROR;
    }

    return mask_changes;
}

int pruneGrid(SudokuPuzzle_P p)
{
    int ret = SUDOKU_RC_SUCCESS;
    int grid_changes = 0;

    /* This operation prunes */
    for(unsigned int row = 0; row < NUM_ROWS; row++)
    {
        for(unsigned col = 0; col < NUM_COLS; col++)
        {
            if(SUDOKU_NO_VALUE == GetCellValueInPuzzle(p, row, col))
            {
                enum SudokuValues_E candidate_value = getValueFromMask(getCellMask(getCellReference(p,row,col)));
                if(candidate_value > SUDOKU_NO_VALUE)
                {
                    SetCellValueInPuzzle(p, candidate_value, row, col);
                    grid_changes++;
                }
                else if(0 == getCellMask(getCellReference(p,row,col)))
                {
                    SetCellValueInPuzzle(p, SUDOKU_INVALID_VALUE, row, col);
                    return (int)SUDOKU_RC_ERROR;
                }

            }
        }
    }

    return grid_changes;
}



int pruneCandidates(SudokuPuzzle_P p)
{
    static enum Sudoku_RC_E ret = SUDOKU_RC_SUCCESS;
    int changes = 0;
    int grid_changes = 0;

    /* Generate Row Mask */
    int mask_changes = updateMasks(p);
    if(mask_changes < 0)
    {
        ret = SUDOKU_RC_ERROR;
    }

    if(SUDOKU_RC_SUCCESS == ret)
    {
        grid_changes = pruneGrid(p);
        if(grid_changes < 0)
        {
            ret = SUDOKU_RC_ERROR;
        }
    }

    if(SUDOKU_RC_SUCCESS == ret)
    {
        changes = (grid_changes + mask_changes);
    }
    else
    {
        changes = (int)SUDOKU_RC_ERROR;
    }

    return changes;
}

enum Sudoku_RC_E PrunePuzzle(SudokuPuzzle_P p)
{
    static enum Sudoku_RC_E ret = SUDOKU_RC_SUCCESS;
    int changes  = 0;

    do
    {
        changes = pruneCandidates(p);
    }
    while(changes > 0);

    if(changes < 0)
    {
        ret = SUDOKU_RC_ERROR;

    }
    else if(changes == 0)
    {
        ret = SUDOKU_RC_SUCCESS;
    }

    return ret;
}

struct BackTrackCandidate_S SimpleSelectionStrategy(SudokuPuzzle_P p)
{
    struct BackTrackCandidate_S cand = {0,0,0};

    // Search for values with two or more candidates
    for(unsigned int n_cand = 2; n_cand < NUM_CANDIDATES; n_cand++)
    {
        for(unsigned int row = 0; row < NUM_ROWS; row++)
        {
            for(unsigned int col = 0; col < NUM_COLS; col++)
            {
                if(n_cand == GetCandidatesInCell(p, row, col))
                {

                    cand.val = getFirstCandidate(getCellMask(getCellReference(p,row,col)));
                    cand.col = col;
                    cand.row = row;
                    return cand;
                }
            }
        }
    }

    return cand;
}



enum Sudoku_RC_E Solve(SudokuPuzzle_P p, int level)
{
    enum Sudoku_RC_E rc = SUDOKU_RC_SUCCESS;
    if(NULL == p){
        return SUDOKU_RC_ERROR;
        }


    /* Count Values */
    int count = CountElementsInGrid(p);

    printf("Values in Grid: %d\n\r", count);

    /* Perform Puzzle Pruning */
    rc = PrunePuzzle(p);

    if(SUDOKU_RC_SUCCESS == rc)
    {
        rc = validateGrid(p);
    }

    count = CountElementsInGrid(p);

    printf("Values in Grid: %d\n\r", count);

    return rc;
}




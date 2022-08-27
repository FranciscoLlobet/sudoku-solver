#ifndef SUDOKU_ENGINE_H_INCLUDED
#define SUDOKU_ENGINE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

enum Sudoku_RC_E
{
    SUDOKU_RC_ERROR = -1,
    SUDOKU_RC_SUCCESS = 0,
    SUDOKU_RC_PRUNE = 1,
};


struct SudokuCell_S
{
    int value;
    enum CellType_E cell_type;
    uint32_t bit_value;     // Value as Bitmask (?)
    uint32_t candidates;    // Candidate Bitmask
    uint32_t n_candidate;   // Number of candidates
    uint32_t score;         // Score
};


struct SudokuPuzzle_S
{
    struct SudokuCell_S grid[NUM_ROWS][NUM_COLS]; /* Main Grid */
    struct SudokuCell_S row[NUM_ROWS]; /* Row Candidates */
    struct SudokuCell_S col[NUM_COLS]; /* Column Candidates */
    struct SudokuCell_S sub[NUM_SUBGRID_ROWS][NUM_SUBGRID_COLS]; /* Subgrids*/
};


struct BackTrackCandidate_S
{
    enum SudokuValues_E val;
    unsigned int row;
    unsigned int col;
};




/* Initialize an empty puzzle object */
enum Sudoku_RC_E InitializePuzzle(SudokuPuzzle_P puzzle);

enum SudokuValues_E SetCellValueInPuzzle(SudokuPuzzle_P puzzle, enum SudokuValues_E val, unsigned int row, unsigned int col);
enum SudokuValues_E GetCellValueInPuzzle(SudokuPuzzle_P puzzle, unsigned int row, unsigned int col);
int GetCandidatesInCell(SudokuPuzzle_P p, unsigned int row, unsigned int col);

enum Sudoku_RC_E PrunePuzzle(SudokuPuzzle_P p);
int pruneCandidates(SudokuPuzzle_P p);


int Solve(SudokuPuzzle_P p, int level);


#ifdef __cplusplus
}
#endif

#endif // SUDOKU_ENGINE_H_INCLUDED

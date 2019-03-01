void createSolution(const Sudoku *sudoku, Solution *solution);
FillNoteStatus fillNote(SolutionBlock **blocks);
FlushNoteStatus flushNote(CheckList *list, short exist);
void solutionToSudoku(Solution *solution, Sudoku *sudoku);
void destroySolution(Solution *solution);
SolutionStatus solveSudoku(const Sudoku *before, SudokuList *afterList);

#include "create-solution.cpp"
#include "fill-note.cpp"
#include "solution-to-sudoku.cpp"
#include "destroy-solution.cpp"
#include "flush-note.cpp"
#include "solve-sudoku.cpp"
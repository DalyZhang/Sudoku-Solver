InputStatus inputSudoku(Sudoku *sudoku);
void outputSudoku(Sudoku *sudoku, OutputMode mode);
Subset createNonEmptySubset(int elementCount);
void destorySudokuList(SudokuList *list);
char checkSudoku(Sudoku *sudoku);

#include "input-sudoku.cpp"
#include "create-non-empty-subset.cpp"
#include "output-sudoku.cpp"
#include "destroy-sudoku-list.cpp"
#include "solve-sudoku/index.cpp"
#include "check-sudoku.cpp"
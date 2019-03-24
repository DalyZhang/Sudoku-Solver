class SudokuSolver {
public:
	enum Mode {M_NORMAL, M_COUNT_ONLY};
};

#include "sudoku-filter.cpp"
#include "sudoku-backtracker.cpp"

class SudokuSolver1 : public SudokuSolver {
public:
	static SudokuSolution *solve(Sudoku &sudoku, Mode mode = M_NORMAL) {
		return SudokuBacktracker::solve(sudoku, mode, -1, SudokuFilter::solve);
	}
};

class SudokuSolver2 : public SudokuSolver {
public:
	static SudokuSolution *solve(Sudoku &sudoku, Mode mode = M_NORMAL) {
		return SudokuBacktracker::solve(sudoku, mode);
	}
};

class SudokuSolver3 : public SudokuSolver {
public:
	static SudokuSolution *solve(Sudoku &sudoku, Mode mode = M_NORMAL) {
		return SudokuFilter::solve(sudoku, mode);
	}
};
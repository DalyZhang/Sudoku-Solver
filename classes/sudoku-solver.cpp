class SudokuSolver {
public:
	enum Mode {M_ALL, M_FIRST};
};

#include "sudoku-backtracker.cpp"
#include "sudoku-filter.cpp"

class SudokuSolver1 : public SudokuSolver {
public:
	static SudokuSolution *solve(Sudoku &sudoku, Mode mode = M_ALL) {
		return SudokuBacktracker::solve(sudoku, mode, SudokuFilter::solve);
	}
};

class SudokuSolver2 : public SudokuSolver {
public:
	static SudokuSolution *solve(Sudoku &sudoku, Mode mode = M_ALL) {
		return SudokuBacktracker::solve(sudoku, mode);
	}
};

class SudokuSolver3 : public SudokuSolver {
public:
	static SudokuSolution *solve(Sudoku &sudoku, Mode mode = M_ALL) {
		return SudokuFilter::solve(sudoku);
	}
};
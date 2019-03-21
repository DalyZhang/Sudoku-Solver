class SudokuBacktracker : public SudokuSolver {
public:
	typedef SudokuSolution *(*Filter)(Sudoku &, Mode);
private:
	static int side;
	static int sideSquare;
	static Mode mode;
	static Filter filter;
	static void copyAndPushSudokuToSolution(SudokuSolution &solution, Sudoku &sudoku);
	static void backtrack(Sudoku *sudoku, SudokuSolution &solution, int blockOffset);
public:
	static SudokuSolution *solve(Sudoku &sudoku, Mode mode = M_ALL, Filter filter = nullptr);
};

int SudokuBacktracker::side;

int SudokuBacktracker::sideSquare;

SudokuBacktracker::Mode SudokuBacktracker::mode;

SudokuBacktracker::Filter SudokuBacktracker::filter;

void SudokuBacktracker::copyAndPushSudokuToSolution(SudokuSolution &solution, Sudoku &sudoku) {
	switch (mode) {
	case M_ALL: case M_FIRST:
		solution.push(Sudoku::deepCopyCreate(sudoku));
		break;
	case M_COUNT_ALL: case M_COUNT_FIRST:
		solution.pseudoCount++;
		break;
	}
}

void SudokuBacktracker::backtrack(Sudoku *sudoku, SudokuSolution &solution, int blockOffset) {

	Sudoku *checkPoint = nullptr;
	SudokuSolution *filterSolution = nullptr;
	if (filter != nullptr) {
		bool toBacktrack;
		filterSolution = filter(*sudoku, M_ALL);
		switch (filterSolution->status) {
		case SudokuSolution::S_SUCCESS:
			solution.status = SudokuSolution::S_SUCCESS;
			copyAndPushSudokuToSolution(solution, **filterSolution->begin());
			// solution.push(Sudoku::deepCopyCreate(**filterSolution->begin()));
			toBacktrack = false;
			break;
		case SudokuSolution::S_ERROR:
			toBacktrack = false;
			break;
		case SudokuSolution::S_UNFINISHED:
			checkPoint = sudoku;
			sudoku = *filterSolution->begin();
			toBacktrack = true;
			break;
		}
		if (!toBacktrack) {
			delete filterSolution;
			filterSolution = nullptr;
			return;
		}
	}

	int i1, i2, i3, i4;

	for (i1 = blockOffset; i1 < sideSquare; i1++) {
		i2 = i1 / side;
		i3 = i1 % side;
		if (sudoku->getValue(i2, i3) != 0) {
			continue;
		}
		for (i4 = 1; i4 <= side; i4++) {
			if (sudoku->checkBlock(i2, i3, i4)) {
				sudoku->setValue(i2, i3, i4);
				backtrack(sudoku, solution, i1 + 1);
				if ((mode == M_FIRST || mode == M_COUNT_FIRST) && solution.status == SudokuSolution::S_SUCCESS) {
					break;
				}
			}
		}
		if (filter != nullptr) {
			sudoku = checkPoint;
			delete filterSolution;
			filterSolution = nullptr;
		} else {
			sudoku->setValue(i2, i3, 0);
		}
		break;
	}

	if (i1 == sideSquare) {
		solution.status = SudokuSolution::S_SUCCESS;
		copyAndPushSudokuToSolution(solution, *sudoku);
	}

}

SudokuSolution *SudokuBacktracker::solve(Sudoku &sudoku, Mode mode, Filter filter) {
	Timer timer;
	timer.start();
	int order = sudoku.getOrder();
	side = order * order;
	sideSquare = side * side;
	SudokuBacktracker::mode = mode;
	SudokuBacktracker::filter = filter;
	SudokuSolution *solution = new SudokuSolution;
	if (sudoku.check()) {
		backtrack(&sudoku, *solution, 0);
	}
	solution->time = timer.stop();
	return solution;
}
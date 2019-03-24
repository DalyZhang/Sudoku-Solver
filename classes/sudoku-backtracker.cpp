class SudokuBacktracker : public SudokuSolver {
public:
	typedef SudokuSolution *(*Filter)(Sudoku &, Mode, int, SudokuFilter::FlushNoteConfig *);
private:
	static int side;
	static int sideSquare;
	static Mode mode;
	static int solutionCountMax;
	static Filter filter;
	static int filterConfigCount;
	static SudokuFilter::FlushNoteConfig *filterConfigs;
	static void copyAndPushSudokuToSolution(SudokuSolution &solution, Sudoku &sudoku);
	static void backtrack(Sudoku *sudoku, SudokuSolution &solution, int blockOffset);
public:
	static SudokuSolution *solve(Sudoku &sudoku, Mode mode = M_NORMAL, int solutionCountMax = -1, Filter filter = nullptr);
	static SudokuSolution *solve(Sudoku &sudoku, Mode mode, int solutionCountMax, Filter filter,
		int configCount, SudokuFilter::FlushNoteConfig *configs);
};

int SudokuBacktracker::side;

int SudokuBacktracker::sideSquare;

SudokuBacktracker::Mode SudokuBacktracker::mode;

int SudokuBacktracker::solutionCountMax;

SudokuBacktracker::Filter SudokuBacktracker::filter;

int SudokuBacktracker::filterConfigCount;

SudokuFilter::FlushNoteConfig *SudokuBacktracker::filterConfigs = nullptr;

void SudokuBacktracker::copyAndPushSudokuToSolution(SudokuSolution &solution, Sudoku &sudoku) {
	switch (mode) {
	case M_NORMAL:
		solution.push(Sudoku::deepCopyCreate(sudoku));
		break;
	case M_COUNT_ONLY:
		solution.pseudoCount++;
		break;
	}
}

void SudokuBacktracker::backtrack(Sudoku *sudoku, SudokuSolution &solution, int blockOffset) {

	Sudoku *checkPoint = nullptr;
	SudokuSolution *filterSolution = nullptr;
	if (filter != nullptr) {
		bool toBacktrack;
		filterSolution = filter(*sudoku, M_NORMAL, filterConfigCount, filterConfigs);
		switch (filterSolution->status) {
		case SudokuSolution::S_SUCCESS:
			solution.status = SudokuSolution::S_SUCCESS;
			copyAndPushSudokuToSolution(solution, **filterSolution->begin());
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
				if (solution.status == SudokuSolution::S_SUCCESS && solutionCountMax != -1
					&& solution.getCount() >= solutionCountMax) {
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

SudokuSolution *SudokuBacktracker::solve(Sudoku &sudoku, Mode mode, int solutionCountMax, Filter filter) {
	static SudokuFilter::FlushNoteConfig configs[] = {{false, 1}};
	return solve(sudoku, mode, solutionCountMax, filter, sizeof (configs) / sizeof (configs[0]), configs);
}

SudokuSolution *SudokuBacktracker::solve(Sudoku &sudoku, Mode mode, int solutionCountMax, Filter filter,
	int configCount, SudokuFilter::FlushNoteConfig *configs) {
	Timer timer;
	timer.start();
	int order = sudoku.getOrder();
	side = order * order;
	sideSquare = side * side;
	SudokuBacktracker::mode = mode;
	SudokuBacktracker::solutionCountMax = solutionCountMax > 0 ? solutionCountMax : -1;
	SudokuBacktracker::filter = filter;
	filterConfigCount = configCount;
	filterConfigs = configs;
	SudokuSolution *solution = new SudokuSolution;
	if (sudoku.check()) {
		backtrack(&sudoku, *solution, 0);
	}
	solution->time = timer.stop();
	return solution;
}
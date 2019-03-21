class SudokuSolution {
public:
	enum Status {S_SUCCESS, S_ERROR, S_UNFINISHED};
private:
	int count = 0;
	Sudoku **sudokus = nullptr;
public:
	int pseudoCount = 0;
	Status status = S_ERROR;
	double time;
	long long tryTimes;
	SudokuSolution();
	~SudokuSolution();
	void push(Sudoku *sudoku);
	int getCount();
	Sudoku **begin();
	Sudoku **end();
};

SudokuSolution::SudokuSolution() {
	sudokus = new Sudoku *[1]{};
}

SudokuSolution::~SudokuSolution() {
	for (int i1 = 0; i1 < count; i1++) {
		delete sudokus[i1];
	}
	delete[] sudokus;
	sudokus = nullptr;
}

void SudokuSolution::push(Sudoku *sudoku) {
	Sudoku **oldSudokus = sudokus;
	sudokus = new Sudoku *[count + 2]{};
	memcpy(sudokus, oldSudokus, sizeof (Sudoku *) * count);
	delete[] oldSudokus;
	sudokus[count] = sudoku;
	count++;
}

int SudokuSolution::getCount() {
	if (pseudoCount == 0) {
		return count;
	} else {
		return pseudoCount;
	}
}

Sudoku **SudokuSolution::begin() {
	return &sudokus[0];
}

Sudoku **SudokuSolution::end() {
	return &sudokus[count];
}
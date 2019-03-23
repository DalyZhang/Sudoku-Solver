class SudokuFilter : public SudokuSolver {
public:
	enum AreaType {AT_ROW, AT_COLUMN, AT_SUBGRID};
	enum FlushNoteStatus {FNS_CONTINUE, FNS_ERROR, FNS_RESET};
private:
	struct ScannerNode {
		int y, x;
		ScannerNode *prev = nullptr, *next = nullptr;
	};
	class Scanner {
	private:
		int count;
		ScannerNode sentinel;
	public:
		Scanner();
		~Scanner();
		void push(int y, int x);
		bool remove(ScannerNode *&node);
		ScannerNode *begin();
		ScannerNode *end();
		int getCount() const;
	};
	typedef FlushNoteStatus (*FlushNote)(short);
	static Mode mode;
	static int order;
	static int side;
	static Scanner *scanners[3];
	static Sudoku *filledSudoku;
	static Scanner *negativeScanners[3];
	static Sudoku **negativeSudokus;
	static FlushNote flushNote;
	static int elementCount;
	static int existCountMax;
	static AreaType scannerAreaType;
	static int scannerAreaPos;
	static void pushSudokuToSolutionOrDelete(SudokuSolution &solution, Sudoku &sudoku);
	static void createNegativeSudokus();
	static void destroyNegativeSudokus();
	static void filter(Sudoku &sudoku, SudokuSolution &solution);
	static FlushNoteStatus forEachSubsetInvokeFlush(int subset, int currentPos, int currentExistCount);
	static FlushNoteStatus flushNotePositivily(short subset);
	static FlushNoteStatus flushNoteNegativily(short subset);
public:
	static SudokuSolution *solve(Sudoku &sudoku, Mode mode = M_ALL);
};

SudokuFilter::Scanner::Scanner() {
	count = 0;
	sentinel = {0, 0, &sentinel, &sentinel};
}

SudokuFilter::Scanner::~Scanner() {
	ScannerNode *snp1 = sentinel.next;
	while (snp1 != &sentinel) {
		snp1 = snp1->next;
		delete snp1->prev;
	}
}

void SudokuFilter::Scanner::push(int y, int x) {
	sentinel.prev = sentinel.prev->next
		= new ScannerNode{y, x, sentinel.prev, &sentinel};
	count++;
}

bool SudokuFilter::Scanner::remove(ScannerNode *&node) {
	if (count == 0 || node == &sentinel) {
		return false;
	}
	node->prev->next = node->next;
	node->next->prev = node->prev;
	ScannerNode *snp1 = node;
	node = node->prev;
	delete snp1;
	count--;
	return true;
}

SudokuFilter::ScannerNode *SudokuFilter::Scanner::begin() {
	return sentinel.next;
}

SudokuFilter::ScannerNode *SudokuFilter::Scanner::end() {
	return &sentinel;
}

int SudokuFilter::Scanner::getCount() const {
	return count;
}

SudokuFilter::Mode SudokuFilter::mode;

int SudokuFilter::order;

int SudokuFilter::side;

SudokuFilter::Scanner *SudokuFilter::scanners[3] = {};

Sudoku *SudokuFilter::filledSudoku = nullptr;

SudokuFilter::Scanner *SudokuFilter::negativeScanners[3] = {};

Sudoku **SudokuFilter::negativeSudokus = nullptr;

SudokuFilter::FlushNote SudokuFilter::flushNote = nullptr;

int SudokuFilter::elementCount;

int SudokuFilter::existCountMax;

SudokuFilter::AreaType SudokuFilter::scannerAreaType;

int SudokuFilter::scannerAreaPos;

void SudokuFilter::pushSudokuToSolutionOrDelete(SudokuSolution &solution, Sudoku &sudoku) {
	switch (mode) {
	case M_ALL: case M_FIRST:
		solution.push(&sudoku);
		break;
	case M_COUNT_ALL: case M_COUNT_FIRST:
		delete &sudoku;
		solution.pseudoCount++;
		break;
	}
}

void SudokuFilter::createNegativeSudokus() {

	int i1, i2, i3, i4, i5, i6;
	short s1;
	ScannerNode *snp1 = nullptr;

	negativeSudokus = new Sudoku *[3]{};
	for (i1 = 0; i1 < 3; i1++) {
		negativeSudokus[i1] = new Sudoku(order);
	}

	for (i1 = 0; i1 < side; i1++) {
		for (i2 = 0; i2 < side; i2++) {
			for (i3 = 0, s1 = filledSudoku->getNote(i1, i2); i3 < side; i3++, s1 >>= 1) {
				if (s1 & 1) {
					i4 = i1 / order * order + i3 / order;
					i5 = i2 / order * order + i3 % order;
					i6 = i1 % order * order + i2 % order;
					negativeSudokus[AT_ROW]->getNote(i1, i3) |= 1 << i2;
					negativeSudokus[AT_COLUMN]->getNote(i3, i2) |= 1 << i1;
					negativeSudokus[AT_SUBGRID]->getNote(i4, i5) |= 1 << i6;
				}
			}
		}
	}

	// negativeSudokus[0]->write(Sudoku::WM_IMAGE);
	// negativeSudokus[1]->write(Sudoku::WM_IMAGE);
	// negativeSudokus[2]->write(Sudoku::WM_IMAGE);

}

void SudokuFilter::destroyNegativeSudokus() {
	for (int i1 = 0; i1 < 3; i1++) {
		delete negativeSudokus[i1];
	}
	delete[] negativeSudokus;
	negativeSudokus = nullptr;
}

void SudokuFilter::filter(Sudoku &sudoku, SudokuSolution &solution) {

	int i1, i2, i3;

	filledSudoku = Sudoku::deepCopyCreate(sudoku);
	filledSudoku->fillNote();
	createNegativeSudokus();

	Scanner scannerStorage[3][side], negativeScannerStorage[3][side];
	for (i1 = 0; i1 < 3; i1++) {
		scanners[i1] = scannerStorage[i1];
		negativeScanners[i1] = negativeScannerStorage[i1];
	}
	for (i1 = 0; i1 < side; i1++) {
		for (i2 = 0; i2 < side; i2++) {
			i3 = i1 / order * order + i2 / order;
			if (sudoku.getValue(i1, i2) == 0) {
				scanners[AT_ROW][i1].push(i1, i2);
				scanners[AT_COLUMN][i2].push(i1, i2);
				scanners[AT_SUBGRID][i3].push(i1, i2);
			}
			if (negativeSudokus[AT_ROW]->getNote(i1, i2) != 0) {
				negativeScanners[AT_ROW][i1].push(i1, i2);
			}
			if (negativeSudokus[AT_COLUMN]->getNote(i1, i2) != 0) {
				negativeScanners[AT_COLUMN][i2].push(i1, i2);
			}
			if (negativeSudokus[AT_SUBGRID]->getNote(i1, i2) != 0) {
				negativeScanners[AT_SUBGRID][i3].push(i1, i2);
			}
		}
	}

	// printf("r: %i, c: %i, s: %i\n", negativeScanners[AT_ROW][0].getCount(),
	// 	negativeScanners[AT_COLUMN][0].getCount(), negativeScanners[AT_SUBGRID][0].getCount());
	// exit(0);

	Sudoku *checkPoint = Sudoku::deepCopyCreate(*filledSudoku);
	ScannerNode *snPtr = nullptr, *snEnd = nullptr;
	short subset;
	FlushNoteStatus flushNoteStatus;
	bool finished, error = false;
	while (true) {
		for (i1 = 0; i1 < 3 && !error; i1++) {
			for (i2 = 0; i2 < side; i2++) {
				elementCount = scanners[i1][i2].getCount();
				existCountMax = elementCount;
				scannerAreaType = (AreaType)i1;
				scannerAreaPos = i2;
				flushNote = flushNotePositivily;
				flushNoteStatus = forEachSubsetInvokeFlush(0, 0, 0);
				if (flushNoteStatus == FNS_RESET) {
					i2--;
					continue;
				}
				if (flushNoteStatus == FNS_ERROR) {
					error = true;
					break;
				}
			}
		}
		if (error) {
			solution.status = SudokuSolution::S_ERROR;
			delete filledSudoku;
			break;
		}
		for (i1 = 0, finished = true; i1 < side && finished; i1++) {
			for (i2 = 0; i2 < side; i2++) {
				if (filledSudoku->getValue(i1, i2) == 0) {
					finished = false;
					break;
				}
			}
		}
		if (finished) {
			pushSudokuToSolutionOrDelete(solution, *filledSudoku);
			solution.status = SudokuSolution::S_SUCCESS;
			break;
		}
		if (Sudoku::compare(*checkPoint, *filledSudoku)) {
			pushSudokuToSolutionOrDelete(solution, *filledSudoku);
			solution.status = SudokuSolution::S_UNFINISHED;
			break;
		}
		Sudoku::deepCopyAssign(*checkPoint, *filledSudoku);
	}

	destroyNegativeSudokus();
	delete checkPoint;

}

SudokuFilter::FlushNoteStatus SudokuFilter::forEachSubsetInvokeFlush(int subset, int currentPos, int currentExistCount) {

	FlushNoteStatus flushNoteStatus;
	short newSubset;

	while (currentPos < elementCount) {
		newSubset = subset | (1 << currentPos);
		if (newSubset != 0) {
			flushNoteStatus = flushNote(newSubset);
			if (flushNoteStatus != FNS_CONTINUE) {
				return flushNoteStatus;
			}
		}
		if (currentExistCount + 1 < existCountMax) {
			flushNoteStatus = forEachSubsetInvokeFlush(newSubset, currentPos + 1, currentExistCount + 1);
			if (flushNoteStatus != FNS_CONTINUE) {
				return flushNoteStatus;
			}
		}
		currentPos++;
	}

	return FNS_CONTINUE;

}

SudokuFilter::FlushNoteStatus SudokuFilter::flushNotePositivily(short subset) {

	int i1, i2, i3, i4;
	short s1;
	ScannerNode *snp1 = nullptr;

	Scanner &scanner = scanners[scannerAreaType][scannerAreaPos];

	/**
	 * existCount -> the number of blocks selected by `subset`
	 * 
	 * collection -> the boolean list designating the existing
	 * -> note numbers in selected blocks
	 */
	int existCount = 0;
	short collection = 0;
	snp1 = scanner.begin();
	for (i1 = 0, s1 = subset; i1 < scanner.getCount(); i1++, s1 >>= 1) {
		if (s1 & 1) {
			existCount++;
			collection |= filledSudoku->getNote(snp1->y, snp1->x);
		}
		snp1 = snp1->next;
	}

	/**
	 * numCount -> the number of different note numbers in
	 * -> selected blocks
	 */
	int numCount = 0;
	for (i1 = 0, s1 = collection; i1 < side; i1++, s1 >>= 1) {
		if (s1 & 1) {
			numCount++;
		}
	}

	/**
	 * flush note numbers of unselected blocks when `existCount`
	 * -> = `numCount`
	 */
	if (existCount != numCount) {
		return FNS_CONTINUE;
	}
	
	/**
	 * existBlockList -> the array of pointers pointing to
	 * -> selected blocks
	 */
	int existBlockList[existCount][2];
	snp1 = scanner.begin();
	for (i1 = i2 = 0, s1 = subset; i1 < scanner.getCount(); i1++, s1 >>= 1) {
		if (s1 & 1) {
			existBlockList[i2][0] = snp1->y;
			existBlockList[i2][1] = snp1->x;
			i2++;
		}
		snp1 = snp1->next;
	}

	/**
	 * sameArea -> the array of boolean vars which designating if
	 * -> all selected blocks are in the same row, column, subgrid
	 */
	bool sameArea[3] = {true, true, true};
	int sameAreaPos[3] = {-1, -1, -1};
	snp1 = scanner.begin();
	for (i1 = 0, s1 = subset; i1 < scanner.getCount(); i1++, s1 >>= 1) {
		if (s1 & 1) {
			if (sameAreaPos[AT_ROW] != -1 && sameAreaPos[AT_ROW] != snp1->y) {
				sameArea[AT_ROW] = false;
			}
			if (sameAreaPos[AT_COLUMN] != -1 && sameAreaPos[AT_COLUMN] != snp1->x) {
				sameArea[AT_COLUMN] = false;
			}
			i2 = snp1->y / order * order + snp1->x / order;
			if (sameAreaPos[AT_SUBGRID] != -1 && sameAreaPos[AT_SUBGRID] != i2) {
				sameArea[AT_SUBGRID] = false;
			}
			sameAreaPos[AT_ROW] = snp1->y;
			sameAreaPos[AT_COLUMN] = snp1->x;
			sameAreaPos[AT_SUBGRID] = i2;
		}
		snp1 = snp1->next;
	}

	/**
	 * flush note numbers of unselected blocks
	 * 
	 * areaScanner -> the list where note numbers should be flushed
	 * 
	 * restNoteNumCount -> the var temporarily storing the number of note
	 * -> numbers in a flushed block
	 * 
	 * lastNum -> the var temporarily recording one of the note number
	 * -> in a flushed block, it is specially the last one due to the
	 * -> implementation
	 * 
	 * areaSubset -> the boolean list designating where selected blocks
	 * -> exist in the `areaScanner`
	 * 
	 * status -> the var temporarily storing the flush status
	 */
	Scanner *areaScanner = nullptr;
	int restNoteNumCount;
	int lastNum;
	short areaSubset;
	FlushNoteStatus status = FNS_CONTINUE;
	for (i1 = 0; i1 < 3; i1++) {

		/**
		 * start flushing when all selected blocks are in the same
		 * -> row, column, subgrid
		 */
		if (!sameArea[i1]) {
			continue;
		}

		/**
		 * indicate the flushed list by one of the selected blocks
		 */
		areaScanner = &scanners[i1][sameAreaPos[i1]];

		/**
		 * calculate the value of `areaSubset`
		 */
		if (scannerAreaType == i1) {
			areaSubset = subset;
		} else {
			areaSubset = 0;
			snp1 = areaScanner->begin();
			for (i2 = 0; i2 < areaScanner->getCount(); i2++) {
				for (i3 = 0; i3 < existCount; i3++) {
					if (snp1->y == existBlockList[i3][0] && snp1->x == existBlockList[i3][1]) {
						areaSubset |= 1 << i2;
						break;
					}
				}
				snp1 = snp1->next;
			}
		}

		/**
		 * start flushing on the unselected blocks, delete note
		 * -> numbers existing in the collection
		 * 
		 * if a block has no note numbers after flushing, it
		 * -> cannot be marked with any value number and the sudoku
		 * -> is self-contradictory
		 */
		snp1 = areaScanner->begin();
		for (i2 = 0; i2 < areaScanner->getCount(); i2++) {
			if (!(areaSubset & (1 << i2))) {
				if ((filledSudoku->getNote(snp1->y, snp1->x) &= ~collection) == 0) {
					return FNS_ERROR;
				}
			}
			snp1 = snp1->next;
		}
		
		/**
		 * remove and mark the specific blocks
		 */
		snp1 = areaScanner->begin();
		for (i2 = 0; snp1 != areaScanner->end(); i2++, snp1 = snp1->next) {

			if (areaSubset & (1 << i2)) {

				/**
				 * calculate the value of `lastNum` and `restNoteNumCount`
				 */
				for (i3 = i4 = 0, s1 = filledSudoku->getNote(snp1->y, snp1->x); i3 < side; i3++, s1 >>= 1) {
					if (s1 & 1) {
						i4++;
						lastNum = i3 + 1;
					}
				}
				restNoteNumCount = i4;

				/**
				 * `restNoteNumCount` = 1 indicates that the block has only one
				 * -> note number, means that the note number is the only
				 * -> possible value number of the block, in this case, 
				 * -> the block should be marked with the only note number
				 * -> and removed from the scanner
				 */
				if (restNoteNumCount == 1 && existCount == 1) {
					if (scannerAreaType == i1) {
						status = FNS_RESET;
					}
					filledSudoku->setValue(snp1->y, snp1->x, lastNum);
					areaScanner->remove(snp1);
					continue;
				}

			}

		}

	}

	return status;

}

SudokuFilter::FlushNoteStatus SudokuFilter::flushNoteNegativily(short subset) {



}

SudokuSolution *SudokuFilter::solve(Sudoku &sudoku, Mode mode) {
	Timer timer;
	timer.start();
	SudokuFilter::mode = mode;
	order = sudoku.getOrder();
	side = order * order;
	SudokuSolution *solution = new SudokuSolution;
	if (sudoku.check()) {
		filter(sudoku, *solution);
	}
	solution->time = timer.stop();
	return solution;
}
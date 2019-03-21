class SudokuFilter : public SudokuSolver {
public:
	enum AreaType {AT_ROW, AT_COLUMN, AT_SUBGRID};
	enum FillValueStatus {FVS_CONTINUE, FVS_ERROR, FVS_RESET};
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
	static void filter(Sudoku &sudoku, SudokuSolution &solution);
	static FillValueStatus fillValue(Sudoku &sudoku, short subset, Scanner **scanners, AreaType areaType, int areaPos);
public:
	static SudokuSolution *solve(Sudoku &sudoku);
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

void SudokuFilter::filter(Sudoku &sudoku, SudokuSolution &solution) {

	int i1, i2, i3;

	int order = sudoku.getOrder();
	int side = order * order;
	int sideSquare = side * side;

	Scanner scanners[3][side], *scannersPtr[3];
	for (i1 = 0; i1 < side; i1++) {
		for (i2 = 0; i2 < side; i2++) {
			i3 = i1 / order * order + i2 / order;
			if (sudoku.getValue(i1, i2) == 0) {
				scanners[AT_ROW][i1].push(i1, i2);
				scanners[AT_COLUMN][i2].push(i1, i2);
				scanners[AT_SUBGRID][i3].push(i1, i2);
			}
		}
	}
	for (i1 = 0; i1 < 3; i1++) {
		scannersPtr[i1] = scanners[i1];
	}

	Sudoku *filledSudoku = Sudoku::deepCopyCreate(sudoku);
	filledSudoku->fillNote();
	Sudoku *checkPoint = Sudoku::deepCopyCreate(*filledSudoku);
	ScannerNode *snPtr = nullptr, *snEnd = nullptr;
	short subset;
	FillValueStatus fillValueStatus;
	bool finished, error = false;
	while (true) {
		for (i1 = 0; i1 < 3 && !error; i1++) {
			for (i2 = 0; i2 < side && !error; i2++) {
				i3 = 1 << scanners[i1][i2].getCount();
				for (subset = 1; subset < i3; subset++) {
					fillValueStatus = fillValue(*filledSudoku, subset, scannersPtr, (AreaType)i1, i2);
					if (fillValueStatus == FVS_RESET) {
						i2--;
						break;
					}
					if (fillValueStatus == FVS_ERROR) {
						error = true;
						break;
					}
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
			solution.push(filledSudoku);
			solution.status = SudokuSolution::S_SUCCESS;
			break;
		}
		if (Sudoku::compare(*checkPoint, *filledSudoku)) {
			solution.push(filledSudoku);
			solution.status = SudokuSolution::S_UNFINISHED;
			break;
		}
		Sudoku::deepCopyAssign(*checkPoint, *filledSudoku);
	}

	delete checkPoint;

}

SudokuFilter::FillValueStatus SudokuFilter::fillValue(Sudoku &sudoku, short subset,
	Scanner **scanners, AreaType areaType, int areaPos) {

	int i1, i2, i3, i4;
	short s1;
	ScannerNode *snp1 = nullptr;

	int order = sudoku.getOrder();
	int side = order * order;
	Scanner &scanner = scanners[areaType][areaPos];

	/**
	 * existCount -> the number of blocks selected by `subset`
	 * 
	 * collection -> the boolean list designating the existing
	 * -> note numbers in selected blocks
	 */
	int existCount;
	short collection = 0;
	snp1 = scanner.begin();
	for (i1 = i2 = 0, s1 = subset; i1 < scanner.getCount(); i1++, s1 >>= 1) {
		if (s1 & 1) {
			i2++;
			collection |= sudoku.getNote(snp1->y, snp1->x);
		}
		snp1 = snp1->next;
	}
	existCount = i2;

	/**
	 * numCount -> the number of different note numbers in
	 * -> selected blocks
	 */
	for (i1 = 0, i2 = 0, s1 = collection; i1 < side; i1++, s1 >>= 1) {
		if (s1 & 1) {
			i2++;
		}
	}
	int numCount = i2;
	
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
	 * flush note numbers of unselected blocks when `existCount`
	 * -> = `numCount`
	 */
	if (existCount != numCount) {
		return FVS_CONTINUE;
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
	FillValueStatus status = FVS_CONTINUE;
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
		if (areaType == i1) {
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
		 */
		snp1 = areaScanner->begin();
		for (i2 = 0; i2 < areaScanner->getCount(); i2++) {
			if (!(areaSubset & (1 << i2))) {
				sudoku.getNote(snp1->y, snp1->x) &= ~collection;
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
				for (i3 = i4 = 0, s1 = sudoku.getNote(snp1->y, snp1->x); i3 < side; i3++, s1 >>= 1) {
					if (s1 & 1) {
						i4++;
						lastNum = i3 + 1;
					}
				}
				restNoteNumCount = i4;

				/**
				 * `restNoteNumCount` = 0 indicates that the block has no note
				 * -> numbers, means that the block cannot be marked with any
				 * -> value number and the sudoku is self-contradictory
				 * 
				 * `restNoteNumCount` = 1 indicates that the block has only one
				 * -> note number, means that the note number is the only
				 * -> possible value number of the block, in this case, 
				 * -> the block should be marked with the only note number
				 * -> and removed from the scanner
				 */
				if (restNoteNumCount == 0) {
					return FVS_ERROR;
				} else if (restNoteNumCount == 1) {
					if (areaType == i1) {
						status = FVS_RESET;
					}
					sudoku.setValue(snp1->y, snp1->x, lastNum);
					areaScanner->remove(snp1);
					continue;
				}

			}

		}

	}

	return status;

}

SudokuSolution *SudokuFilter::solve(Sudoku &sudoku) {
	Timer timer;
	timer.start();
	SudokuSolution *solution = new SudokuSolution;
	if (sudoku.check()) {
		filter(sudoku, *solution);
	}
	solution->time = timer.stop();
	return solution;
}
class SudokuFilter : public SudokuSolver {
public:
	enum AreaType {AT_ROW, AT_COLUMN, AT_SUBGRID};
	enum FlushNoteStatus {FNS_CONTINUE, FNS_ERROR, FNS_RESET};
	struct FlushNoteConfig {
		bool isPositive;
		int strength;	
	};
private:
	struct ScannerNode {
		int y, x;
		int blockPos;
		ScannerNode *prev = nullptr, *next = nullptr;
	};
	class Scanner {
	private:
		int count;
		ScannerNode sentinel;
	public:
		Scanner();
		~Scanner();
		void push(int y, int x, int blockPos);
		bool remove(ScannerNode *&node);
		ScannerNode *begin();
		ScannerNode *end();
		int getCount() const;
	};
	static Mode mode;
	static int order;
	static int side;
	static int flushNoteConfigCount;
	static FlushNoteConfig *flushNoteConfigs;
	static Scanner *scanners[3];
	static Sudoku *filledSudoku;
	static int elementCount;
	static int existCountMax;
	static AreaType scannerAreaType;
	static int scannerAreaPos;
	static bool isPositiveFlush;
	static short *negativeNotes;
	static short negativeValueExists;
	static void pushSudokuToSolutionOrDelete(SudokuSolution &solution, Sudoku &sudoku);
	static void filter(Sudoku &sudoku, SudokuSolution &solution);
	static FlushNoteStatus startFlush(AreaType areaType, int areaPos, bool isPositive = true);
	static FlushNoteStatus startFlush(AreaType areaType, int areaPos, bool isPositive, int strength);
	static FlushNoteStatus forEachSubsetInvokeFlush(int subset, int currentPos, int currentExistCount);
	static FlushNoteStatus flushNote(short subset);
public:
	static SudokuSolution *solve(Sudoku &sudoku, Mode mode = M_NORMAL);
	static SudokuSolution *solve(Sudoku &sudoku, Mode mode, int configCount, FlushNoteConfig *configs);
};

SudokuFilter::Scanner::Scanner() {
	count = 0;
	sentinel = {0, 0, 0, &sentinel, &sentinel};
}

SudokuFilter::Scanner::~Scanner() {
	ScannerNode *snp1 = sentinel.next;
	while (snp1 != &sentinel) {
		snp1 = snp1->next;
		delete snp1->prev;
	}
}

void SudokuFilter::Scanner::push(int y, int x, int blockPos) {
	sentinel.prev = sentinel.prev->next
		= new ScannerNode{y, x, blockPos, sentinel.prev, &sentinel};
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

int SudokuFilter::flushNoteConfigCount;

SudokuFilter::FlushNoteConfig *SudokuFilter::flushNoteConfigs = nullptr;

SudokuFilter::Scanner *SudokuFilter::scanners[3] = {};

Sudoku *SudokuFilter::filledSudoku = nullptr;

int SudokuFilter::elementCount;

int SudokuFilter::existCountMax;

SudokuFilter::AreaType SudokuFilter::scannerAreaType;

int SudokuFilter::scannerAreaPos;

bool SudokuFilter::isPositiveFlush;

short *SudokuFilter::negativeNotes = nullptr;

short SudokuFilter::negativeValueExists;

void SudokuFilter::pushSudokuToSolutionOrDelete(SudokuSolution &solution, Sudoku &sudoku) {
	switch (mode) {
	case M_NORMAL:
		solution.push(&sudoku);
		break;
	case M_COUNT_ONLY:
		delete &sudoku;
		solution.pseudoCount++;
		break;
	}
}

void SudokuFilter::filter(Sudoku &sudoku, SudokuSolution &solution) {

	int i1, i2, i3;

	filledSudoku = Sudoku::deepCopyCreate(sudoku);
	filledSudoku->fillNote();

	Scanner scannerStorage[3][side];
	for (i1 = 0; i1 < 3; i1++) {
		scanners[i1] = scannerStorage[i1];
	}
	for (i1 = 0; i1 < side; i1++) {
		for (i2 = 0; i2 < side; i2++) {
			if (sudoku.getValue(i1, i2) == 0) {
				scanners[AT_ROW][i1].push(i1, i2, i2);
				scanners[AT_COLUMN][i2].push(i1, i2, i1);
				scanners[AT_SUBGRID][i1 / order * order + i2 / order].push(i1, i2, i1 % order * order + i2 % order);
			}
		}
	}

	Sudoku *checkPoint = Sudoku::deepCopyCreate(*filledSudoku);
	ScannerNode *snPtr = nullptr, *snEnd = nullptr;
	FlushNoteStatus flushNoteStatus;
	bool finished, error = false;
	negativeNotes = new short[side]{};
	while (true) {
		for (i1 = 0; i1 < 3 && !error; i1++) {
			for (i2 = 0; i2 < side && !error; i2++) {
				for (i3 = 0; i3 < flushNoteConfigCount; i3++) {
					if (flushNoteConfigs[i3].strength == -1) {
						flushNoteStatus = startFlush((AreaType)i1, i2, flushNoteConfigs[i3].isPositive);
					} else {
						flushNoteStatus = startFlush((AreaType)i1, i2, flushNoteConfigs[i3].isPositive,
							flushNoteConfigs[i3].strength);
					}
					if (flushNoteStatus == FNS_RESET) {
						i2--;
						break;
					}
					if (flushNoteStatus == FNS_ERROR) {
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

	delete checkPoint;
	delete[] negativeNotes;
	checkPoint = nullptr;
	negativeNotes = nullptr;

}

SudokuFilter::FlushNoteStatus SudokuFilter::startFlush(AreaType areaType, int areaPos, bool isPositive) {
	return startFlush(areaType, areaPos, isPositive, scanners[areaType][areaPos].getCount());
}

SudokuFilter::FlushNoteStatus SudokuFilter::startFlush(AreaType areaType, int areaPos, bool isPositive, int strength) {
	
	if (strength <= 0) {
		return FNS_CONTINUE;
	}

	Scanner &scanner = scanners[areaType][areaPos];
	elementCount = scanner.getCount();
	existCountMax = strength > elementCount ? elementCount : strength;
	scannerAreaType = areaType;
	scannerAreaPos = areaPos;
	isPositiveFlush = isPositive;

	if (!isPositive) {

		ScannerNode *snp1 = nullptr;
		short s1;
		int i1, i2, i3;

		memset(negativeNotes, 0, sizeof (negativeNotes[0]) * side);
		negativeValueExists = 0;
		for (i1 = 0; i1 < side; i1++) {
			switch (areaType) {
			case AT_ROW:
				i2 = areaPos;
				i3 = i1;
				break;
			case AT_COLUMN:
				i2 = i1;
				i3 = areaPos;
				break;
			case AT_SUBGRID:
				i2 = areaPos / 3 * 3 + i1 / 3;
				i3 = areaPos % 3 * 3 + i1 % 3;
				break;
			}
			negativeValueExists |= 1 << (filledSudoku->getValue(i2, i3) - 1);
		}

		for (snp1 = scanner.begin(); snp1 != scanner.end(); snp1 = snp1->next) {
			for (i1 = 0, s1 = filledSudoku->getNote(snp1->y, snp1->x); i1 < side; i1++, s1 >>= 1) {
				if (s1 & 1) {
					negativeNotes[i1] |= 1 << snp1->blockPos;
				}
			}
		}

	}

	return forEachSubsetInvokeFlush(0, 0, 0);

}

SudokuFilter::FlushNoteStatus SudokuFilter::forEachSubsetInvokeFlush(int subset, int currentPos, int currentExistCount) {

	if (currentExistCount >= existCountMax) {
		return FNS_CONTINUE;
	}

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
		flushNoteStatus = forEachSubsetInvokeFlush(newSubset, currentPos + 1, currentExistCount + 1);
		if (flushNoteStatus != FNS_CONTINUE) {
			return flushNoteStatus;
		}
		currentPos++;
	}

	return FNS_CONTINUE;

}

SudokuFilter::FlushNoteStatus SudokuFilter::flushNote(short subset) {

	int i1, i2, i3, i4;
	short s1;
	ScannerNode *snp1 = nullptr;

	/**
	 * The current scanner specified by `scannerAreaType` and
	 * `scannerAreaPos`.
	 */
	Scanner &scanner = scanners[scannerAreaType][scannerAreaPos];

	/**
	 * The boolean list designating the positions of selected blocks
	 * in the area of the current scanner.
	 */
	short posCollection;

	/**
	 * The boolean list designating the existing note numbers in
	 * selected blocks.
	 */
	short numCollection;

	/**
	 * The number of blocks selected by `posCollection` in the area
	 * of the current scanner.
	 */
	int posCount;

	/**
	 * The number of different note numbers in selected blocks.
	 */
	int numCount;

	if (!isPositiveFlush) {
		posCollection = numCollection = 0;
		numCount = 0;
		for (i1 = 0, s1 = subset; i1 < side; i1++) {
			if (!(negativeValueExists & (1 << i1))) {
				if (s1 & 1) {
					posCollection |= negativeNotes[i1];
					numCollection |= 1 << i1;
					numCount++;
				}
				s1 >>= 1;
			}
		}
		posCount = 0;
		for (i1 = 0, s1 = posCollection; i1 < side; i1++, s1 >>= 1) {
			if (s1 & 1) {
				posCount++;
			}
		}
	} else {
		posCount = 0;
		posCollection = numCollection = 0;
		for (snp1 = scanner.begin(), s1 = subset; snp1 != scanner.end(); s1 >>= 1, snp1 = snp1->next) {
			if (s1 & 1) {
				posCount++;
				posCollection |= 1 << snp1->blockPos;
				numCollection |= filledSudoku->getNote(snp1->y, snp1->x);
			}
		}
		numCount = 0;
		for (i1 = 0, s1 = numCollection; i1 < side; i1++, s1 >>= 1) {
			if (s1 & 1) {
				numCount++;
			}
		}
	}

	/**
	 * Flush note numbers of unselected blocks when `posCount` =
	 * `numCount`.
	 */
	if (posCount != numCount) {
		return FNS_CONTINUE;
	}
	
	/**
	 * In case of negative flush mode, the selected blocks need
	 * flushing.
	 */
	if (!isPositiveFlush) {
		for (i1 = 0; i1 < side; i1++) {
			if (negativeNotes[i1] != 0 && !((1 << i1) & numCollection)) {
				if ((negativeNotes[i1] &= ~posCollection) == 0) {
					return FNS_ERROR;
				}
			}
		}
		for (snp1 = scanner.begin(); snp1 != scanner.end(); snp1 = snp1->next) {
			if (posCollection & (1 << snp1->blockPos)) {
				filledSudoku->getNote(snp1->y, snp1->x) &= numCollection;
			}
		}
	}
	
	/**
	 * The array of coordinates of selected blocks.
	 */
	int existBlockList[posCount][2];
	for (snp1 = scanner.begin(), i2 = 0; snp1 != scanner.end(); snp1 = snp1->next) {
		if (posCollection & (1 << snp1->blockPos)) {
			existBlockList[i2][0] = snp1->y;
			existBlockList[i2][1] = snp1->x;
			i2++;
		}
	}

	/**
	 * The array of boolean vars which designating if all selected
	 * blocks are in the same row, column, subgrid.
	 */
	bool sameArea[3] = {true, true, true};

	/**
	 * The array of positions of the same area of each types.
	 */
	int sameAreaPos[3] = {-1, -1, -1};

	for (snp1 = scanner.begin(); snp1 != scanner.end(); snp1 = snp1->next) {
		if (posCollection & (1 << snp1->blockPos)) {
			if (sameAreaPos[AT_ROW] != -1 && sameAreaPos[AT_ROW] != snp1->y) {
				sameArea[AT_ROW] = false;
			}
			if (sameAreaPos[AT_COLUMN] != -1 && sameAreaPos[AT_COLUMN] != snp1->x) {
				sameArea[AT_COLUMN] = false;
			}
			i1 = snp1->y / order * order + snp1->x / order;
			if (sameAreaPos[AT_SUBGRID] != -1 && sameAreaPos[AT_SUBGRID] != i1) {
				sameArea[AT_SUBGRID] = false;
			}
			sameAreaPos[AT_ROW] = snp1->y;
			sameAreaPos[AT_COLUMN] = snp1->x;
			sameAreaPos[AT_SUBGRID] = i1;
		}
	}

	/**
	 * The scanner in the specified area.
	 */
	Scanner *localScanner = nullptr;

	/**
	 * The var temporarily storing the number of note numbers in a
	 * flushed block.
	 */
	int restNoteNumCount;
	
	/**
	 * The var temporarily recording one of the note number in a flushed
	 * block. It is specially the last one due to the implementation.
	 */
	int lastNum;
	
	/**
	 * The boolean list designating the positions of selected blocks exist
	 * in the area of `localScanner`
	 */
	short localPosCollection;
		
	/**
	 * The var temporarily storing the flush status.
	 */
	FlushNoteStatus status = FNS_CONTINUE;

	/**
	 * Flush note numbers of unselected blocks.
	 */
	for (i1 = 0; i1 < 3; i1++) {

		/**
		 * Start flushing when all selected blocks are in the same
		 * row, column, subgrid.
		 */
		if (!sameArea[i1]) {
			continue;
		}

		/**
		 * Specify the area of the scanner.
		 */
		localScanner = &scanners[i1][sameAreaPos[i1]];

		/**
		 * Calculate the value of `localPosCollection`.
		 */
		if (scannerAreaType == i1) {
			localPosCollection = posCollection;
		} else {
			localPosCollection = 0;
			for (snp1 = localScanner->begin(); snp1 != localScanner->end(); snp1 = snp1->next) {
				for (i2 = 0; i2 < posCount; i2++) {
					if (snp1->y == existBlockList[i2][0] && snp1->x == existBlockList[i2][1]) {
						localPosCollection |= 1 << snp1->blockPos;
						break;
					}
				}
			}
		}

		/**
		 * Start flushing on the unselected blocks, delete note
		 * numbers existing in the numCollection. If a block has no
		 * note numbers after flushing, it cannot be marked with any
		 * value number and the sudoku is self-contradictory.
		 */
		for (snp1 = localScanner->begin(); snp1 != localScanner->end(); snp1 = snp1->next) {
			if (!(localPosCollection & (1 << snp1->blockPos))) {
				if ((filledSudoku->getNote(snp1->y, snp1->x) &= ~numCollection) == 0) {
					return FNS_ERROR;
				}
			}

		}

		/**
		 * Remove and mark the specific blocks.
		 */
		for (snp1 = localScanner->begin(); snp1 != localScanner->end(); snp1 = snp1->next) {

			if (localPosCollection & (1 << snp1->blockPos)) {

				/**
				 * Calculate the value of `lastNum` and `restNoteNumCount`.
				 */
				for (i2 = i3 = 0, s1 = filledSudoku->getNote(snp1->y, snp1->x); i2 < side; i2++, s1 >>= 1) {
					if (s1 & 1) {
						i3++;
						lastNum = i2 + 1;
					}
				}
				restNoteNumCount = i3;

				/**
				 * `restNoteNumCount` = 1 indicates that the block has only one
				 * note number, means that the note number is the only
				 * possible value number of the block. In this case, 
				 * the block should be marked with the only note number
				 * and removed from the scanner.
				 */
				if (restNoteNumCount == 1 && posCount == 1) {
					status = FNS_RESET;
					filledSudoku->setValue(snp1->y, snp1->x, lastNum);
					localScanner->remove(snp1);
					continue;
				}

			}

		}

	}

	return status;

}

SudokuSolution *SudokuFilter::solve(Sudoku &sudoku, Mode mode) {
	static FlushNoteConfig configs[] = SUDOKU_FILTER_CONFIG_DEFAULT;
	return solve(sudoku, mode, count(configs), configs);
}

SudokuSolution *SudokuFilter::solve(Sudoku &sudoku, Mode mode, int configCount, FlushNoteConfig *configs) {
	Timer timer;
	timer.start();
	flushNoteConfigCount = configCount;
	flushNoteConfigs = configs;
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
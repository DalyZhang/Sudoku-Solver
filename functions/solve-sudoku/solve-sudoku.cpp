void pushSudoku(SudokuList *list, Sudoku *sudoku) {

	if (list->count == 0) {
		list->count++;
		list->list = (Sudoku **)calloc(1, sizeof (Sudoku *));
	} else {
		list->count++;
		list->list = (Sudoku **)realloc(list->list, sizeof (Sudoku *) * list->count);
	}
	list->list[list->count - 1] = sudoku;

}

SolutionStatus solveSudoku(const Sudoku *before, SudokuList *afterList) {

	static int solveStack = 0;
	solveStack++;

	int i1, i2, i3;
	BoolList s1;

	Solution solution;
	createSolution(before, &solution);
	fillNote(solution.blocks);

	Subset subset;
	SolutionBlock blockCache[SD_S][SD_S];
	SolutionStatus status;
	FlushNoteStatus flushStatus;
	char finished, error = 0, toCreateHypothesis = 0;
	memcpy(&blockCache, &solution.blockStorage, sizeof (blockCache));
	while (1) {
		for (i1 = 0; i1 < 3 && !error; i1++) {
			for (i2 = 0; i2 < SD_S && !error; i2++) {
				if (solution.area[i1][i2].count == 0) {
					continue;
				}
				subset = createNonEmptySubset(solution.area[i1][i2].count);
				for (i3 = 0; i3 < subset.count; i3++) {
					flushStatus = flushNote(&solution.area[i1][i2], subset.exist[i3]);
					if (flushStatus == FLUSH_NOTE_STATUS__RECHECK) {
						i2--;
						break;
					}
					if (flushStatus == FLUSH_NOTE_STATUS__ERROR) {
						error = 1;
						break;
					}
				}
			}
		}
		if (error) {
			status = SOLUTION_STATUS__ERROR;
			break;
		}
		for (i1 = 0, finished = 1; i1 < SD_S; i1++) {
			for (i2 = 0; i2 < SD_S; i2++) {
				if (solution.blocks[i1][i2].value == 0) {
					finished = 0;
				}
			}
		}
		if (finished) {
			status = SOLUTION_STATUS__SUCCESS;
			break;
		}
		if (!memcmp(&blockCache, &solution.blockStorage, sizeof (blockCache))) {
			toCreateHypothesis = 1;
			break;
		}
		memcpy(&blockCache, &solution.blockStorage, sizeof (blockCache));
	}

	Sudoku *after = NULL;
	if (solveStack == 1) {
		*afterList = {};
	}
	if (!toCreateHypothesis) {
		if (status == SOLUTION_STATUS__SUCCESS) {
			after = (Sudoku *)calloc(1, sizeof (Sudoku));
			solutionToSudoku(&solution, after);
			pushSudoku(afterList, after);
		}
	} else {

		Sudoku checkPoint;
		solutionToSudoku(&solution, &checkPoint);
		// outputSudoku(&checkPoint, OUTPUT_MODE__NOTE);

		Block *firstNonValueBlock = NULL;
		for (i1 = 0; i1 < SD_S; i1++) {
			for (i2 = 0; i2 < SD_S; i2++) {
				if (checkPoint.blocks[i1][i2].value == 0) {
					firstNonValueBlock = &checkPoint.blocks[i1][i2];
					break;
				}
			}
			if (firstNonValueBlock != NULL) {
				break;
			}
		}

		int noteNumCount = 0;
		char noteNumList[SD_S];
		for (i1 = 0, s1 = firstNonValueBlock->note; i1 < SD_S; i1++, s1 >>= 1) {
			if (s1 & 1) {
				noteNumList[noteNumCount] = i1 + 1;
				noteNumCount++;
			}
		}

		for (i1 = 0, status = SOLUTION_STATUS__ERROR; i1 < noteNumCount; i1++) {
			firstNonValueBlock->value = noteNumList[i1];
			if (solveSudoku(&checkPoint, afterList) == SOLUTION_STATUS__SUCCESS) {
				status = SOLUTION_STATUS__SUCCESS;
			}
		}

	}
	destroySolution(&solution);

	solveStack--;
	return status;

}
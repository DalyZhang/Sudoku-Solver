SolutionStatus solveSudoku(const Sudoku *before, Sudoku *after) {

	int i1, i2, i3;
	
	Solution solution;
	createSolution(before, &solution);
	fillNote(solution.blocks);

	Subset subset;
	SolutionBlock blockCache[9][9];
	SolutionStatus status;
	FlushNoteStatus flushStatus;
	char finished, error = 0;
	memcpy(&blockCache, &solution.blockStorage, sizeof (blockCache));
	while (1) {
		for (i1 = 0; i1 < 3 && !error; i1++) {
			for (i2 = 0; i2 < 9 && !error; i2++) {
				if (solution.area[i1][i2].count == 0) {
					continue;
				}
				subset = createNonEmptySubset(solution.area[i1][i2].count);
				for (i3 = 0; i3 < subset.count; i3++) {
					flushStatus = flushNote(&solution.area[i1][i2], subset.exist[i3]);
					if (flushStatus == FLUSH_NOTE_RECHECK) {
						i2--;
						break;
					}
					if (flushStatus == FLUSH_NOTE_ERROR) {
						error = 1;
						break;
					}
				}
			}
		}
		if (error) {
			status = SOLUTION_ERROR;
			break;
		}
		for (i1 = 0, finished = 1; i1 < 9; i1++) {
			for (i2 = 0; i2 < 9; i2++) {
				if (solution.blocks[i1][i2].value == 0) {
					finished = 0;
				}
			}
		}
		if (finished) {
			status = SOLUTION_SUCCESS;
			break;
		}
		if (!memcmp(&blockCache, &solution.blockStorage, sizeof (blockCache))) {
			status = SOLUTION_UNFINISHED;
			break;
		}
		memcpy(&blockCache, &solution.blockStorage, sizeof (blockCache));
	}

	solutionToSudoku(&solution, after);
	destroySolution(&solution);

	return status;

}
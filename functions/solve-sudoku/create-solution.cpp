void createCheckList(CheckList *checkList, SolutionBlock **blockList) {

	int i1, i2;
	CheckNode *cnp1 = NULL, *cnp2 = NULL;

	cnp1 = &checkList->sentinel;
	for (i1 = 0, i2 = 0; i1 < 9; i1++) {
		if (blockList[i1]->value == 0) {
			cnp2 = (CheckNode *)calloc(1, sizeof (CheckNode));
			cnp2->prev = cnp1;
			cnp2->prev->next = cnp2;
			cnp2->block = blockList[i1];
			cnp1 = cnp2;
			i2++;
		}
	}
	cnp1->next = &checkList->sentinel;
	checkList->sentinel.prev = cnp1;
	checkList->count = i2;

}

void createSolution(const Sudoku *sudoku, Solution *solution) {
	
	int i1, i2, i3, i4, i5, i6;

	*solution = {};
	for (i1 = 0; i1 < 9; i1++) {
		for (i2 = 0; i2 < 9; i2++) {
			solution->blockStorage[i1][i2].value = sudoku->blockStorage[i1][i2].value;
		}
		solution->blocks[i1] = &solution->blockStorage[i1][0];
	}

	SolutionBlock *blockList[9] = {};
	for (i1 = 0; i1 < 9; i1++) {
		for (i2 = 0; i2 < 9; i2++) {
			blockList[i2] = &solution->blocks[i1][i2];
			blockList[i2]->area[AREA_ROW] = &solution->area[AREA_ROW][i1];
		}
		solution->area[AREA_ROW][i1].type = AREA_ROW;
		createCheckList(&solution->area[AREA_ROW][i1], &blockList[0]);
	}
	for (i1 = 0; i1 < 9; i1++) {
		for (i2 = 0; i2 < 9; i2++) {
			blockList[i2] = &solution->blocks[i2][i1];
			blockList[i2]->area[AREA_COLUMN] = &solution->area[AREA_COLUMN][i1];
		}
		solution->area[AREA_COLUMN][i1].type = AREA_COLUMN;
		createCheckList(&solution->area[AREA_COLUMN][i1], &blockList[0]);
	}
	for (i1 = 0; i1 < 3; i1++) {
		for (i2 = 0; i2 < 3; i2++) {
			i5 = i1 * 3 + i2;
			for (i3 = 0; i3 < 3; i3++) {
				for (i4 = 0; i4 < 3; i4++) {
					i6 = i3 * 3 + i4;
					blockList[i6] = &solution->blocks[i1 * 3 + i3][i2 * 3 + i4];
					blockList[i6]->area[AREA_SUBGRID] = &solution->area[AREA_SUBGRID][i5];
				}
			}
			solution->area[AREA_SUBGRID][i5].type = AREA_SUBGRID;
			createCheckList(&solution->area[AREA_SUBGRID][i5], &blockList[0]);
		}
	}

}
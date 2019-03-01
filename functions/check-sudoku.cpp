char checkSudoku(Sudoku *sudoku) {

	int i1, i2, i3, i4, i5, i6;

	Block **blocks = sudoku->blocks;
	for (i1 = 0; i1 < SD_S; i1++) {
		for (i2 = 0; i2 < SD_S; i2++) {
			if (blocks[i1][i2].value == 0) {
				continue;
			}
			for (i3 = 0; i3 < SD_S; i3++) {
				if (i3 != i1 && blocks[i1][i2].value == blocks[i3][i2].value) {
					return 0;
				}
				if (i3 != i2 && blocks[i1][i2].value == blocks[i1][i3].value) {
					return 0;
				}
			}
			for (i3 = 0, i5 = i1 / SD_O * SD_O; i3 < SD_O; i3++, i5++) {
				for (i4 = 0, i6 = i2 / SD_O * SD_O; i4 < SD_O; i4++, i6++) {
					if (i5 != i1 && i6 != i2 && blocks[i1][i2].value == blocks[i5][i6].value) {
						return 0;
					}
				}
			}
		}
	}

	return 1;

}
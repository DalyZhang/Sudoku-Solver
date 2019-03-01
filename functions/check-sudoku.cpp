char checkSudoku(Sudoku *sudoku) {

	int i1, i2, i3, i4, i5, i6;

	Block **blocks = sudoku->blocks;
	for (i1 = 0; i1 < 9; i1++) {
		for (i2 = 0; i2 < 9; i2++) {
			if (blocks[i1][i2].value == 0) {
				continue;
			}
			for (i3 = 0; i3 < 9; i3++) {
				if (i3 != i1 && blocks[i1][i2].value == blocks[i3][i2].value) {
					return 0;
				}
				if (i3 != i2 && blocks[i1][i2].value == blocks[i1][i3].value) {
					return 0;
				}
			}
			for (i3 = 0, i5 = i1 / 3 * 3; i3 < 3; i3++, i5++) {
				for (i4 = 0, i6 = i2 / 3 * 3; i4 < 3; i4++, i6++) {
					if (i5 != i1 && i6 != i2 && blocks[i1][i2].value == blocks[i5][i6].value) {
						return 0;
					}
				}
			}
		}
	}

	return 1;

}
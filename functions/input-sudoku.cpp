InputStatus inputSudoku(Sudoku *sudoku) {
	
	int i1, i2;
	char c1;

	*sudoku = {};
	for (i1 = 0; i1 < 9; i1++) {
		for (i2 = 0; i2 < 9; i2++) {
			while (1) {
				c1 = getchar();
				if (c1 == EOF) {
					return INPUT_STATUS__EOF;
				} else if (c1 <= '9' && c1 >= '0') {
					break;
				}
			}
			sudoku->blockStorage[i1][i2].value = (char)(c1 - '0');
		}
		sudoku->blocks[i1] = &sudoku->blockStorage[i1][0];
	}

	return INPUT_STATUS__SUCCESS;

}
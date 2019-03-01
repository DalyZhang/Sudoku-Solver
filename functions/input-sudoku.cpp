char characterToSudokuValue(char character) {

	static char *table = NULL;

	if (table == NULL) {

		int i1;
		
		table = (char *)calloc(128, sizeof (char));
		memset(table, -1, sizeof (char) * 128);
		const char *lists[] = SUDOKU_CHAR_LISTS;
		int listLen = strlen(lists[SD_O - 1]);
		for (i1 = 0; i1 < listLen; i1++) {
			table[lists[SD_O - 1][i1]] = i1;
		}

	}

	return table[character];

}

InputStatus inputSudoku(Sudoku *sudoku) {
	
	int i1, i2;
	char c1;

	*sudoku = {};
	for (i1 = 0; i1 < SD_S; i1++) {
		for (i2 = 0; i2 < SD_S; i2++) {
			while (1) {
				c1 = getchar();
				if (c1 == EOF) {
					return INPUT_STATUS__EOF;
				} else if ((c1 = characterToSudokuValue(c1)) != -1) {
					break;
				}
			}
			sudoku->blockStorage[i1][i2].value = c1;
		}
		sudoku->blocks[i1] = &sudoku->blockStorage[i1][0];
	}

	return INPUT_STATUS__SUCCESS;

}
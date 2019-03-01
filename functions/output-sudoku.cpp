char sudokuValueToCharacter(char sudokuValue) {

	static const char *table = NULL;

	if (table == NULL) {
		const char *lists[] = SUDOKU_CHAR_LISTS;
		table = lists[SD_O - 1];
	}

	return table[sudokuValue];

}

void outputSudoku(Sudoku *sudoku, OutputMode mode) {
	
	int i1, i2, i3;
	BoolList s1;

	switch (mode) {

		case OUTPUT_MODE__NOTE:
		for (i1 = 0; i1 < SD_S; i1++) {
			for (i2 = 0; i2 < SD_S; i2++) {
				printf("%c;", sudokuValueToCharacter(sudoku->blocks[i1][i2].value));
				for (i3 = 0, s1 = sudoku->blocks[i1][i2].note; i3 < SD_S; i3++, s1 >>= 1) {
					if (s1 & 1) {
						printf("%c", sudokuValueToCharacter(i3 + 1));
					}
				}
				puts(";");
			}
			puts("");
		}
		break;

		case OUTPUT_MODE__PLAIN:
		for (i1 = 0; i1 < SD_S; i1++) {
			for (i2 = 0; i2 < SD_S; i2++) {
				printf("%c", sudokuValueToCharacter(sudoku->blocks[i1][i2].value));
			}
			puts("");
		}
		break;

		case OUTPUT_MODE__IMAGE: {

			// int blockWidth = 3 * 2 + 2;
			// i1 = (3 * 2 + 2) * 9 + 2;
			// char boundary[(3 * 2 + 2) * 9 + 2] = {}, row[(3 * 2 + 2) * 9 + 2] = {};

			// for (i1 = 0; i1 < 9; i1++) {
			// 	i2 = i1 * 8;
			// 	boundary[i2] = '+';
			// 	for (i3 = 0; i3 < 9 - 1; i3++) {

			// 	}
			// }

		}
		break;

	}
	
	
}
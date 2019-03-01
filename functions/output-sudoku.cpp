char sudokuValueToCharacter(char sudokuValue) {

	static const char *table = NULL;

	if (table == NULL) {
		const char *lists[] = SUDOKU_CHAR_LISTS;
		table = lists[SD_O - 1];
	}

	return table[sudokuValue];

}

void outputSudoku(Sudoku *sudoku, OutputMode mode) {
	
	int i1, i2, i3, i4, i5;
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

			int blockWidth = SD_O * 2 + 2;
			int width = blockWidth * SD_S;

			char line[width + 2] = {};
			line[0] = line[width] = SUDOKU_PIVOT;
			memset(&line[1], SUDOKU_LINE_H, blockWidth - 1);
			for (i1 = 1; i1 < SD_S; i1++) {
				memcpy(&line[i1 * blockWidth], line, blockWidth);
			}

			char clearBlockRow[width + 2] = {};
			clearBlockRow[0] = clearBlockRow[width] = SUDOKU_LINE_V;
			memset(&clearBlockRow[1], SUDOKU_SPACE, blockWidth - 1);
			for (i1 = 1; i1 < SD_S; i1++) {
				memcpy(&clearBlockRow[i1 * blockWidth], clearBlockRow, blockWidth);
			}
			
			char blockRows[SD_O][width + 2];
			for (i1 = 0; i1 < SD_S; i1++) {
				puts(line);
				for (i2 = 0; i2 < SD_O; i2++) {
					memcpy(blockRows[i2], clearBlockRow, width + 2);
				}
				// puts("123");
				for (i2 = 0; i2 < SD_S; i2++) {
					// puts("0");
					// printf("value: %i\n", sudoku->blocks[i1][i2].value);
					if (sudoku->blocks[i1][i2].value == 0) {
						// puts("1");
						for (i3 = 0, s1 = sudoku->blocks[i1][i2].note; i3 < SD_S; i3++, s1 >>= 1) {
							if (s1 & 1) {
								i4 = i3 / SD_O;
								i5 = i2 * blockWidth + (i3 % SD_O + 1) * 2;
								// printf("y: %i, x: %i\n", i4, i5);
								blockRows[i4][i5] = sudokuValueToCharacter(i3 + 1);
							}
						}
					} else {
						i3 = i2 * blockWidth + blockWidth / 2;
						// printf("w: %i, y: %i, x: %i\n", blockWidth, SD_O / 2, i3);
						blockRows[SD_O / 2][i3] = sudokuValueToCharacter(sudoku->blocks[i1][i2].value);
						// puts("test");
						blockRows[SD_O / 2][i3 - 1] = SUDOKU_BRACKET_L;
						blockRows[SD_O / 2][i3 + 1] = SUDOKU_BRACKET_R;
					}
				}
				for (i2 = 0; i2 < SD_O; i2++) {
					puts(blockRows[i2]);
				}
			}
			puts(line);

		}
		break;

	}
	
	
}
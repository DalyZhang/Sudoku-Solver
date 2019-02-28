void outputSudoku(Sudoku *sudoku, int mode) {
	
	int i1, i2, i3;
	short s1;

	if (mode == OUTPUT_MODE_WITH_NOTE) {
		for (i1 = 0; i1 < 9; i1++) {
			for (i2 = 0; i2 < 9; i2++) {
				printf("%i;", sudoku->blocks[i1][i2].value);
				for (i3 = 0, s1 = sudoku->blocks[i1][i2].note; i3 < 9; i3++, s1 >>= 1) {
					if (s1 & 1) {
						printf("%i", i3 + 1);
					}
				}
				puts(";");
			}
			puts("");
		}
	}
	
	for (i1 = 0; i1 < 9; i1++) {
		for (i2 = 0; i2 < 9; i2++) {
			printf("%i", sudoku->blocks[i1][i2].value);
		}
		puts("");
	}
	
}
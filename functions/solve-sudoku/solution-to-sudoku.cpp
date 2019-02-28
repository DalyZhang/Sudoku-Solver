void solutionToSudoku(Solution *solution, Sudoku *sudoku) {

	int i1, i2;
	
	for (int i1 = 0; i1 < 9; i1++) {
		sudoku->blocks[i1] = &sudoku->blockStorage[i1][0];
		for (int i2 = 0; i2 < 9; i2++) {
			sudoku->blocks[i1][i2].value = solution->blocks[i1][i2].value;
			sudoku->blocks[i1][i2].note = solution->blocks[i1][i2].note;
		}
	}

}
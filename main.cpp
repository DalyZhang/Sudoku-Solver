#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "definitions.cpp"
#include "structs.cpp"
#include "functions/functions.cpp"

int main() {

	// Subset s;
	// s = createNonEmptySubset(4);
	// for (int i = 0; i < s.count; i++) {
	// 	printf("%i\n", s.exist[i]);
	// }
	// s = createNonEmptySubset(9);
	// for (int i = 0; i < s.count; i++) {
	// 	printf("%i\n", s.exist[i]);
	// }
	
	// Sudoku sudoku;
	// inputSudoku(&sudoku);
	// for (int i1 = 0; i1 < 9; i1++) {
	// 	for (int i2 = 0; i2 < 9; i2++) {
	// 		printf("%i", sudoku.blocks[i1][i2].value);
	// 	}
	// 	puts("");
	// }

	// Sudoku sudoku;
	// if (inputSudoku(&sudoku)) {
	// 	puts("eof");
	// 	return 1;
	// }
	// outputSudoku(&sudoku, OUTPUT_MODE_WITH_NOTE);

	// Sudoku sudoku;
	// if (inputSudoku(&sudoku)) {
	// 	puts("eof");
	// 	return 1;
	// }
	// Solution solution;
	// createSolution(&sudoku, &solution);
	// for (int i = 0; i < 9; i++) {
	// 	printf("%i,", solution.area[1][i].count);
	// 	puts("");
	// 	CheckNode *cnp = solution.area[1][i].sentinel.next;
	// 	while (cnp->block != NULL) {
	// 		printf("%i,", (int)cnp->block->value);
	// 		cnp = cnp->next;
	// 	}
	// 	puts("");
	// }
	
	// Sudoku before, after;
	// if (inputSudoku(&before)) {
	// 	puts("eof");
	// 	return 1;
	// }
	// Solution solution;
	// createSolution(&before, &solution);
	// fillNote(solution.blocks);
	// solutionToSudoku(&solution, &after);
	// destroySolution(&solution);
	// outputSudoku(&after, OUTPUT_MODE_WITH_NOTE);

	// printf("%i", countExist(15));

	Sudoku before, after;
	SolutionStatus status;
	while (!inputSudoku(&before)) {
		status = solveSudoku(&before, &after);
		printf("status: %i\n", status);
		outputSudoku(&after, OUTPUT_MODE_PLAIN);
		puts("");
	}

	// Sudoku before, after;
	// inputSudoku(&before);
	// while (1) {
	// 	solveSudoku(&before, &after);
	// }

	return 0;
}
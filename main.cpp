#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>

#include "definitions.cpp"
#include "structs.cpp"
#include "functions/index.cpp"

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
	// outputSudoku(&sudoku, OUTPUT_MODE__NOTE);

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
	// outputSudoku(&after, OUTPUT_MODE__NOTE);

	// printf("%i", countExist(15));

	Sudoku before;
	SudokuList afterList;
	SolutionStatus status;
	clock_t startTime, stopTime;
	int ord = 0;
	while (!inputSudoku(&before)) {

		if (!checkSudoku(&before)) {
			puts("error: the sudoku is invalid");
			continue;
		}

		startTime = clock();
		status = solveSudoku(&before, &afterList);
		stopTime = clock();
		ord++;
		printf("ordinal: %i\n", ord);
		printf("status: %i\n", status);
		printf("time: %ims\n", stopTime - startTime);
		printf("solution count: %i\n", afterList.count);
		for (int i = 0; i < afterList.count; i++) {
			puts("---------");
			outputSudoku(afterList.list[i], OUTPUT_MODE__PLAIN);
		}
		puts("=========");

		destorySudokuList(&afterList);
		
	}

	// Sudoku before, after;
	// inputSudoku(&before);
	// while (1) {
	// 	solveSudoku(&before, &after);
	// }

	return 0;
}
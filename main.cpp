#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <windows.h>
#include <conio.h>

#include "definitions.cpp"
#include "classes/index.cpp"

void solverTest() {
	
	int order = 3, side = order * order;
	Sudoku sudoku(order);
	SudokuSolution *solution = nullptr;
	int ord;
	double totalTime;
	FILE *fp = nullptr;

	// fp = fopen("solver1.result.txt", "wb");
	fp = stdout;
	ord = 0;
	totalTime = 0;
	while (sudoku.read() != Sudoku::RS_EOF) {
		ord++;
		solution = SudokuSolver1::solve(sudoku);
		totalTime += solution->time;
		fprintf(fp, "ordinal: %i\n", ord);
		fprintf(fp, "status: %i\n", solution->status);
		fprintf(fp, "time: %lfms\n", solution->time);
		fprintf(fp, "solution count: %i\n", solution->getCount());
		for (auto &result : *solution) {
			for (int i = 0; i < side; i++) {
				fputc('-', fp);
			}
			fputc('\n', fp);
			result->write(Sudoku::WM_PLAIN, fp);
		}
		for (int i = 0; i < side; i++) {
			fputc('=', fp);
		}
		fputc('\n', fp);
		delete solution;
		fflush(fp);
	}
	fprintf(fp, "total spend: %lfms", totalTime);
	// fclose(fp);

}

void judgeTest() {

	static constexpr char *tips[] = {"easy", "medium", "hard", "very hard", "error", "multi-solution"};
	
	Sudoku sudoku(3);
	SudokuJudge::Difficulty difficulty;
	int ord;
	FILE *fp = nullptr;

	fp = stdout;
	ord = 0;
	while (sudoku.read() != Sudoku::RS_EOF) {
		ord++;
		difficulty = SudokuJudge::judge(sudoku);
		fprintf(fp, "ordinal: %i\n", ord);
		fprintf(fp, "difficulty: %s\n", tips[difficulty]);
		for (int i = 0; i < 26; i++) {
			fputc('=', fp);
		}
		fputc('\n', fp);
	}

}

int main(int argc, char *args[]) {

	if (argc > 1) {
		if (strcmp(args[1], "solver") == 0) {
			solverTest();
		} else if (strcmp(args[1], "judge") == 0) {
			judgeTest();
		}
	} else {
		solverTest();
	}

	return 0;

}
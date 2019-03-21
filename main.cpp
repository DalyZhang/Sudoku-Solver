#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <windows.h>
#include <conio.h>

#include "definitions.cpp"
#include "classes/index.cpp"

int main() {

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
	fclose(fp);

	return 0;

}
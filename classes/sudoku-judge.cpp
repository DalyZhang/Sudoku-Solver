class SudokuJudge {
public:
	enum JudgeStatus {JS_SUCCESS, JS_ERROR, JS_UNFINISHED, JS_MULTI_SOLUTIONS};
	enum Difficulty {D_EASY, D_MEDIUM, D_HARD, D_VERY_HARD, D_ERROR, D_MULTI_SOLUTION};
private:
	typedef SudokuFilter::FlushNoteConfig Config;
	static Config **configList;
	static int *configCountList;
	static Config **initializeConfigList();
	static JudgeStatus judgeAll(Sudoku &sudoku);
	static JudgeStatus judgeDifficulty(Sudoku &sudoku, Difficulty difficulty);
public:
	static Difficulty judge(Sudoku &sudoku);
};

SudokuJudge::Config **SudokuJudge::configList = initializeConfigList();

int *SudokuJudge::configCountList;

SudokuJudge::Config **SudokuJudge::initializeConfigList() {
	static Config easyConfigs[] = SUDOKU_JUDGE_CONFIG_EASY;
	static Config mediumConfigs[] = SUDOKU_JUDGE_CONFIG_MEDIUM;
	static Config hardConfigs[] = SUDOKU_JUDGE_CONFIG_HARD;
	static Config *configList[] = {easyConfigs, mediumConfigs, hardConfigs};
	static int configCountList[] = {count(easyConfigs), count(mediumConfigs), count(hardConfigs)};
	SudokuJudge::configCountList = configCountList;
	return configList;
}

SudokuJudge::JudgeStatus SudokuJudge::judgeAll(Sudoku &sudoku) {

	SudokuSolution *solution = SudokuBacktracker::solve(sudoku, SudokuSolver::M_COUNT_ONLY, 2, SudokuFilter::solve);

	int solutionCount = solution->getCount();
	delete solution;
	switch (solutionCount) {
	case 0:
		return JS_ERROR;
	case 1:
		return JS_SUCCESS;
	case 2:
		return JS_MULTI_SOLUTIONS;
	}

}

SudokuJudge::JudgeStatus SudokuJudge::judgeDifficulty(Sudoku &sudoku, Difficulty difficulty) {

	SudokuSolution *solution = SudokuFilter::solve(sudoku, SudokuSolver::M_COUNT_ONLY,
		configCountList[difficulty], configList[difficulty]);

	SudokuSolution::Status solutionStatus = solution->status;
	delete solution;
	switch (solutionStatus) {
	case SudokuSolution::S_SUCCESS:
		return JS_SUCCESS;
	case SudokuSolution::S_UNFINISHED:
		return JS_UNFINISHED;
	}
	
}

SudokuJudge::Difficulty SudokuJudge::judge(Sudoku &sudoku) {

	int i1;

	Difficulty difficulty = D_VERY_HARD;
	JudgeStatus judgeStatus;

	switch (judgeStatus = judgeAll(sudoku)) {
	case JS_ERROR:
		return D_ERROR;
	case JS_MULTI_SOLUTIONS:
		return D_MULTI_SOLUTION;
	}

	for (i1 = (int)D_HARD; i1 > -1; i1--) {
		judgeStatus = judgeDifficulty(sudoku, (Difficulty)i1);
		if (judgeStatus == JS_SUCCESS) {
			difficulty = (Difficulty)i1;
		} else {
			return difficulty;
		}
	}

	return difficulty;
	
}
typedef unsigned char BYTE;

enum SolutionStatus {
	SOLUTION_SUCCESS,
	SOLUTION_ERROR,
	SOLUTION_UNFINISHED
};
enum FlushNoteStatus {
	FLUSH_NOTE_CONTINUE,
	FLUSH_NOTE_ERROR,
	FLUSH_NOTE_RECHECK
};
enum AreaType {
	AREA_ROW,
	AREA_COLUMN,
	AREA_SUBGRID
};
enum OutputMode {
	OUTPUT_MODE_PLAIN,
	OUTPUT_MODE_WITH_NOTE
};
enum InputStatus {
	INPUT_SUCCESS,
	INPUT_EOF
};
enum FillNoteStatus {
	FILL_NOTE_SUCCESS,
	FILL_NOTE_ERROR
};
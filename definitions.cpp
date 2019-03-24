typedef short note_t;

#define SUDOKU_CHAR_LISTS {"01", "01234", "0123456789", "0123456789ABCDEFG"}
#define SUDOKU_PIVOT '+'
#define SUDOKU_LINE_H '-'
#define SUDOKU_LINE_V '|'
#define SUDOKU_SPACE ' '
#define SUDOKU_BRACKET_L '['
#define SUDOKU_BRACKET_R ']'

#define SUDOKU_FILTER_CONFIG_DEFAULT {{true, -1}}

#define SUDOKU_BACKTRACKER_FILTER_CONFIG_DEFAULT {{false, 1}}

#define SUDOKU_JUDGE_CONFIG_EASY {{false, 1}}
#define SUDOKU_JUDGE_CONFIG_MEDIUM {{false, 1}, {true, 1}}
#define SUDOKU_JUDGE_CONFIG_HARD {{true, -1}}

#define count(array) (sizeof (array) / sizeof (array[0]))
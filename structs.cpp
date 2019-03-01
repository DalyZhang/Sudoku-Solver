struct Block;
struct Sudoku;
struct Subset;
struct CheckNode;
struct CheckList;
struct SolutionBlock;
struct Solution;

struct Block {
	char value;
	short note;
	// CheckList *row, *column, *subgrid;
};

struct Sudoku {
	Block blockStorage[9][9], *blocks[9];
};

struct SudokuList {
	int count;
	Sudoku **list;
};

struct Subset {
	int count;
	short *exist;
};
//
struct CheckNode {
	SolutionBlock *block;
	CheckNode *prev, *next;
	// CheckList *parent;
};

struct CheckList {
	int count;
	AreaType type;
	CheckNode sentinel;
};

struct SolutionBlock {
	char value;
	short note;
	CheckList *area[3];
};

struct Solution {
	SolutionBlock blockStorage[9][9], *blocks[9];
	CheckList area[3][9];
};
//
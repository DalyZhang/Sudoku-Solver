struct Block;
struct Sudoku;
struct Subset;
struct CheckNode;
struct CheckList;
struct SolutionBlock;
struct Solution;

struct Block {
	char value;
	BoolList note;
	// CheckList *row, *column, *subgrid;
};

struct Sudoku {
	Block blockStorage[SD_S][SD_S], *blocks[SD_S];
};

struct SudokuList {
	int count;
	Sudoku **list;
};

struct Subset {
	int count;
	BoolList *exist;
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
	BoolList note;
	CheckList *area[3];
};

struct Solution {
	SolutionBlock blockStorage[SD_S][SD_S], *blocks[SD_S];
	CheckList area[3][SD_S];
};
//
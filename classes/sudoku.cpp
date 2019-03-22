class Sudoku {
public:
	enum ReadStatus {RS_SUCCESS, RS_EOF};
	enum ReadMode {RM_CHARACTER, RM_NUMBER};
	enum WriteMode {WM_PLAIN, WM_PLAIN_NUMBER, WM_NOTE, WM_NOTE_NUMBER, WM_IMAGE};
	enum CompareMode {CM_VALUE = 0b1, CM_NOTE = 0b10};
private:
	int order = 3;
	int *value = nullptr, **valuePtr = nullptr;
	note_t *note = nullptr, **notePtr = nullptr;
	int characterToValue(char character);
	char valueToCharacter(int value);
	static int charModeMaxOrder;
	static int initCharModeMaxOrder();
public:
	Sudoku(int order = 3);
	~Sudoku();
	ReadStatus read(ReadMode mode = RM_CHARACTER, FILE *file = stdin);
	// ReadStatus readString(char *&sudokuString, ReadMode mode = RM_CHARACTER);
	void write(WriteMode mode = WM_PLAIN, FILE *file = stdout);
	void writeString(char *&sudokuString, WriteMode mode = WM_PLAIN);
	bool fillNoteInBlock(int y, int x) const;
	bool fillNote() const;
	bool checkBlock(int y, int x, int testValue = 0) const;
	bool check() const;
	int getOrder() const;
	int getValue(int y, int x) const;
	bool setValue(int y, int x, int value);
	note_t &getNote(int y, int x) const;
	static bool compare(Sudoku &a, Sudoku &b, int mode = CM_VALUE | CM_NOTE);
	static bool deepCopyAssign(Sudoku &dest, Sudoku &source);
	static Sudoku *deepCopyCreate(Sudoku &source);
};

int Sudoku::characterToValue(char character) {
	static int **table = nullptr;
	if (table == nullptr) {
		int i1, i2;
		const char *lists[] = SUDOKU_CHAR_LISTS;
		int listCount = sizeof (lists) / sizeof (lists[0]);
		table = new int *[listCount];
		for (i1 = 0; i1 < listCount; i1++) {
			table[i1] = new int[128];
			memset(table[i1], -1, sizeof (table[0][0]) * 128);
		}
		int listLen;
		for (i1 = 0; i1 < listCount; i1++) {
			listLen = strlen(lists[i1]);
			for (i2 = 0; i2 < listLen; i2++) {
				table[i1][lists[i1][i2]] = i2;
			}
		}
	}
	return table[order - 1][character];
}

char Sudoku::valueToCharacter(int value) {
	static const char *table[] = SUDOKU_CHAR_LISTS;
	return table[order - 1][value];
}

int Sudoku::charModeMaxOrder = initCharModeMaxOrder();

int Sudoku::initCharModeMaxOrder() {
	const char *lists[] = SUDOKU_CHAR_LISTS;
	return sizeof (lists) / sizeof (lists[0]);
}

Sudoku::Sudoku(int order) {
	if (order < 1 || order > charModeMaxOrder) {
		order = 3;
	}
	this->order = order;
	int side = order * order;
	int sideSquare = side * side;
	value = new int[sideSquare]{};
	note = new note_t[sideSquare]{};
	valuePtr = new int *[side];
	notePtr = new note_t *[side];
	for (int i1 = 0, i2 = 0; i1 < side; i1++, i2 += side) {
		valuePtr[i1] = &value[i2];
		notePtr[i1] = &note[i2];
	}
}

Sudoku::~Sudoku() {
	delete[] value;
	delete[] note;
	delete[] valuePtr;
	delete[] notePtr;
	value = nullptr;
	note = nullptr;
	valuePtr = nullptr;
	notePtr = nullptr;
}

Sudoku::ReadStatus Sudoku::read(ReadMode mode, FILE *file) {

	int i1, i2, i3;
	char c1;

	int side = order * order;

	if (order > charModeMaxOrder) {
		if (mode == RM_CHARACTER) {
			mode = RM_NUMBER;
		}
	}

	switch (mode) {
	case RM_CHARACTER:
		for (i1 = 0; i1 < side; i1++) {
			for (i2 = 0; i2 < side; i2++) {
				while (true) {
					c1 = fgetc(file);
					if (c1 == EOF) {
						return RS_EOF;
					} else if ((c1 = characterToValue(c1)) != -1) {
						break;
					}
				}
				valuePtr[i1][i2] = c1;
			}
		}
		break;
	case RM_NUMBER:
		for (i1 = 0; i1 < side; i1++) {
			for (i2 = 0; i2 < side; i2++) {
				while (true) {
					if (fscanf(file, "%d", &i3) == EOF) {
						return RS_EOF;
					} else if (i3 >= 0 || i3 <= side) {
						break;
					}
				}
				valuePtr[i1][i2] = i3;
			}
		}
		break;
	}

	return RS_SUCCESS;

}

// ReadStatus Sudoku::readString(char *&sudokuString, ReadMode mode) {

// }

void Sudoku::write(WriteMode mode, FILE *file) {
	char *str = nullptr;
	writeString(str, mode);
	fprintf(file, str);
	delete[] str;
}

void Sudoku::writeString(char *&sudokuString, WriteMode mode) {

	int i1, i2, i3, i4, i5;
	note_t n1;

	int side = order * order;

	if (order > charModeMaxOrder) {
		if (mode == WM_NOTE) {
			mode = WM_NOTE_NUMBER;
		} else if (mode == WM_PLAIN || mode == WM_IMAGE) {
			mode = WM_PLAIN_NUMBER;
		}
	}

	switch (mode) {

	case WM_NOTE:
	case WM_NOTE_NUMBER:
		sudokuString = new char[((side + 4) * side + 1) * side + 1]{};
		for (i1 = 0, i3 = 0; i1 < side; i1++) {
			for (i2 = 0; i2 < side; i2++) {
				sudokuString[i3++] = valueToCharacter(valuePtr[i1][i2]);
				sudokuString[i3++] = ':';
				for (i4 = 0, n1 = notePtr[i1][i2]; i4 < side; i4++, n1 >>= 1) {
					if (n1 & 1) {
						sudokuString[i3++] = valueToCharacter(i4 + 1);
					}
				}
				sudokuString[i3++] = ';';
				sudokuString[i3++] = '\n';
			}
			sudokuString[i3++] = '\n';
		}
		break;

	case WM_PLAIN:
	case WM_PLAIN_NUMBER:
		sudokuString = new char[(side + 1) * side + 1]{};
		for (i1 = 0, i3 = 0; i1 < side; i1++) {
			for (i2 = 0; i2 < side; i2++) {
				sudokuString[i3++] = valueToCharacter(valuePtr[i1][i2]);
			}
			sudokuString[i3++] = '\n';
		}
		break;

	case WM_IMAGE:
		{

			int blockWidth = order * 2 + 2;
			int width = blockWidth * side;
			sudokuString = new char[(width + 2) * (side * (order + 1) + 1) + 1]{};

			char line[width + 3] = {};
			line[0] = line[width] = SUDOKU_PIVOT;
			line[width + 1] = '\n';
			memset(&line[1], SUDOKU_LINE_H, blockWidth - 1);
			for (i1 = 1; i1 < side; i1++) {
				memcpy(&line[i1 * blockWidth], line, blockWidth);
			}

			char clearBlockRow[width + 3] = {};
			clearBlockRow[0] = clearBlockRow[width] = SUDOKU_LINE_V;
			clearBlockRow[width + 1] = '\n';
			memset(&clearBlockRow[1], SUDOKU_SPACE, blockWidth - 1);
			for (i1 = 1; i1 < side; i1++) {
				memcpy(&clearBlockRow[i1 * blockWidth], clearBlockRow, blockWidth);
			}

			char blockRows[order][width + 3];
			for (i1 = 0; i1 < side; i1++) {
				strcat(sudokuString, line);
				for (i2 = 0; i2 < order; i2++) {
					memcpy(blockRows[i2], clearBlockRow, width + 3);
				}
				for (i2 = 0; i2 < side; i2++) {
					if (valuePtr[i1][i2] == 0) {
						for (i3 = 0, n1 = notePtr[i1][i2]; i3 < side; i3++, n1 >>= 1) {
							if (n1 & 1) {
								i4 = i3 / order;
								i5 = i2 * blockWidth + (i3 % order + 1) * 2;
								blockRows[i4][i5] = valueToCharacter(i3 + 1);
							}
						}
					} else {
						i3 = i2 * blockWidth + blockWidth / 2;
						blockRows[order / 2][i3] = valueToCharacter(valuePtr[i1][i2]);
						blockRows[order / 2][i3 - 1] = SUDOKU_BRACKET_L;
						blockRows[order / 2][i3 + 1] = SUDOKU_BRACKET_R;
					}
				}
				for (i2 = 0; i2 < order; i2++) {
					strcat(sudokuString, blockRows[i2]);
				}
			}
			strcat(sudokuString, line);

		}
		break;

	}

}

bool Sudoku::fillNoteInBlock(int y, int x) const {

	int i1, i2, i3, i4;

	int side = order * order;
	bool hasError = false;

	if (valuePtr[y][x] == 0) {
		notePtr[y][x] = ~(~0 << side);
		for (i1 = 0; i1 < side; i1++) {
			if (valuePtr[y][i1] != 0) {
				notePtr[y][x] &= ~(1 << (valuePtr[y][i1] - 1));
			}
			if (valuePtr[i1][x] != 0) {
				notePtr[y][x] &= ~(1 << (valuePtr[i1][x] - 1));
			}
		}
		for (i1 = i3 = y / order * order; i1 < i3 + order; i1++) {
			for (i2 = i4 = x / order * order; i2 < i4 + order; i2++) {
				if (valuePtr[i1][i2] != 0) {
					notePtr[y][x] &= ~(1 << (valuePtr[i1][i2] - 1));
				}
			}
		}
		if (notePtr[y][x] == 0) {
			hasError = true;
		}
	} else {
		notePtr[y][x] = 0;
	}

	return !hasError;

}

bool Sudoku::fillNote() const {

	int i1, i2;

	int side = order * order;
	bool hasError = false;

	for (i1 = 0; i1 < side; i1++) {
		for (i2 = 0; i2 < side; i2++) {
			hasError = !fillNoteInBlock(i1, i2);
		}
	}

	return !hasError;

}

bool Sudoku::checkBlock(int y, int x, int testValue) const {

	int i1, i2, i3, i4;

	int side = order * order;
	if (testValue == 0) {
		testValue = valuePtr[y][x];
	}

	if (testValue == 0) {
		return true;
	}
	for (i1 = 0; i1 < side; i1++) {
		if (y != i1 && testValue == valuePtr[i1][x]) {
			return false;
		}
		if (x != i1 && testValue == valuePtr[y][i1]) {
			return false;
		}
	}
	for (i1 = 0, i3 = y / order * order; i1 < order; i1++, i3++) {
		for (i2 = 0, i4 = x / order * order; i2 < order; i2++, i4++) {
			if ((y != i3 || x != i4) && testValue == valuePtr[i3][i4]) {
				return false;
			}
		}
	}

	return true;

}

bool Sudoku::check() const {

	int i1, i2;

	int side = order * order;

	for (i1 = 0; i1 < side; i1++) {
		for (i2 = 0; i2 < side; i2++) {
			if (!checkBlock(i1, i2)) {
				return false;
			}
		}
	}

	return true;

}

int Sudoku::getOrder() const {
	return order;
}

int Sudoku::getValue(int y, int x) const {
	return valuePtr[y][x];
}

bool Sudoku::setValue(int y, int x, int newValue) {
	if (newValue < 0 || newValue > order * order) {
		return false;
	}
	valuePtr[y][x] = newValue;
	return true;
}

note_t &Sudoku::getNote(int y, int x) const {
	return notePtr[y][x];
}

bool Sudoku::compare(Sudoku &a, Sudoku &b, int mode) {

	int i1, i2;

	if (a.order != b.order) {
		return false;
	}

	int side = a.order * a.order;
	int sideSquare = side * side;

	if (mode & CM_VALUE) {
		if (memcmp(a.value, b.value, sizeof (a.value[0]) * sideSquare)) {
			return false;
		}
	}

	if (mode & CM_NOTE) {
		if (memcmp(a.note, b.note, sizeof (a.note[0]) * sideSquare)) {
			return false;
		}
	}

	return true;

}

bool Sudoku::deepCopyAssign(Sudoku &dest, Sudoku &source) {
	if (dest.order != source.order) {
		return false;
	}
	int side = source.order * source.order;
	int sideSquare = side * side;
	memcpy(dest.value, source.value, sizeof (source.value[0]) * sideSquare);
	memcpy(dest.note, source.note, sizeof (source.note[0]) * sideSquare);
	return true;
}

Sudoku *Sudoku::deepCopyCreate(Sudoku &source) {
	Sudoku *newSudoku = new Sudoku(source.order);
	deepCopyAssign(*newSudoku, source);
	return newSudoku;
}
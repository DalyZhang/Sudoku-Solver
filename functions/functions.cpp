void solve(void);
void input(Cell **cells);
void fillNote(Cell **cells);
void createNoteCellGuard(NoteCell *noteCellGuardList, Cell **cells);
void insertNoteCell(NoteCell *noteCellGuard, Cell *cell);
BYTE flushCell(NoteCell *cellGuard, BYTE *subset, BYTE count, BYTE pos);
void output(Cell **cells);

#include "solve.cpp"
#include "input.cpp"
#include "fill-note.cpp"
#include "create-note-cell-guard.cpp"
#include "flush-cell.cpp"
#include "output.cpp"
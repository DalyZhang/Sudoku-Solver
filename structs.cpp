// struct Note {
//     BYTE value;
//     Note *prev, *next;
// };

struct Cell {
    BYTE value, note[9];
};

struct NoteCell {
    Cell *cell;
    NoteCell *prev, *next;
};
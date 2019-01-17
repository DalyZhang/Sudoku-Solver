void fillNote(Cell **cells) {
    
    int i1, i2, i3, i4, i5, i6;

    Cell *cell = NULL;

    for (i1 = 0; i1 < 9; i1++) {
        for (i2 = 0; i2 < 9; i2++) {
            cell = &cells[i1][i2];
            if (cell->value == 0) {
                memset(&cell->note, 3, sizeof (cell->note));
                for (i3 = 0; i3 < 9; i3++) {
                    if (cells[i3][i2].value != 0) {
                        cell->note[cells[i3][i2].value - 1] = 0;
                    }
                    if (cells[i1][i3].value != 0) {
                        cell->note[cells[i1][i3].value - 1] = 0;
                    }
                }
                for (i3 = i5 = i1 / 3 * 3; i3 < i5 + 3; i3++) {
                    for (i4 = i6 = i2 / 3 * 3; i4 < i6 + 3; i4++) {
                        if (cells[i3][i4].value != 0) {
                            cell->note[cells[i3][i4].value - 1] = 0;
                        }
                    }
                }
            }
        }
    }

}
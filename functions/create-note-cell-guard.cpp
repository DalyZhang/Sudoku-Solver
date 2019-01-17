void createNoteCellGuard(NoteCell *noteCellGuardList, Cell **cells) {

    int i1, i2, i3, i4, i5, i6, i7;
    
    for (i1 = 0; i1 < 9; i1++) {
        i3 = i1 + 9;
        noteCellGuardList[i1].prev =
        noteCellGuardList[i1].next = &noteCellGuardList[i1];
        noteCellGuardList[i3].prev =
        noteCellGuardList[i3].next = &noteCellGuardList[i3];
        for (i2 = 0; i2 < 9; i2++) {
            if (cells[i1][i2].value == 0) {
                insertNoteCell(&noteCellGuardList[i1], &cells[i1][i2]);
            }
            if (cells[i2][i1].value == 0) {
                insertNoteCell(&noteCellGuardList[i3], &cells[i2][i1]);
            }
        }
    }

    for (i1 = 0; i1 < 3; i1++) {
        for (i2 = 0; i2 < 3; i2++) {
            i5 = 18 + i1 * 3 + i2;
            noteCellGuardList[i5].prev =
            noteCellGuardList[i5].next = &noteCellGuardList[i5];
            for (i3 = 0; i3 < 3; i3++) {
                for (i4 = 0; i4 < 3; i4++) {
                    i6 = i1 * 3 + i3;
                    i7 = i2 * 3 + i4;
                    if (cells[i6][i7].value == 0) {
                        insertNoteCell(&noteCellGuardList[i5], &cells[i6][i7]);
                    }
                }
            }
        }
    }

}

void insertNoteCell(NoteCell *noteCellGuard, Cell *cell) {

    NoteCell *ncp1 = (NoteCell *)calloc(1, sizeof (NoteCell));
    ncp1->cell = cell;
    ncp1->next = noteCellGuard;
    ncp1->prev = noteCellGuard->prev;
    ncp1->prev->next = ncp1->next->prev = ncp1;

}
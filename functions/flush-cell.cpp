BYTE flushCell(NoteCell *cellGuard, BYTE *subset, BYTE count, BYTE pos) {

    int i1;
    BYTE b1;

    /*
     * recursively get the subset of the existences of cells -> newSubset
     * stop to flush when a subset created
     * restart the creating and flush when
     * - get a returned value from flushCell() different from count
     */
    if (pos < count) {
        BYTE newSubset[pos + 1] = {};
        if (subset != NULL) {
            memcpy(newSubset, subset, pos * sizeof (BYTE));
        }
        for (i1 = 0; i1 < 2; i1++) {
            newSubset[pos] = i1;
            b1 = flushCell(cellGuard, newSubset, count, pos + 1);
            if (b1 != count) {
                count = b1;
                if (subset != NULL || b1 == 0) {
                    break;
                } else {
                    i1 = -1;
                    continue;
                }
            }
        }
        return count;
    }

    // empty line
    // puts("");

    // output the subset of existences
    // for (i1 = 0; i1 < count; i1++) {
    //     printf("%i", (int)subset[i1]);
    // }
    // puts("===");

    int i2;
    NoteCell *ncp1 = NULL, *ncp2 = NULL;

    // output the condition of notes
    // for (ncp1 = cellGuard->next; ncp1 != cellGuard; ncp1 = ncp1->next) {
    //     for (i1 = 0; i1 < 9; i1++) {
    //         if (ncp1->cell->note[i1]) {
    //             printf("%i", i1 + 1);
    //         }
    //     }
    //     printf(";");
    // }
    // puts("");
    
    /*
     * make the statistics for the note numbers existing in the cells selected -> isExist
     * count the number of selected cells -> cellNum
     */
    int cellNum;
    BYTE isExist[9] = {};
    for (i1 = cellNum = 0, ncp1 = cellGuard->next; i1 < count; i1++, ncp1 = ncp1->next) {
        if (subset[i1]) {
            for (i2 = 0; i2 < 9; i2++) {
                if (ncp1->cell->note[i2]) {
                    isExist[i2] = 1;
                }
            }
            cellNum++;
        }
    }
    if (cellNum == 0) {
        return count;
    }

    /*
     * count the number of different note numbers -> kindNum
     */
    int kindNum;
    for (i1 = 0, kindNum = 0; i1 < 9; i1++) {
        if (isExist[i1]) {
            kindNum++;
        }
    }

    // output test
    // printf("%i=%i\n", kindNum, cellNum);

    /*
     * flush note numbers of unselected cells
     */
    if (kindNum == cellNum) {

        /*
         * create the collection of the existing note numbers -> collection
         */
        BYTE collection[kindNum];
        for (i1 = 0, i2 = 0; i1 < 9; i1++) {
            if (isExist[i1]) {
                collection[i2] = i1;
                i2++;
            }
        }

        /*
         * flush: delete note numbers of unselected cells existing in the collection
         */
        for (i1 = 0, ncp1 = cellGuard->next; i1 < count; i1++, ncp1 = ncp1->next) {
            if (subset[i1] == 0) {
                for (i2 = 0; i2 < kindNum; i2++) {
                    ncp1->cell->note[collection[i2]] = 0;
                }
            }
        }

        // output the condition of notes before the deleting of cell(s)
        // for (ncp1 = cellGuard->next; ncp1 != cellGuard; ncp1 = ncp1->next) {
        //     for (i1 = 0; i1 < 9; i1++) {
        //         if (ncp1->cell->note[i1]) {
        //             printf("%i", i1 + 1);
        //         }
        //     }
        //     printf(";");
        // }
        // puts("");

        /*
         * remove and mark the specific cell(s)
         */
        int noteNum;
        BYTE lastNote;
        for (i1 = 0, ncp1 = cellGuard->next; ncp1 != cellGuard; ) {
            if (subset[i1] == 1) {
                /*
                 * count the number of note numbers of the cell -> noteNum
                 * make the statistics to find the last note number -> lastNote
                 */
                for (i2 = 0, noteNum = 0; i2 < 9; i2++) {
                    if (ncp1->cell->note[i2] != 0) {
                        lastNote = i2;
                        noteNum++;
                    }
                }
                /*
                 * flush: remove (from the guard list) and mark (with the answer) the
                 * - selected cell(s) with single note number
                 */
                if (noteNum == 0) {
                    puts("There is something wrong with the sudoku.");
                    exit(0);
                } else if (noteNum == 1) {
                    ncp1->cell->value = lastNote + 1;
                    ncp1->prev->next = ncp1->next;
                    ncp1->next->prev = ncp1->prev;
                    ncp2 = ncp1;
                    ncp1 = ncp1->next;
                    i1++;
                    free(ncp2);
                    count--;
                    // indicate the minus of uncompleted cells
                    // printf("--\n");
                    continue;
                }
            }
            ncp1 = ncp1->next;
            i1++;
        }

        // output the condition of notes after the deleting of cell(s)
        // printf("flushed: ");
        // for (ncp1 = cellGuard->next; ncp1 != cellGuard; ncp1 = ncp1->next) {
        //     for (i1 = 0; i1 < 9; i1++) {
        //         if (ncp1->cell->note[i1]) {
        //             printf("%i", i1 + 1);
        //         }
        //     }
        //     printf(";");
        // }
        // puts("");

    }
    return count;
}
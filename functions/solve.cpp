void solve(void) {

    int i1, i2, i3;
    NoteCell *ncp1 = NULL;

    Cell *cells[9] = {}, cellsInStack[9][9] = {};
    for (i1 = 0; i1 < 9; i1++) {
        cells[i1] = cellsInStack[i1];
    }
    input(cells);
    fillNote(cells);

    NoteCell noteCellGuardList[27] = {};
    createNoteCellGuard(noteCellGuardList, cells);

    // output test
    // for (i1 = 0; i1 < 9; i1++) {
    //     for (i2 = 0; i2 < 9; i2++) {
    //         printf("%i;", cells[i1][i2].value);
    //         for (i3 = 0; i3 < 9; i3++) {
    //             if (cells[i1][i2].note[i3]) {
    //                 printf("%i", i3 + 1);
    //             }
    //         }
    //         puts(";");
    //     }
    // }
    // exit(0);
    // for (i1 = 0; i1 < 27; i1++) {
    //     ncp1 = noteCellGuardList[i1].next;
    //     while (ncp1 != &noteCellGuardList[i1]) {
    //         for (i2 = 0; i2 < 9; i2++) {
    //             if (ncp1->cell->note[i2]) {
    //                 printf("%i", i2 + 1);
    //             }
    //         }
    //         printf(";");
    //         ncp1 = ncp1->next;
    //     }
    //     printf("\n");
    // }
    // exit(0);

    Cell lastCells[9][9] = {};
    memcpy(lastCells, cellsInStack, sizeof (cellsInStack));
    
    while (1) {

        /*
         * flush cells in the order of noteCellGuardList
         */
        for (i1 = 0; i1 < 27; i1++) {
            i2 = 0;
            ncp1 = noteCellGuardList[i1].next;
            while (ncp1 != &noteCellGuardList[i1]) {
                i2++;
                ncp1 = ncp1->next;
            }
            if (i2 != 0) {
                flushCell(&noteCellGuardList[i1], NULL, i2, 0);
            }
            // output test
            // puts("finished!");
            // ncp1 = noteCellGuardList[i1].next;
            // while (ncp1 != &noteCellGuardList[i1]) {
            //     for (i2 = 0; i2 < 9; i2++) {
            //         if (ncp1->cell->note[i2]) {
            //             printf("%i", i2 + 1);
            //         }
            //     }
            //     printf(";");
            //     ncp1 = ncp1->next;
            // }
            // printf("\n");
        }

        /*
         * detect if the function flushCell() did nothing
         */
        if (memcmp(lastCells, cellsInStack, sizeof (cellsInStack)) == 0) {
            puts("Unfinished! Can't further solve the sudoku.");
            break;
        } else {
            memcpy(lastCells, cellsInStack, sizeof (cellsInStack));
        }

        /*
         * detect if all cells were filled (means that the sudoku is solved)
         */
        for (i1 = 0, i3 = 0; i1 < 9; i1++) {
            for (i2 = 0; i2 < 9; i2++) {
                if (cells[i1][i2].value == 0) {
                    i3++;
                }
            }
        }
        if (i3 == 0) {
            puts("Successfully solve the sudoku!");
            break;
        }

    }

    output(cells);
    
}
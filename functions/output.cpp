void output(Cell **cells) {
    
    int i1, i2, i3;

    for (i1 = 0; i1 < 9; i1++) {
        for (i2 = 0; i2 < 9; i2++) {
            printf("%i;", cells[i1][i2].value);
            for (i3 = 0; i3 < 9; i3++) {
                if (cells[i1][i2].note[i3]) {
                    printf("%i", i3 + 1);
                }
            }
            puts(";");
        }
    }
    for (i1 = 0; i1 < 9; i1++) {
        for (i2 = 0; i2 < 9; i2++) {
            printf("%i", cells[i1][i2].value);
        }
        puts("");
    }
    
}
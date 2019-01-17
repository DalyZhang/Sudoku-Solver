void input(Cell **cells) {

    int i1, i2;
    char c1;

    for (i1 = 0; i1 < 9; i1++) {
        for (i2 = 0; i2 < 9; i2++) {
            while (1) {
                c1 = getchar();
                if (c1 == EOF) {
                    printf("end");
                    exit(0);
                } else if (c1 <= '9' && c1 >= '0') {
                    break;
                }
            }
            cells[i1][i2] = {(BYTE)(c1 - '0')};
        }
    }

}
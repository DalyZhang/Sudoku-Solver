void destorySudokuList(SudokuList *list) {

	int i1;

	for (i1 = 0; i1 < list->count; i1++) {
		free(list->list[i1]);
	}
	list->count = 0;

}
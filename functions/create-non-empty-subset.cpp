Subset createNonEmptySubset(int elementCount) {

	static BoolList *exist = NULL;
	int i1;

	if (exist != NULL) {
		free(exist);
	}
	int count = (1 << elementCount) - 1;
	exist = (BoolList *)calloc(count, sizeof (BoolList));
	for (i1 = 0; i1 < count; i1++) {
		exist[i1] = (BoolList)(i1 + 1);
	}
	Subset subset = {count, exist};
	return subset;

}
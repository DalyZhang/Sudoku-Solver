Subset createNonEmptySubset(int elementCount) {

	static short *exist = NULL;
	int i1;

	if (exist != NULL) {
		free(exist);
	}
	int count = (1 << elementCount) - 1;
	exist = (short *)calloc(count, sizeof (short));
	for (i1 = 0; i1 < count; i1++) {
		exist[i1] = (short)(i1 + 1);
	}
	Subset subset = {count, exist};
	return subset;

}
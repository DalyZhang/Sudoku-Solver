void destroySolution(Solution *solution) {

	int i1, i2;
	CheckNode *cnp1 = NULL, *cnp2 = NULL;

	for (i1 = 0; i1 < 3; i1++) {
		for (i2 = 0; i2 < SD_S; i2++) {
			cnp1 = solution->area[i1][i2].sentinel.next;
			while (cnp1->block != NULL) {
				cnp2 = cnp1;
				cnp1 = cnp1->next;
				free(cnp2);
			}
		}
	}

}
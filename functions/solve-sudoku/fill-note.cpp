FillNoteStatus fillNote(SolutionBlock **blocks) {
	
	int i1, i2, i3, i4, i5, i6;

	SolutionBlock *block = NULL;

	for (i1 = 0; i1 < SD_S; i1++) {
		for (i2 = 0; i2 < SD_S; i2++) {
			block = &blocks[i1][i2];
			if (block->value == 0) {
				block->note = ~(~0 << SD_S);
				for (i3 = 0; i3 < SD_S; i3++) {
					block->note &= ~(1 << (blocks[i1][i3].value - 1));
					block->note &= ~(1 << (blocks[i3][i2].value - 1));
				}
				for (i3 = i5 = i1 / SD_O * SD_O; i3 < i5 + SD_O; i3++) {
					for (i4 = i6 = i2 / SD_O * SD_O; i4 < i6 + SD_O; i4++) {
						block->note &= ~(1 << (blocks[i3][i4].value - 1));
					}
				}
				if (block->note == 0) {
					return FILL_NOTE_STATUS__ERROR;
				}
			}
		}
	}

	return FILL_NOTE_STATUS__SUCCESS;

}
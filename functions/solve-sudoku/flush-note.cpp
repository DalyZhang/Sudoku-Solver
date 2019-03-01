int countExist(short exist) {

	int i1, i2;

	for (i1 = 0, i2 = 0; i1 < 9; i1++, exist >>= 1) {
		if (exist & 1) {
			i2++;
		}
	}

	return i2;

}

void printExist(short exist) {

	int i1 = 0;
	
	for (i1 = 0; i1 < 9; i1++, exist >>= 1) {
		printf("%i", exist & 1);
	}

}

FlushNoteStatus flushNote(CheckList *list, short exist) {

	int i1, i2, i3, i4;
	CheckNode *cnp1 = NULL, *cnp2 = NULL;
	CheckList *clp1 = NULL;
	short s1;

	/**
	 * existCount -> the number of blocks selected by `exist`
	 * 
	 * collection -> the boolean list designating the existing
	 * -> note numbers in selected blocks
	 */
	int existCount;
	short collection = 0;
	cnp1 = list->sentinel.next;
	for (i1 = i2 = 0, s1 = exist; i1 < list->count; i1++, s1 >>= 1) {
		if (s1 & 1) {
			i2++;
			collection |= cnp1->block->note;
		}
		cnp1 = cnp1->next;
	}
	existCount = i2;

	/**
	 * numCount -> the number of different note numbers in
	 * -> selected blocks
	 */
	int numCount = countExist(collection);

	/**
	 * existBlockList -> the array of pointers pointing to
	 * -> selected blocks
	 */
	SolutionBlock *existBlockList[existCount];
	cnp1 = list->sentinel.next;
	for (i1 = i2 = 0, s1 = exist; i1 < list->count; i1++, s1 >>= 1) {
		if (s1 & 1) {
			existBlockList[i2] = cnp1->block;
			i2++;
		}
		cnp1 = cnp1->next;
	}
	
	/*
     * flush note numbers of unselected blocks when `existCount`
	 * -> = `numCount`
     */
	if (existCount != numCount) {
		return FLUSH_NOTE_STATUS__CONTINUE;
	}

	/**
	 * sameArea -> the array of boolean vars which designating if
	 * -> all selected blocks are in the same row, column, subgrid
	 */
	char sameArea[3] = {};
	for (i1 = 0; i1 < 3; i1++) {
		sameArea[i1] = 1;
		if (list->type != i1) {
			cnp1 = list->sentinel.next;
			clp1 = NULL;
			for (i2 = 0; i2 < list->count; i2++) {
				if (exist & (1 << i2)) {
					if (cnp1->block->area[i1] != clp1 && clp1 != NULL) {
						sameArea[i1] = 0;
						break;
					}
					clp1 = cnp1->block->area[i1];
				}
				cnp1 = cnp1->next;
			}
		}
	}
	
	/**
	 * flush note numbers of unselected blocks
	 * 
	 * flushedList -> the list where note numbers should be flushed
	 * 
	 * restNoteNumCount -> the var temporarily storing the number of note
	 * -> numbers in a flushed block
	 * 
	 * lastNum -> the var temporarily recording one of the note number
	 * -> in a flushed block, it is specially the last one due to the
	 * -> implementation
	 * 
	 * thisExist -> the boolean list designating where selected blocks
	 * -> exist in the `flushedList`
	 * 
	 * status -> the var temporarily storing the flush status
	 */
	CheckList *flushedList = NULL;
	int restNoteNumCount;
	char lastNum;
	short thisExist;
	FlushNoteStatus status = FLUSH_NOTE_STATUS__CONTINUE;
	for (i1 = 0; i1 < 3; i1++) {
		
		/**
		 * start flushing when all selected blocks are in the same
		 * -> row, column, subgrid
		 */
		if (!sameArea[i1]) {
			continue;
		}

		/**
		 * indicate the flushed list by one of the selected blocks
		 */
		flushedList = &existBlockList[0]->area[i1][0];

		/**
		 * calculate the value of `thisExist`
		 */
		if (list->type == i1) {
			thisExist = exist;
		} else {
			thisExist = 0;
			cnp1 = flushedList->sentinel.next;
			for (i2 = 0; i2 < flushedList->count; i2++) {
				for (i3 = 0; i3 < existCount; i3++) {
					if (cnp1->block == existBlockList[i3]) {
						thisExist |= 1 << i2;
						break;
					}
				}
				cnp1 = cnp1->next;
			}
		}

		/**
		 * start flushing on the unselected blocks, delete note
		 * -> numbers existing in the collection
		 */
		cnp1 = flushedList->sentinel.next;
		for (i2 = 0; i2 < flushedList->count; i2++) {
			if (!(thisExist & (1 << i2))) {
				cnp1->block->note &= ~collection;
			}
			cnp1 = cnp1->next;
		}

		/*
         * remove and mark the specific blocks
         */
		cnp1 = flushedList->sentinel.next;
		for (i2 = 0; cnp1 != &flushedList->sentinel; i2++) {
			if (thisExist & (1 << i2)) {

				/**
				 * calculate the value of `lastNum` and `restNoteNumCount`
				 */
				for (i3 = i4 = 0, s1 = cnp1->block->note; i3 < 9; i3++, s1 >>= 1) {
					if (s1 & 1) {
						i4++;
						lastNum = (char)(i3 + 1);
					}
				}
				restNoteNumCount = i4;

				/**
				 * `restNoteNumCount` = 0 indicates that the block has no note
				 * -> numbers, means that the block cannot be marked with any
				 * -> value number and the sudoku is self-contradictory
				 * 
				 * `restNoteNumCount` = 1 indicates that the block has only one
				 * -> note number, means that the note number is the only
				 * -> possible value number of the block, in this case, 
				 * -> the block should be marked with the only note number
				 * -> and removed from the current flushed list
				 */
				if (restNoteNumCount == 0) {
					return FLUSH_NOTE_STATUS__ERROR;
				} else if (restNoteNumCount == 1) {
					if (list->type == i1) {
						status = FLUSH_NOTE_STATUS__RECHECK;
					}
					flushedList->count--;
					cnp1->block->value = lastNum;
					cnp2 = cnp1;
					cnp1->prev->next = cnp1->next;
					cnp1->next->prev = cnp1->prev;
					cnp1 = cnp1->next;
					free(cnp2);
					continue;
				}

			}
			cnp1 = cnp1->next;
		}

	}

	return status;

}
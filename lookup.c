#include "lookup.h"

static int currTime = 0;

uint8_t getTrainNum(train_lookup *tl_ptr, uint8_t algNumIn) {
  uint8_t i = 0;

	for (i = 0; i < MAXTRAINS; i++) {
		if (tl_ptr[i].algNum == algNumIn) {
			return tl_ptr[i].trainNum;
		}
	}
	
	return LOOKUP_FAILURE;
}

uint8_t getTrainSlot(train_lookup *tl_ptr, uint8_t trainNumIn) {
	uint8_t i = 0;

	for (i = 0; i < MAXTRAINS; i++) {
		if (tl_ptr[i].trainNum == trainNumIn) {
			return tl_ptr[i].trainSlot;
		}
	}
	
	return LOOKUP_FAILURE;
}

void setTrainData(train_lookup *tl_ptr, uint8_t *data_from_PIC, uint8_t offset) {
	int8_t min = 127;
	uint8_t i = 0;
    uint8_t minI = 0;
    uint8_t trainFound = 0;
	
	// Loop that finds the train if it exists
	for (i = 0; i < MAXTRAINS; i++) {
		if (tl_ptr[i].trainNum == data_from_PIC[offset+4] 
			&& tl_ptr[i].isSet == 0) {
			tl_ptr[i].trainSlot = data_from_PIC[offset+2];
			tl_ptr[i].accessTime = currTime++;
			tl_ptr[i].isSet = 1;
			trainFound = 1;
			break;
		}
	}
	// Loop that stores the train if it doesn't exist
	if (!trainFound) {
		for (i = 0; i < MAXTRAINS; i++) {
			if (tl_ptr[i].isSet == 0) {
				tl_ptr[i].trainNum = data_from_PIC[offset+4];
				tl_ptr[i].trainSlot = data_from_PIC[offset+2];
				tl_ptr[i].accessTime = currTime++;
				tl_ptr[i].isSet = 1;
				trainFound = 1;
				break;
			}
		}
	}
	if (!trainFound) {
		// Loop that stores the train if array is full
		for (i = 0; i < MAXTRAINS; i++) {
			if (tl_ptr[i].accessTime < min) {
				min = tl_ptr[i].accessTime;
				minI = i;
			}
		}
		tl_ptr[minI].trainNum = data_from_PIC[offset+4];
		tl_ptr[minI].trainSlot = data_from_PIC[offset+2];
		tl_ptr[minI].accessTime = currTime++;
		tl_ptr[minI].isSet = 1;
		trainFound = 1;
	}
}

void setTrainDataFromAlg(train_lookup *tl_ptr, uint8_t *data_from_Alg, uint8_t offset) {
	int i = 0;
	
	for (i = 0; i < MAXTRAINS; i++) {
		if (tl_ptr[i].isSet == 0) {
			tl_ptr[i].algNum = data_from_Alg[offset];
			break;
		}
	}

}

void initTrainData(train_lookup *tl_ptr) {
	tl_ptr[0].algNum = 1;
	tl_ptr[0].trainNum = 3;
	tl_ptr[0].trainSlot = 2;
	tl_ptr[0].isSet = 1;
	tl_ptr[1].algNum = 2;
	tl_ptr[1].trainNum = 10;
	tl_ptr[1].trainSlot = 1;
	tl_ptr[1].isSet = 1;
	tl_ptr[2].algNum = 3;
	tl_ptr[2].trainNum = 6;
	tl_ptr[2].trainSlot = 0;
	tl_ptr[2].isSet = 0;
	
}

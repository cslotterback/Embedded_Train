#ifndef __lookup_h
#define __lookup_h

#include "FreeRTOS.h"

#define MAXTRAINS 3
#define LOOKUP_SUCCESS 1
#define LOOKUP_FAILURE -1
#define LOOKUP_FULL -2

typedef struct __train_lookup {
  uint8_t algNum;
    uint8_t trainNum;
    uint8_t trainSlot;
    uint8_t isSet;
    int8_t accessTime;
} train_lookup;

void setTrainData(train_lookup *tl_ptr, uint8_t *data_from_PIC, uint8_t offset);
void initTrainData(train_lookup *tl_ptr);
uint8_t getTrainNum(train_lookup *tl_ptr, uint8_t algNumIn);
uint8_t getTrainSlot(train_lookup *tl_ptr, uint8_t trainNum);


#endif

#include "loco_comm.h"

#include <stdio.h>
#include <stdlib.h>

int8_t isValidOpCode(uint8_t opCode) {
    switch (opCode) {
        case OPC_LOCO_ADR:
        case OPC_LOCO_SPD:
        case OPC_SL_RD_DATA:
        case OPC_MOVE_SLOTS:
        case OPC_LOCO_SND:
        case OPC_LOCO_DIRF:
        case OPC_IDLE:
        case OPC_GPON:
        case OPC_GPOFF:
        case OPC_BUSY:
        case OPC_SW_ACK:
        case OPC_SW_STATE:
        case OPC_RQ_SL_DATA:
        case OPC_LINK_SLOTS:
        case OPC_UNLINK_SLOTS:
        case OPC_CONSIST_FUNC:
        case OPC_SLOT_STAT1:
        case OPC_LONG_ACK:
        case OPC_INPUT_REP:
        case OPC_SW_REP:
        case OPC_SW_REQ:
            return LOCO_SUCCESS;
        default:
            return LOCO_INVALID_OPCODE;
    }

  return LOCO_GENERAL_ERR;
}

int8_t opLength(uint8_t opCode, uint8_t nextByte) {
    uint8_t length = (opCode & 0xE0);
    if (length == 0x80)
        return 2;
    else if (length == 0xA0)
        return 4;
    else if (length == 0xC0)
        return 6;
    else if (length == 0xE0)
        return (int) nextByte;

    return LOCO_INVALID_LEN;
}

uint8_t checkSumGen(uint8_t *toCheck, uint8_t size, uint8_t offset) {
    uint8_t checkSum = 0x00;
    for (; offset < size; offset++) {
        checkSum = checkSum ^ toCheck[offset];
    }
    checkSum = ~checkSum;
    return checkSum;
}

void requestLoco(uint8_t trainNumber, uint8_t *toSend, uint8_t offset) {
    toSend[offset] = OPC_LOCO_ADR;
    toSend[offset+1] = 0x00;
    toSend[offset+2] = trainNumber;
    toSend[offset+3] = checkSumGen(toSend, offset+4, offset);
}

void setActive(uint8_t slot, uint8_t *toSend, uint8_t offset) {
    toSend[offset] = OPC_MOVE_SLOTS;
    toSend[offset+1] = slot;
    toSend[offset+2] = slot;
    toSend[offset+3] = checkSumGen(toSend, offset+4, offset);
}

void setTrainDirection(uint8_t slot, uint8_t direction, uint8_t *toSend, uint8_t offset) {
    toSend[offset] = OPC_LOCO_DIRF;
    toSend[offset+1] = slot;
    toSend[offset+2] = direction;
    toSend[offset+3] = checkSumGen(toSend, offset+4, offset);
}

void setTrainForward(uint8_t slot, uint8_t speed, uint8_t *toSend, uint8_t offset) {
    toSend[offset] = OPC_LOCO_SPD;
    toSend[offset+1] = slot;
    toSend[offset+2] = speed;
    toSend[offset+3] = checkSumGen(toSend, offset+4, offset);
}

void setTrainFree(uint8_t trainNumber, uint8_t slot, uint8_t *toSend, uint8_t offset) {
	toSend[offset] = OPC_WR_SL_DATA;
	toSend[offset+2] = slot;
	toSend[offset+4] = trainNumber;
	toSend[offset+13] = checkSumGen(toSend, offset+14, offset);
}

void setSwitchClosed(uint8_t switchNumber, uint8_t *toSend, uint8_t offset) {
	toSend[offset] = OPC_SW_REQ;
	toSend[offset+1] = switchNumber;
	toSend[offset+2] = 0x30;
	toSend[offset+3] = checkSumGen(toSend, offset+4, offset);
}

void setSwitchThrown(uint8_t switchNumber, uint8_t *toSend, uint8_t offset) {
	toSend[offset] = OPC_SW_REQ;
	toSend[offset+1] = switchNumber;
	toSend[offset+2] = 0x10;
	toSend[offset+3] = checkSumGen(toSend, offset+4, offset);
}

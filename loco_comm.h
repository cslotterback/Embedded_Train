#ifndef __loco_comm
#define __loco_comm

#include "FreeRTOS.h"

#define NUMBER_OF_TRAINS 2
#define LOCO_SUCCESS 1
#define LOCO_INVALID_LEN -1
#define LOCO_INVALID_OPCODE -2
#define LOCO_GENERAL_ERR -3

// Valid Loconet opcode definitions
#define OPC_IDLE 0x85
#define OPC_GPON 0x83
#define OPC_GPOFF 0x82
#define OPC_BUSY 0x81
#define OPC_LOCO_ADR 0xBF
#define OPC_SW_ACK 0xBD
#define OPC_SW_STATE 0xBC
#define OPC_RQ_SL_DATA 0xBB
#define OPC_MOVE_SLOTS 0xBA
#define OPC_LINK_SLOTS 0xB9
#define OPC_UNLINK_SLOTS 0xB8
#define OPC_CONSIST_FUNC 0xB6
#define OPC_SLOT_STAT1 0xB5
#define OPC_LONG_ACK 0xB4
#define OPC_INPUT_REP 0xB2
#define OPC_SW_REP 0xB1
#define OPC_SW_REQ 0xB0
#define OPC_LOCO_SND 0xA2
#define OPC_LOCO_DIRF 0xA1
#define OPC_LOCO_SPD 0xA0
#define OPC_WR_SL_DATA 0xEF
#define OPC_SL_RD_DATA 0xE7
#define OPC_PEER_XFER 0xE5
#define OPC_IMM_PACKET 0xED

int8_t isValidOpCode(uint8_t opCode);
int8_t opLength(uint8_t opCode, uint8_t nextByte);
uint8_t checkSumGen(uint8_t *toCheck, uint8_t size, uint8_t offset);
void requestLoco(uint8_t trainNumber, uint8_t *toSend, uint8_t offset);
void setActive(uint8_t slot, uint8_t *toSend, uint8_t offset);
void setTrainDirection(uint8_t slot, uint8_t direction, uint8_t *toSend, uint8_t offset);
void setTrainForward(uint8_t slot, uint8_t speed, uint8_t *toSend, uint8_t offset);
void setTrainFree(uint8_t trainNumber, uint8_t slot, uint8_t *toSend, uint8_t offset);

#endif

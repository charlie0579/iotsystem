/******************************************
* File: plantformMessage.h
* Description: message struture difine, message id define.
* Author : William								
* Data: 4-1-2016
************************************************/
#ifndef _PLANTFORM_MESSAGE_H
#define _PLANTFORM_MESSAGE_H
#include "includes.h"
#include <stdint.h>

#define RECEIVE_SIM900_MESSAGE 0x0001

typedef enum ECommonMessageStatus
{
    ECommonMessageStatus_NotDefine,
    ECommonMessageStatus_Ok,
    ECommonMessageStatus_InvalidLength,
    ECommonMessageStatus_Error

}ECommonMessageStatus;
typedef struct commonMessage
{
    uint16_t messageId;
    uint16_t control;
    char Data[50];
    ECommonMessageStatus status;
}commonMessage;

extern void MboxPost(commonMessage *MessagePtr);
extern void MboxPend(commonMessage *MessagePtr);

#endif

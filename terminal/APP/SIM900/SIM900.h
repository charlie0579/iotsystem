/**************************************************************
* File: Usart.h
* Description; Ues for usart
* Author: William
* Date 05-01-2016
******************************************************************/
#ifndef _SIM900_H
#define _SIM900_H

#include "includes.h"
#include "stm32f10x.h"
#include "APPIncludes.h"

#define	SIM900_STK_SIZE		1000
#define Sim900AReceiveBuffSize  300

void SIM900Process(void *arg);
extern OS_STK  SIM900ProcessStk[SIM900_STK_SIZE];

///////////////////////
#define false        0
#define true         1

#define MATCHTARGET0   0
#define MATCHTARGET1   1
#define MATCHTARGET2   2
#define MATCHTARGET3   3
#define MATCHTARGET12  12
#define MATCHTARGET13  13
#define MATCHTARGET23  23
#define MATCHTARGET123 123
typedef struct Buff
{
    char* buffP;
    int buffSize;
}Buff;

typedef enum
{
	ESim900NoInit                = 0,
	ESim900Ready                 = 1,
	ESim900GPRSConnect           = 2,
	ESim900GPRSDisConnect        = 3,
	Esim900GPRSSendReceiveMsg    = 4,
	Esim900GPRSSendFault         = 5,
	
}ESIM900Status;

typedef enum
{
	ELedInit          = 0,
	ELedOpen          = 1,
	ELedClose         = 2,
}ELEDStatus;

extern char mqtt_message[100];
extern char Sim900AReceiveBuff[Sim900AReceiveBuffSize];
extern u8 tempBuffP;
extern Buff SIM900ReveBuff;

extern void ClearSim900AReceiveBuff(void);
extern u8 ConfirmIfEndReceive(Buff srcBuff, char* targetBuff1,char* targetBuff2, char* targetBuff3);
extern void CheckLEDCommand(void);
extern void ConfirmLEDCommand(void);
extern void GPRSSendMsg(char* SendString, int StringSize);
extern void GPRSConnect(u32 DelayTimeMs);
extern void CheckSIMStatus(void);
extern void LongDelayms(unsigned int uiDly);

extern void SendMsgToSim900AWithCount(char *buf, int count);
extern char* Sim900AReceiveData(int* RevBuffSize);
extern void SendMsgToSim900A(char *p);
extern u8 SendCharSim900A(u8 ch);
extern char* CheckTargetBuffReveived(Buff srcBuff, char* targetBuff1);
#endif


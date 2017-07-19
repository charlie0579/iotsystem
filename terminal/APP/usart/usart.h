/**************************************************************
* File: Usart.h
* Description; Ues for usart
* Author: William
* Date 01-01-2016
******************************************************************/
#ifndef __USART1_H
#define	__USART1_H

#include "includes.h"
#include "stm32f10x.h"
#include <stdio.h>
#include "APPIncludes.h"

#define USARTBITS115200  115200
#define USARTBITS9600    9600


/************set stack size£¨OS_STK £©************/
#define	USART_STK_SIZE		200

extern OS_STK UsartProcessStk[USART_STK_SIZE];



void USART1_Config(void);
void USART2_Config(void);
void USART3_Config(void);
int fputc(int ch, FILE *f);
void USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
void UsartProcess(void *arg);

bool Usart1DateHandle(char* _Date);

extern bool         FlagUsart1ReceiveDate;
extern bool         FlagUsart1ReceiveDateDone;
extern bool         FlagUsart2ReceiveDate;
extern bool         FlagUsart2ReceiveDateDone;
extern bool         FlagUsart3ReceiveDate;
extern bool         FlagUsart3ReceiveDateDone;
extern  char *itoa(int value, char *string, int radix);
#endif /* __USART1_H */

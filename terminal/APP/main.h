#ifndef _MAIN_H
#define _MAIN_H
#include "includes.h"
#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include "APPIncludes.h"


const uint32_t SystemFrequency         = 72000000;      /*!< System Clock Frequency (Core Clock) */


#define START_TASK_PRIO         4	 
#define	USART_PROCESS_PRIO   	6
#define	SIM900_PROCESS_PRIO   	7
#define DS18B20_TASK_PRIO       60
#define	LEDRUN_TASK_PRIO	    59 
 
/************set stack size£¨OS_STK £©************/
#define START_TASK_STK_SIZE   200  

OS_STK start_task_stk[START_TASK_STK_SIZE];	

#endif

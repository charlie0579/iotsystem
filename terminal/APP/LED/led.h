#ifndef __LED_H
#define	__LED_H

#include "includes.h"
#include "stm32f10x.h"
#include "../APP/GPIOInput/GpioInput.h"
/* the macro definition to trigger the led on or off 
 * 1 - off
 - 0 - on
 */
#define ON  0
#define OFF 1

  
 
/************set stack size（OS_STK ）************/

#define	LEDRUN_STK_SIZE		200 

//带参宏，可以像内联函数一样使用
#define LED1(a)	if (a)	\
					GPIO_SetBits(GPIOC,GPIO_Pin_12);\
					else		\
					GPIO_ResetBits(GPIOC,GPIO_Pin_12)

#define LED2(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_8);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_8)

#define LED3(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_8);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_8)

#define LED4(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_10);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_10)

void LEDGPIOConfig(void);
void LEDRunningTask(void *arg);
extern OS_STK LEDRunningTaskStk[LEDRUN_STK_SIZE];

#endif /* __LED_H */

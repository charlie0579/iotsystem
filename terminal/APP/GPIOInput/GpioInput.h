#ifndef __GPIOINPUT_H_
#define	__GPIOINPUT_H_

#include "includes.h"
#include "stm32f10x.h"

//���κ꣬��������������һ��ʹ��

#define  GPIOC2_DATA_IN	   GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)
#define  GPIOC4_DATA_IN	   GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)
void GPIOInputConfig(void);

#endif /* __LED_H */

/***********************************************
 * File  ：led.c
 * Description   ：led         
 * Author  : William
***********************************************/
#include "GpioInput.h"
#include "ucos_ii.h"

 
void GPIOInputConfig(void)
{		
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启GPIOC的外设时钟*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE); 	

	/*设置上拉输入*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   

    /*选择要控制的GPIOC引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_4;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
 
}


/******************* (C) COPYRIGHT 2015 *****END OF FILE************/

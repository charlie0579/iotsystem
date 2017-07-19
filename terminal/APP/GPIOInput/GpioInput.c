/***********************************************
 * File  ��led.c
 * Description   ��led         
 * Author  : William
***********************************************/
#include "GpioInput.h"
#include "ucos_ii.h"

 
void GPIOInputConfig(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*����GPIOC������ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE); 	

	/*������������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   

    /*ѡ��Ҫ���Ƶ�GPIOC����*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_4;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
 
}


/******************* (C) COPYRIGHT 2015 *****END OF FILE************/

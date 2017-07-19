/***********************************************
 * File  ��led.c
 * Description   ��led         
 * Author  : William
***********************************************/
#include "led.h"
#include "ucos_ii.h"

OS_STK LEDRunningTaskStk[LEDRUN_STK_SIZE];
/*
 * ��������LED_GPIO_Config
 * ����  ������LED�õ���I/O��
 * ����  ����
 * ���  ����
 */
 
void LEDGPIOConfig(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*����GPIOC������ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE); 	

	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

    /*ѡ��Ҫ���Ƶ�GPIOC����*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_10;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);	    

	/* �ر�����led��	*/
	GPIO_ResetBits(GPIOC, GPIO_Pin_12);
    GPIO_ResetBits(GPIOB, GPIO_Pin_8|GPIO_Pin_10);
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);

}


void LEDRunningTask(void *arg)
{
    int temp = 0;
    (void)arg;
    LEDGPIOConfig();
    GPIOInputConfig();

    while (1)
    {
        for(; temp <= 1000; temp++)
        {
            LED1(1);
            LED2(1);
            OSTimeDlyHMSM(0, 0,0,500);					  
            LED1(0);
            LED2(0);           
            OSTimeDlyHMSM(0, 0,0,500);
        }
        if(GPIOC2_DATA_IN)
        {
            LED2(0);
        }
        else
        {
            LED2(1);
        }
        if(GPIOC4_DATA_IN)
        {
            LED1(0);
        }
        else
        {
            LED1(1);
        }
        OSTimeDlyHMSM(0, 0,0,100);
    }
}


/******************* (C) COPYRIGHT 2015 *****END OF FILE************/

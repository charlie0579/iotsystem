/***********************************************
 * File  ：led.c
 * Description   ：led         
 * Author  : William
***********************************************/
#include "led.h"
#include "ucos_ii.h"

OS_STK LEDRunningTaskStk[LEDRUN_STK_SIZE];
/*
 * 函数名：LED_GPIO_Config
 * 描述  ：配置LED用到的I/O口
 * 输入  ：无
 * 输出  ：无
 */
 
void LEDGPIOConfig(void)
{		
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启GPIOC的外设时钟*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE); 	

	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

    /*选择要控制的GPIOC引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_10;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);	    

	/* 关闭所有led灯	*/
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

/******************** (C) COPYRIGHT 2015 STMicroelectronics ********************
* File Name          : main.c
* Author             : William
* Version            : V1.0.0
* Date               : 30/12/2015
* Description        : EasyLife
*******************************************************************************/
#include "main.h"
#include "stm32f10x.h"


void RCC_Configuration(void)  
{  
    RCC_DeInit();  
    RCC_HSEConfig(RCC_HSE_ON);  
    while(!RCC_WaitForHSEStartUp());  
  
    RCC_HCLKConfig(RCC_SYSCLK_Div1);  
    RCC_PCLK2Config(RCC_HCLK_Div1);  
    RCC_PCLK1Config(RCC_HCLK_Div2);  
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);  
    RCC_PLLCmd(ENABLE);  
    while(!(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==SET));  
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);  
    while(!(RCC_GetSYSCLKSource()==0x08));  

}

void StartUpTask(void *p_arg)
{
    (void)p_arg;       	


#if 0
    while( DS18B20_Init())	
    {

        printf("\r\n No ds18b20\r\n");
    }	
    printf("\r\n ds18b20 exit\r\n");
#endif
    while (1)
    {
#if 0
        OS_ENTER_CRITICAL();
        OSIntEnter();
        printf("William current temperature %.1f\r\n",(DS18B20_Get_Temp()-0.5));	
        OSIntExit();
		OS_EXIT_CRITICAL();
        OSSchedUnlock(); 
#endif        
		OSTimeDlyHMSM(0, 0,2,0);
    
    }
}

int main(void)
{
	SystemInit();		
    RCC_Configuration();
	SysTick_Config(SystemFrequency/OS_TICKS_PER_SEC);

/******** UCOSII init  ***********************************************************/
	OSInit();	/* UCOSII init */

	CreatePlantFormCenter();
	/* UCOSII create a startup task */		   
	OSTaskCreate(StartUpTask,(void *)0,&start_task_stk[START_TASK_STK_SIZE-1], START_TASK_PRIO);
    OSTaskCreate(UsartProcess,(void *)0,&UsartProcessStk[USART_STK_SIZE-1], USART_PROCESS_PRIO);
	OSTaskCreate(SIM900Process,(void *)0,&SIM900ProcessStk[SIM900_STK_SIZE-1], SIM900_PROCESS_PRIO);
    OSTaskCreate(DS18B20Task,(void *)0,&DS18B20TaskStk[DS18B20_STK_SIZE-1], DS18B20_TASK_PRIO);
    OSTaskCreate(LEDRunningTask,(void *)0,&LEDRunningTaskStk[LEDRUN_STK_SIZE-1], LEDRUN_TASK_PRIO);

	OSStart();	 	/* start OS */
/******** UCOSII end OS startup £ý ***********************************************************/
}


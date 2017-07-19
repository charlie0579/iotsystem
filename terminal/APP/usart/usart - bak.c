/******************** (C) COPYRIGHT 2015 ***************************
 * File  ：usart1.c
 * Description : Init uart. Can use macro  defind printf to USARTx        
 * Author :  William
**********************************************************************************/
#include "usart.h"
#include <stdarg.h>
#include "misc.h"
#include "plantformMessage.h"

#define USEUSART2 1
#define Usart1BuffSize  100  
#define Usart2BuffSize  100  
#define Usart3BuffSize  100 
OS_STK       UsartProcessStk[USART_STK_SIZE];
commonMessage ProcessMBoxmsg;


char         Usart1Buff[Usart1BuffSize];
INT8U        Usart1DateLength;
bool         FlagUsart1ReceiveDate;
bool         FlagUsart1ReceiveDateDone;
INT8U        Usart1ReceiveDateTimeout;

char         Usart2Buff[Usart2BuffSize];
INT8U        Usart2DateLength;
bool         FlagUsart2ReceiveDate;
bool         FlagUsart2ReceiveDateDone;
INT8U        Usart2ReceiveDateTimeout;

char         Usart3Buff[Usart3BuffSize];
INT8U        Usart3DateLength;
bool         FlagUsart3ReceiveDate;
bool         FlagUsart3ReceiveDateDone;
INT8U        Usart3ReceiveDateTimeout;
/*************************************************
 * Interface ：USART1_Config
 * Description  ：USART1 confige
 *************************************************/
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	  
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 
	USART_Cmd(USART1, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	/* enable RX interrupt */
}



/*************************************************
 * Interface ：USART2_Config
 * Description  ：USART2 confige
 *************************************************/

void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
//	USART_ClockInitTypeDef USART_ClockInitStructure;  
	
	/* config USART2 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	/* USART2 GPIO config */
	/* Configure USART22 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART2 Rx (PA.03) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    /* Enable the USART2 Interrupt */

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 
 

	USART_InitStructure.USART_BaudRate = USARTBITS115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure); 
	USART_Cmd(USART2, ENABLE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	/* enable RX interrupt */
}




/*************************************************
 * Interface ：USART3_Config
 * Description  ：USART3 confige
 *************************************************/

void USART3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	

	/* USART3 clock enable */  
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
   /* GPIOD clock enable */  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 
	
	  
	/* USART3 mode config */
	USART_InitStructure.USART_BaudRate = USARTBITS9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure); 
	USART_Cmd(USART3, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	/* enable RX interrupt */
}


/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USARTx
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
#ifdef USEUSART3
int fputc(int ch, FILE *f)
{
	/* 将Printf内容发往串口 */
	USART_SendData(USART3, (unsigned char) ch);
//	while (!(USART1->SR & USART_FLAG_TXE));
	while( USART_GetFlagStatus(USART3,USART_FLAG_TC)!= SET);	
	return (ch);
}
#endif

#ifdef USEUSART2
int fputc(int ch, FILE *f)
{
	/* 将Printf内容发往串口 */
	USART_SendData(USART2, (unsigned char) ch);
	while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
	return (ch);
}
#endif

#ifdef USEUSART1
int fputc(int ch, FILE *f)
{
	/* 将Printf内容发往串口 */
	USART_SendData(USART1, (unsigned char) ch);
	while (!(USART1->SR & USART_FLAG_TXE));	
	return (ch);
}
#endif



/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART1_printf()调用
 */
static char *itoa(int value, char *string, int radix)
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;
	
	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
	    *ptr = 0;
	    return string;
	}
	
	if (!value)
	{
	    *ptr++ = 0x30;
	    *ptr = 0;
	    return string;
	}
	
	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
	    *ptr++ = '-';
	
	    /* Make the value positive. */
	    value *= -1;
	}
	
	for (i = 10000; i > 0; i /= 10)
	{
	    d = value / i;
	
	    if (d || flag)
	    {
	        *ptr++ = (char)(d + 0x30);
	        value -= (d * i);
	        flag = 1;
	    }
	}
	
	/* Null terminate the string. */
	*ptr = 0;
	
	return string;

} /* NCL_Itoa */

/*
 * 函数名：USART1_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口1，即USART1
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART1_printf( USART1, "\r\n this is a demo \r\n" );
 *            		 USART1_printf( USART1, "\r\n %d \r\n", i );
 *            		 USART1_printf( USART1, "\r\n %s \r\n", j );
 */
void USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
{
	const char *s;
	int d;   
	char buf[16];
	
	va_list ap;
	va_start(ap, Data);
	
	while ( *Data != 0)     // 判断是否到达字符串结束符
	{				                          
		if ( *Data == 0x5c )  //'\'
	{									  
	switch ( *++Data )
	{
		case 'r':							          //回车符
			USART_SendData(USARTx, 0x0d);
			Data ++;
		break;
		
		case 'n':							          //换行符
			USART_SendData(USARTx, 0x0a);	
			Data ++;
		break;
		
		default:
			Data ++;
		break;
	}			 
	}
	else if ( *Data == '%')
	{									  //
	switch ( *++Data )
	{				
		case 's':										  //字符串
			s = va_arg(ap, const char *);
	for ( ; *s; s++) 
	{
		USART_SendData(USARTx,*s);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
		Data++;
		break;
	
	case 'd':										//十进制
	d = va_arg(ap, int);
	itoa(d, buf, 10);
	for (s = buf; *s; s++) 
	{
		USART_SendData(USARTx,*s);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
	Data++;
	break;
		 default:
				Data++;
		    break;
	}		 
	} /* end of else if */
	else USART_SendData(USARTx, *Data++);
	while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
}


void USART1_IRQHandler(void)  
{
    u8 ch;
    (void)ch;
	OSIntEnter();		//inform OS enter Inturrupt
#if 1
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);//清中断标志

		ch=USART1->DR;
		USART_SendData(USART2, (uint8_t) ch); 	    
	}	
#endif    
#if 0
    if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)  
    {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);//清中断标志
        FlagUsart1ReceiveDate = TRUE;
        Usart1Buff[Usart1DateLength] = USART_ReceiveData(USART1);
//        USART_SendData(USART2,Usart1Buff[Usart1DateLength]);
        if((Usart1BuffSize - 1) == Usart1DateLength)
        {
         Usart1DateLength = 0;
        }
        Usart1DateLength++;
    }
#endif
    OSIntExit();              
}

void USART2_IRQHandler(void)  
  
{
	u8 ch;
    
	OSIntEnter();		//inform OS enter Inturrupt

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{ 	
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);//清中断标志

		ch=USART2->DR;
		USART_SendData(USART1, (uint8_t) ch); 	    
	} 	
#if 0
    if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)  
    {
        FlagUsart2ReceiveDate = TRUE;
        Usart2Buff[Usart2DateLength] = USART_ReceiveData(USART2);
        USART_SendData(USART1,Usart2Buff[Usart2DateLength]);
        if((Usart2BuffSize - 1) == Usart2DateLength)
        {
         Usart2DateLength = 0;
        }
        Usart2DateLength++;
    }
#endif
    OSIntExit();          

}

void USART3_IRQHandler(void)  
  
{ 
	commonMessage msg;
	msg.messageId = RECEIVE_SIM900_MESSAGE;
	msg.status = ECommonMessageStatus_Ok; 
	OSIntEnter();		//inform OS enter Inturrupt
    if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)  
    {
        FlagUsart3ReceiveDate = TRUE;
        Usart3Buff[Usart3DateLength] = USART_ReceiveData(USART3);
        if((Usart3BuffSize - 1) == Usart3DateLength)
        {
         Usart3DateLength = 0;
        }
        Usart3DateLength++;
        OSMboxPost(CommonMsg, &msg);
    }
    OSIntExit();     
}

char ReceiveDate[Usart1BuffSize];
bool Usart1DateHandle(char* _Date)
{
	
    char CmpStringOk[Usart1BuffSize];
    char CmpStringError[Usart1BuffSize];
    bool ret = TRUE;
	ProcessMBoxmsg.messageId = RECEIVE_SIM900_MESSAGE;
    ProcessMBoxmsg.status = ECommonMessageStatus_Ok;
    strcpy(ReceiveDate, _Date);
    strcpy(CmpStringOk,"OK");
    strcpy(CmpStringError,"ERROR");
    memset(Usart1Buff,0,sizeof(char)*Usart1BuffSize);
    if(strstr(ReceiveDate,CmpStringError))
    {
    	printf("SIM900A error match\n");
        ProcessMBoxmsg.status = ECommonMessageStatus_Error;
        OSMboxPost(CommonMsg, &ProcessMBoxmsg);     
    }
    else if(strstr(ReceiveDate,CmpStringOk))
    {
        printf("SIM900A OK match\n");
        ProcessMBoxmsg.status = ECommonMessageStatus_Ok;
        OSMboxPost(CommonMsg, &ProcessMBoxmsg);  
    }
    else
    {
        printf("SIM900A Not define\n");
        ProcessMBoxmsg.status = ECommonMessageStatus_NotDefine;
        OSMboxPost(CommonMsg, &ProcessMBoxmsg);  
    }
    return ret;
}

void UsartProcess(void *arg)
{
    (void)arg;
    Usart1DateLength = 0;
    FlagUsart1ReceiveDateDone = FALSE;
    memset(Usart1Buff,0,sizeof(char)*Usart1BuffSize);
	USART1_Config();
    USART2_Config(); 

    while (1)
    {		
        // Usart1
        while(FlagUsart1ReceiveDate)
        {
            FlagUsart1ReceiveDate = FALSE;
            FlagUsart1ReceiveDateDone = TRUE;
            OSTimeDlyHMSM(0,0,0,900);
        }
        
        if(FlagUsart1ReceiveDateDone)
        {
            FlagUsart1ReceiveDateDone = FALSE;
            Usart1DateLength = 0;
            if(!Usart1DateHandle(Usart1Buff))
            {
            	printf("Handle Usart1 date Error\n");
            }
            printf("Receive date from Usart1\r\n");
            printf(Usart1Buff);
            printf("\r\n");
        }
#if 0
        // Usart2
        while(FlagUsart2ReceiveDate)
        {
            FlagUsart2ReceiveDate = FALSE;
            FlagUsart2ReceiveDateDone = TRUE;
            OSTimeDlyHMSM(0,0,0,10);
        }
        
        if(FlagUsart2ReceiveDateDone)
        {
            FlagUsart2ReceiveDateDone = FALSE;
            Usart2DateLength = 0;
            printf("Receive date from Usart2\r\n");
            printf(Usart2Buff);
            printf("\r\n");
        }
 #endif       
		OSTimeDlyHMSM(0,0,1,0);				 	   
    }
}

/******************* (C) COPYRIGHT 2015 *****END OF FILE************/


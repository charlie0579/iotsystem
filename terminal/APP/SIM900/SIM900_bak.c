/******************** (C) COPYRIGHT 2015 ***************************
 * File  £ºusart1.c
 * Description : Init uart. Can use macro  defind printf to USARTx        
 * Author :  William
**********************************************************************************/
#include "SIM900.h"
#include <stdarg.h>
#include "misc.h"
#include "plantformMessage.h"
#include "usart.h"

ESIMStatus SIMStatus;
commonMessage Receivemsg;
INT8U	      err;    	
OS_STK        SIM900ProcessStk[SIM900_STK_SIZE];

u8 SendCharSim900A(u8 ch)
{
	/* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}
void SendMsgToSim900A(unsigned char *p)
{
	while(*p != '\0')
	{
		SendCharSim900A(*p++);
	}
}
void CheckSIMStatus()
{
//	SendMsgToSim900A("\r\n");
    OSTimeDlyHMSM(0,0,0,10);
	SendMsgToSim900A("AT+CPIN?\r\n");
    OSTimeDlyHMSM(0,0,0,10);
}

void SIM900A_CIPCLOSE()
{
    OSTimeDlyHMSM(0,0,0,10);
	SendMsgToSim900A("AT+CIPCLOSE\r\n");
    OSTimeDlyHMSM(0,0,0,10);    
}
void SIM900A_CIPSHUT()
{
    OSTimeDlyHMSM(0,0,0,10);
	SendMsgToSim900A("AT+CIPSHUT\r\n");
    OSTimeDlyHMSM(0,0,0,10);    
}
void SIM900A_CIPHEAD()
{
    OSTimeDlyHMSM(0,0,0,10);
	SendMsgToSim900A("AT+CIPHEAD=1\r\n");
    OSTimeDlyHMSM(0,0,0,10);    
}
void HandleReceivedMsg()
{
   if(strstr(Receivemsg.Data,"+CPIN: READY"))
   {
       SIMStatus = ESimReady;
   }
   else if(strstr(Receivemsg.Data, "AT+CIPCLOSE") /*&& (strstr(Receivemsg.Data, "CLOSE OK"))*/)
   {
       SIMStatus = ESimCIPCLOSE;
   }
   else if(strstr(Receivemsg.Data, "AT+CIPSHUT") /*&& (strstr(Receivemsg.Data, "SHUT OK"))*/)
   {
       SIMStatus = ESimCIPSHUT;
   }
   else if(strstr(Receivemsg.Data, "AT+CIPHEAD") && (strstr(Receivemsg.Data, "OK")))
   {
       SIMStatus = ESimCIPHEAD;
   }
   else if(strstr(Receivemsg.Data, "AT+CIPSTART=") && (strstr(Receivemsg.Data, "CONNECT OK")))
   {
       SIMStatus = ESimGPRSConnect;
   }
   else if(strstr(Receivemsg.Data, "AT+CIPSTART=") && (strstr(Receivemsg.Data, "ERROR")))
   {
       SIMStatus = ESimGPRSTest;
   }
}


void GPRSConnect()
{
	/////////////////////////////////////
#if 0
	SendMsgToSim900A("AT+CIPCLOSE\r\n");
    OSTimeDlyHMSM(0,0,0,100);
	SendMsgToSim900A("AT+CIPSHUT\r\n");
    OSTimeDlyHMSM(0,0,0,100);
	SendMsgToSim900A("AT+CIPHEAD=1\r\n");
    OSTimeDlyHMSM(0,0,0,100);
#endif
    OSTimeDlyHMSM(0,0,0,100);
    SendMsgToSim900A("AT+CIPSTART=\"TCP\",\"115.159.41.116\",\"8801\"\r\n");
    OSTimeDlyHMSM(0,0,5,0);
}
void SIM900Process(void *arg)
{
    (void)arg;
    SIMStatus = ESimNoInit;
    OSTimeDlyHMSM(0,0,5,0);
    while (1)
    {
        
        switch(SIMStatus)
		{
			case ESimNoInit:
                OSTimeDlyHMSM(0,0,3,0);
                CheckSIMStatus();
                printf("Check SIM status\r\n");
				break;
			case ESimReady:
                printf("SIM900A ready\r\n");
                OSTimeDlyHMSM(0,0,1,0);
                SIM900A_CIPCLOSE();
				break;
            case ESimCIPCLOSE:
                printf("SIM900A ESimCIPCLOSE\r\n");
                OSTimeDlyHMSM(0,0,1,0);
                SIM900A_CIPSHUT();
                break;
            case ESimCIPSHUT:
                printf("SIM900A ESimCIPSHUT\r\n");
                OSTimeDlyHMSM(0,0,1,0);
                SIM900A_CIPHEAD();
                break;
            case ESimCIPHEAD:
                printf("SIM900A ESimCIPHEAD\r\n");
                OSTimeDlyHMSM(0,0,1,0);
                GPRSConnect();
                break;
            case ESimGPRSTest:
                printf("Test SIM900A\r\n");
                break;
			case ESimGPRSDisConnect:
                printf("SIM900A ESimGPRSDisConnect\r\n");
				break;
			case ESimGPRSConnect:
                printf("SIM900A ESimGPRSConnect\r\n");
				break;
			case ESimGPRSSendReceiveMsg:
                printf("SIM900A ESimGPRSSendReceiveMsg\r\n");
				//send and receive message here
				break;
			case ESimGPRSSendFault:

			default:
				break;
		}
              
		Receivemsg = *(commonMessage*)OSMboxPend(CommonMsg, OS_TICKS_PER_SEC*5, &err);

		if(RECEIVE_SIM900_MESSAGE == Receivemsg.messageId)
		{
            if(ECommonMessageStatus_Ok == Receivemsg.status)
            {
                printf("#######################################\r\n");
                printf("Nomal: Recive message data %s \r\n", Receivemsg.Data);
                HandleReceivedMsg();
            }
			else if(ECommonMessageStatus_InvalidLength == Receivemsg.status)
            {
                printf("ERR: receive invalid length date,the string shoud < 100\r\n");
            }
            else if(ECommonMessageStatus_Error == Receivemsg.status)
            {
                printf("ERR: recive ECommonMessageStatus_Error \r\n");
                printf("ERR: Recive message data %s \r\n", Receivemsg.Data);
            }
		}

 #if 0
         // Usart3
        while(FlagUsart2ReceiveDate)
        {
            printf("Receive date from Usart3\n");
            FlagUsart3ReceiveDate = FALSE;
            FlagUsart3ReceiveDateDone = TRUE;
            OSTimeDlyHMSM(0,0,0,3);
        }
     
        if(FlagUsart3ReceiveDateDone)
        {
            FlagUsart3ReceiveDateDone = FALSE;
            Usart3DateLength = 0;
            printf("Receive date from Usart3\n");
            printf(Usart3Buff);
            printf("\n");
        }
#endif			
		OSTimeDlyHMSM(0, 0,0,50);				 	   
    }
}

/******************* (C) COPYRIGHT 2015 *****END OF FILE************/


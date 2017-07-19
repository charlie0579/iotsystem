/******************** (C) COPYRIGHT 2015 ***************************
 * File  ：usart1.c
 * Description : Init uart. Can use macro  defind printf to USARTx        
 * Author :  William
**********************************************************************************/
#include "SIM900.h"
#include <stdarg.h>
#include "misc.h"
#include "plantformMessage.h"
#include "usart.h"
#include "../APP/MQTT/mqtt.h"

#define MUTEX_PRIO              1       // MUTEX_PRIO < OS_LOWEST_PRIO  
#define EchoTestSize            10

commonMessage Receivemsg;
INT8U   err; 
OS_STK        SIM900ProcessStk[SIM900_STK_SIZE];
OS_EVENT    *Mutex;  
ESIM900Status SIMStatus;
ELEDStatus LEDStatus;
ELEDStatus LEDHisStatus;

int SendFaultCount = 0;
char Sim900AReceiveBuff[Sim900AReceiveBuffSize];
u8 tempBuffP = 0;
Buff SIM900ReveBuff;

char RevMsgBuff[100];
char MsgLength[3];

char client_id[] = "terminal1234"; 
char username[] = "nokia";
char password[] = "nokia123";
char mqtt_message[100];
int MQTTmessageSize;

char* ATConnectToServer = "AT+CIPSTART=\"TCP\",\"115.159.41.116\",\"8801\"\r\n";
//char* ATConnectToServer = "AT+CIPSTART=\"TCP\",\"115.159.41.116\",\"1883\"\r\n";
char* ATCIPCLOSE = "AT+CIPCLOSE\r\n";
char* ATCIPSHUT = "AT+CIPSHUT\r\n";
char* ATCIPHEAD = "AT+CIPHEAD=1\r\n";
char* ATCPIN = "AT+CPIN?\r\n";

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

void SendMsgToSim900A(char *p)
{
	while(*p != '\0')
	{
		SendCharSim900A(*p++);
	}
}

void SendMsgToSim900AWithCount(char *buf, int count)
{
    int TempCount = count;
	while(TempCount != 0)
	{
		SendCharSim900A(*buf++);
        TempCount--;
	}
}

void ResetErrorCount(void)
{
	SendFaultCount = 0;
}

char *bytesbytes(const char*s1, int s1size, const char*s2, int s2size)
{
    int n;
    if(s2size > 0 && s1size >= s2size)
    {
        while(s1size > 0)
        {
            for(n=0;*(s1+n)==*(s2 + n);n++ )
            {
                if(n >= s2size-1)
                    return(char*)s1;
            }
            s1++;
            s1size--;
        }
        return NULL;
    }
    else
    {
        return NULL;
    }
}

char* Sim900AReceiveData(int* RevBuffSize)
{
    char* p = NULL;
    int tempi = 10;
    char* char1 = 0;
    char* char2 = 0;
    int pos1 = 0;
    int pos2 = 0;
    char* ret = NULL;

//    OSMutexPend(Mutex, 0, &err);
/*********************************/
    while(tempi--)
    {
        p = CheckTargetBuffReveived(SIM900ReveBuff, "+IPD");
        if(p == NULL)
        {
            OSTimeDlyHMSM(0,0,0,100); 
        }
        else
        {
            char1 = CheckTargetBuffReveived(SIM900ReveBuff, ",");
            pos1 = char1 - Sim900AReceiveBuff;
            char2 = CheckTargetBuffReveived(SIM900ReveBuff, ":");
            pos2 = char2 - Sim900AReceiveBuff;
            
        }
    }

    if(NULL != p)
    {
        strncpy(MsgLength, Sim900AReceiveBuff+pos1+1, (pos2-pos1-1));
        *RevBuffSize = atoi(MsgLength);
        memcpy(RevMsgBuff, Sim900AReceiveBuff+pos2+1, *RevBuffSize);
        ret = RevMsgBuff;
        ClearSim900AReceiveBuff();
    }
    else
    {
        ret = NULL;
    }
//    OSMutexPost(Mutex);
    return ret;

}


void ClearSim900AReceiveBuff(void)
{
	u16 tempi;
	tempBuffP = 0;
    OSMutexPend(Mutex, 0, &err);
	for(tempi = 0; tempi < Sim900AReceiveBuffSize; tempi++ )
	{
		Sim900AReceiveBuff[tempi] = '\0';
	}
    OSMutexPost(Mutex);
}


void LongDelayms(unsigned int uiDly)
{
    OSTimeDlyHMSM(0,0,0,uiDly);
}

char* CheckTargetBuffReveived(Buff srcBuff, char* targetBuff1)
{
	char *p1 = NULL;
	int i;
    Buff tempBuff;
    tempBuff.buffP = targetBuff1;
    tempBuff.buffSize = strlen(targetBuff1);
    OSMutexPend(Mutex, 0, &err);
	for(i = 0; i< 10; i++)
	{
        if(NULL != targetBuff1)
        {
            p1 = bytesbytes((const char*)srcBuff.buffP, srcBuff.buffSize, (const char*)tempBuff.buffP,tempBuff.buffSize);
        }
        
		if((p1 == NULL) && (targetBuff1 != NULL)) 
		{
			OSTimeDlyHMSM(0,0,0,100);
		}
		else
		{
			break;
		}
	}

    OSMutexPost(Mutex);
    return p1;
}


u8 ConfirmIfEndReceive(Buff srcBuff, char* targetBuff1,char* targetBuff2, char* targetBuff3)
{
	char *p1 = NULL;
    char *p2 = NULL;
    char *p3 = NULL;
    int ret = 0;
	int i;
    OSMutexPend(Mutex, 0, &err);
	for(i = 0; i< 20; i++)
	{
        if(NULL != targetBuff1)
        {
            p1 = bytesbytes((const char*)srcBuff.buffP, srcBuff.buffSize, (const char*)targetBuff1,strlen(targetBuff1));
        }
        if(NULL != targetBuff2)
        {
            p2 = bytesbytes((const char*)srcBuff.buffP, srcBuff.buffSize, (const char*)targetBuff2,strlen(targetBuff2));
        }
        if(NULL != targetBuff3)
        {
            p3 = bytesbytes((const char*)srcBuff.buffP, srcBuff.buffSize, (const char*)targetBuff3,strlen(targetBuff3));
        }
		if(((p1 == NULL) && (targetBuff1 != NULL)) ||((p2 == NULL) && (targetBuff2 != NULL)) || ((p3 == NULL) && (targetBuff3 != NULL)))
		{
			OSTimeDlyHMSM(0,0,0,100);
		}
		else
		{
			break;
		}
	}
	if((NULL == p1) && ((NULL == p2) && (NULL == p3)))
	{
		ret = MATCHTARGET0;
	}
	else if((NULL != p1) && ((NULL == p2) && (NULL == p3)))
	{
		ret = MATCHTARGET1;
	}
    else if((NULL != p1) && ((NULL != p2) && (NULL == p3)))
	{
		ret = MATCHTARGET12;
	}
    else if((NULL != p1) && ((NULL != p2) && (NULL != p3)))
	{
		ret = MATCHTARGET123;
	}
    else if((NULL != p1) && ((NULL == p2) && (NULL != p3)))
	{
		ret = MATCHTARGET13;
	}
    else if((NULL == p1) && ((NULL != p2) && (NULL != p3)))
	{
		ret = MATCHTARGET23;
	}
    OSMutexPost(Mutex);
    return ret;
}


void CheckSIMStatus()
{
	ResetErrorCount();
    printf("**************AT+CPIN?*****************\r\n");

    SendMsgToSim900A(ATCIPCLOSE);
	OSTimeDlyHMSM(0,0,0,50);
	SendMsgToSim900A(ATCIPSHUT);
	OSTimeDlyHMSM(0,0,0,50);
	SendMsgToSim900A(ATCIPHEAD);
    OSTimeDlyHMSM(0,0,0,50);
    SendMsgToSim900A(ATCPIN);
    OSTimeDlyHMSM(0,0,0,50);
	if(ConfirmIfEndReceive(SIM900ReveBuff, "AT+CPIN", "OK", NULL) == MATCHTARGET12)
	{
		printf("\r\nSIM card ready \r\n");
		SIMStatus = ESim900Ready;
	}
	else
	{
		printf("\r\n %s \r\n", Sim900AReceiveBuff);
		printf("\r\n SIM card ready not ready!");
		SIMStatus = ESim900NoInit;
	}
		
	printf("\r\n*******************************\r\n");
//	ClearSim900AReceiveBuff();
}

void GPRSConnect(u32 DelayTimeMs)
{
	/////////////////////////////////////
    int RetMatch1 = 0;
    int RetMatch2 = 0;
    int GPRSConnectCount = 0;
	OSTimeDlyHMSM(0,0,0,DelayTimeMs);

	SendMsgToSim900A(ATCIPCLOSE);
	OSTimeDlyHMSM(0,0,0,DelayTimeMs);
	SendMsgToSim900A(ATCIPSHUT);
	OSTimeDlyHMSM(0,0,0,DelayTimeMs);
	SendMsgToSim900A(ATCIPHEAD);
	OSTimeDlyHMSM(0,0,0,DelayTimeMs);
 
	if(ConfirmIfEndReceive(SIM900ReveBuff, "AT+CIPHEAD", "OK", NULL) == MATCHTARGET12)
	{
//        ClearSim900AReceiveBuff();
		printf("AT+CIPHEAD ready \r\n");
        while(GPRSConnectCount < 5)
        {
            GPRSConnectCount++;
            SendMsgToSim900A(ATConnectToServer);
            OSTimeDlyHMSM(0,0,1,0);
            
            RetMatch1 = ConfirmIfEndReceive(SIM900ReveBuff, "AT+CIPSTART","CONNECT OK",NULL);
            RetMatch2 = ConfirmIfEndReceive(SIM900ReveBuff, "AT+CIPSTART","ALREADY CONNECTED",NULL);
            if(( MATCHTARGET12 == RetMatch1) || ( MATCHTARGET12 == RetMatch2))
            {
                printf("GPRS CONNECT OK \r\n");
                SIMStatus = Esim900GPRSSendReceiveMsg;
                break;
            }
            else
            {
                printf("GPRS NOT CONNECTTED!\r\n");
                SIMStatus = ESim900GPRSDisConnect;
            }
        }
	}
	else
	{
		printf("AT+CIPHEAD not ready!\r\n");
		SIMStatus = ESim900NoInit;
	}
//	ConfirmLEDCommand();
//	ClearSim900AReceiveBuff();
}

void GPRSSendMsg(char* SendString, int StringSize)
{

    int DelayTimeMs = 50;
    int tempi = 0;
    (void)tempi;

	OSTimeDlyHMSM(0,0,0,DelayTimeMs);;
	SendMsgToSim900A("AT+CIPSEND\r\n");
	OSTimeDlyHMSM(0,0,0,DelayTimeMs);
	if(ConfirmIfEndReceive(SIM900ReveBuff, "AT+CIPSEND",">", NULL) == MATCHTARGET12)
	{
        SendMsgToSim900AWithCount(SendString, StringSize);
		SendCharSim900A(0x1A);
		OSTimeDlyHMSM(0,0,0,500);
        printf("\r\nSend message to Server\r\n");
        for(tempi = 0; tempi < StringSize; tempi++)
        {
            printf("%d ",*SendString++);
        }
        printf("\r\n");
		if(ConfirmIfEndReceive(SIM900ReveBuff, "SEND OK", NULL, NULL ) == MATCHTARGET1)
		{
			SendFaultCount = 0;
			SIMStatus = Esim900GPRSSendReceiveMsg;
		}
		else
		{
			printf("\r\n Send Fault\r\n");
			SendFaultCount++;
		}
	}
	else
	{
		SIMStatus = Esim900GPRSSendReceiveMsg;
		SendFaultCount++;
	}
	if(2 == SendFaultCount)
	{
		SIMStatus = ESim900NoInit;
        SendFaultCount = 0;
	}
//	ConfirmLEDCommand();
//	ClearSim900AReceiveBuff();
}

/////////
void ConfirmLEDCommand(void)
{
		u32 DelayTimeMs = 50;
		if(ConfirmIfEndReceive(SIM900ReveBuff, "TURNON", NULL,NULL) == MATCHTARGET1)
		{
			printf("\r\n Open LED\r\n");
			LEDStatus = ELedOpen;
			//////////////////////////
			LongDelayms(DelayTimeMs);
			SendMsgToSim900A("\r\n");
			LongDelayms(DelayTimeMs);
			SendMsgToSim900A("AT+CIPSEND\r\n");
			LongDelayms(DelayTimeMs);
			if(ConfirmIfEndReceive(SIM900ReveBuff,"AT+CIPSEND", ">", NULL) == MATCHTARGET12)
			{
		//		SendMsgToSim900A("TERMINAL\r\n");
		//		LongDelayms(1000);
		//		SendCharSim900A(0x1a);

				SendMsgToSim900A("SUCCESS\r\n");

				LongDelayms(DelayTimeMs);
				SendCharSim900A(0x1a);
				LongDelayms(DelayTimeMs*5);
			}
			///////////////////////////////			
		}
		else if(ConfirmIfEndReceive(SIM900ReveBuff,"AT+CIPSEND", "SHUTDOWN", NULL) == MATCHTARGET12) 
		{
			printf("\r\n Close LED\r\n");
			LEDStatus = ELedClose;
			
			//////////////////////////
			LongDelayms(DelayTimeMs);
			SendMsgToSim900A("\r\n");
			LongDelayms(DelayTimeMs);
			SendMsgToSim900A("AT+CIPSEND\r\n");
			LongDelayms(DelayTimeMs);
			if(ConfirmIfEndReceive(SIM900ReveBuff, ">", NULL, NULL) == MATCHTARGET1)
			{
		//		SendMsgToSim900A("TERMINAL\r\n");
		//		LongDelayms(1000);
		//		SendCharSim900A(0x1a);

				SendMsgToSim900A("SUCCESS\r\n");

				LongDelayms(DelayTimeMs);
				SendCharSim900A(0x1a);
				LongDelayms(DelayTimeMs*5);
			}
			///////////////////////////////
		}
		else if(ConfirmIfEndReceive(SIM900ReveBuff, "QUERY", NULL, NULL ) == MATCHTARGET1) 
		{
			printf("\r\n Query LED\r\n");
			//////////////////////////
			LongDelayms(DelayTimeMs);
			SendMsgToSim900A("\r\n");
			LongDelayms(DelayTimeMs);
			SendMsgToSim900A("AT+CIPSEND\r\n");
			LongDelayms(DelayTimeMs);
			if(ConfirmIfEndReceive(SIM900ReveBuff,"AT+CIPSEND", ">", NULL) == MATCHTARGET12)
			{
		//		SendMsgToSim900A("TERMINAL\r\n");
		//		LongDelayms(1000);
		//		SendCharSim900A(0x1a);
				if(LEDStatus == ELedOpen)
				{
					SendMsgToSim900A("OPEN\r\n");
				}
				else
				{
					SendMsgToSim900A("CLOSE\r\n");
				}
				LongDelayms(DelayTimeMs);
				SendCharSim900A(0x1a);
				LongDelayms(DelayTimeMs*5);
			}
			///////////////////////////////
		}
}

//////////////
void CheckLEDCommand(void)
{
	LongDelayms(50);
	ConfirmLEDCommand();
//	ClearSim900AReceiveBuff();
}

void CloseLed(void)
{
	GPIOD ->ODR &= ~GPIO_Pin_2;	   //点亮(LED共阳极)
	OSTimeDlyHMSM(0,0,2,0);
}
void OpenLed(void)
{
	GPIOD ->ODR = GPIO_Pin_2;	   //熄灭(LED共阳极)
	OSTimeDlyHMSM(0,0,2,0);

}

/////////////////////////////////////////////////////
///////////////////////////////////////////////////
u32 EchoCount = 0;
char EchoTest[EchoTestSize];
void SIM900Process(void *arg)
{
    int RevBuffSize = 0;
    char* RevMsgP = NULL;
    int tempi = 0;
    char* tempC = NULL;

    (void)arg;
    (void)tempC;
    Mutex = OSMutexCreate(MUTEX_PRIO,&err);
    if(err == OS_ERR_NONE)  
    {  
        printf("OSMutexCreate Sucess.\r\n");       
    }

    SIM900ReveBuff.buffP = Sim900AReceiveBuff;
    SIM900ReveBuff.buffSize = Sim900AReceiveBuffSize;
    
    SIMStatus = ESim900NoInit;
    MQTTmessageSize = mqtt_connect_message((unsigned char*)mqtt_message,client_id,username,password);
    printf("\r\nMQTTmessageSize is %d\r\n",MQTTmessageSize);

#if 0   
    tempBuff[0] = 0;
    tempBuff[1] = '4';
    tempBuff[2] = 'M';
    tempBuff[3] = 'Q';
    tempBuff[4] = 'T';
    tempBuff[5] = 'T';
    tempBuff[6] = '0'; // protocol level 4
#endif    
    for(tempi = 0; tempi < MQTTmessageSize; tempi++)
        printf("%d ", mqtt_message[tempi]);
    OSTimeDlyHMSM(0,0,10,0);

    while (1)
    {
		switch(SIMStatus)
		{
			case ESim900NoInit:
				CheckSIMStatus();
                ClearSim900AReceiveBuff();
				break;
			case ESim900Ready:
				GPRSConnect(50);
                ClearSim900AReceiveBuff();
				break;
			case ESim900GPRSDisConnect:
				GPRSConnect(50);
                ClearSim900AReceiveBuff();
				break;
			case ESim900GPRSConnect:
//				GPRSSendMsg(EchoTest, EchoTestSize);
//				CheckLEDCommand();
				break;
			case Esim900GPRSSendReceiveMsg:
				//send and receive message here
				EchoCount++;
//				if((EchoCount%10) == 0)
				{
//                    GPRSSendMsg(&tempBuff[0],MQTTmessageSize-40);
                    
                    printf("\r\n####Check receive message from Server\r\n");
                    RevMsgP = Sim900AReceiveData(&RevBuffSize);
                    tempC = RevMsgP;
                    if(tempC != NULL)
                    {
                        printf("\r\n!!!!!!!!!!!receive message from Server\r\n");
                        for(tempi = 0; tempi < RevBuffSize; tempi++)
                            printf("%d ", *tempC++);
                    }
                    else
                    {
                        printf("!&&&&&&&No IPD in Buff\r\n");
                    }

//                    GPRSSendMsg(EchoTest,EchoTestSize*5);
                    GPRSSendMsg(mqtt_message,MQTTmessageSize);
				}
//				CheckLEDCommand();
//                ClearSim900AReceiveBuff();
				break;
			case Esim900GPRSSendFault:
//				GPRSSendMsg(EchoTest,EchoTestSize);
//				CheckLEDCommand();
                ClearSim900AReceiveBuff();
			default:
				break;
		}
#if 0
		switch(LEDStatus)
		{
			case ELedOpen:
				if(LEDHisStatus != ELedOpen)
				{
					LEDHisStatus = ELedOpen;
					printf("Send Open \r\n");
					OpenLed();
				}
				break;
			case ELedClose:
				if(LEDHisStatus != ELedClose)
				{
					LEDHisStatus = ELedClose;
					printf("Send Close \r\n");
					CloseLed();
				}
				break;
			default:
				break;
		}
#endif
        printf("SIMStatus is %d\r\n",SIMStatus);
        if(SIMStatus == Esim900GPRSSendReceiveMsg)
        {
//            MQTT_main();
        }            
		OSTimeDlyHMSM(0, 0,2,0);				 	   
    }
}

/******************* (C) COPYRIGHT 2015 *****END OF FILE************/


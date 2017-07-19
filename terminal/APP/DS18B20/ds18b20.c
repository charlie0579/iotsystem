/******************** (C) COPYRIGHT 2017 ***************************
 * File  ��ousart1.c
 * Description : Init uart. Can use macro  defind printf to USARTx        
 * Author :  William
**********************************************************************************/
#include <stdarg.h>
#include "misc.h"
#include "plantformMessage.h"
#include "usart.h"
#include "ds18b20.h"
#include "stm32f10x.h"
#include "led.h"
# include <stdio.h>
# include <stdlib.h>

#include "usart.h"
#include <stdarg.h>
#include "misc.h"
#include "plantformMessage.h"
OS_STK DS18B20TaskStk[DS18B20_STK_SIZE];
OS_CPU_SR     cpu_sr;

void Delay_us(u32 usTime)
{
  u8 tmp = 0;
  while(usTime)
  {
      for(tmp = 0; tmp <= 6; tmp++);
      usTime--;
  }      
}
/*
 * ��������DS18B20_GPIO_Config
 * ����  ������DS18B20�õ���I/O��
 * ����  ����
 * ���  ����
 */
static void DS18B20_GPIO_Config(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*����DS18B20_PORT������ʱ��*/
	RCC_APB2PeriphClockCmd(DS18B20_CLK, ENABLE); 

	/*ѡ��Ҫ���Ƶ�DS18B20_PORT����*/															   
  	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;	

	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯������ʼ��DS18B20_PORT*/
  	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
	
	GPIO_SetBits(DS18B20_PORT, DS18B20_PIN);	 
}

/*
 * ��������DS18B20_Mode_IPU
 * ����  ��ʹDS18B20-DATA���ű�Ϊ����ģʽ
 * ����  ����
 * ���  ����
 */
static void DS18B20_Mode_IPU(void)
{
 	  GPIO_InitTypeDef GPIO_InitStructure;

	  	/*ѡ��Ҫ���Ƶ�DS18B20_PORT����*/	
	  GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;

	   /*��������ģʽΪ��������ģʽ*/ 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	

	  /*���ÿ⺯������ʼ��DS18B20_PORT*/
	  GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}


/*
 * ��������DS18B20_Mode_Out_PP
 * ����  ��ʹDS18B20-DATA���ű�Ϊ���ģʽ
 * ����  ����
 * ���  ����
 */
static void DS18B20_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	 	/*ѡ��Ҫ���Ƶ�DS18B20_PORT����*/															   
  	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;	

	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/*���ÿ⺯������ʼ��DS18B20_PORT*/
  	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

/*
 *�������ӻ����͸�λ����
 */
static void DS18B20_Rst(void)
{
	/* ��������Ϊ������� */
	DS18B20_Mode_Out_PP();
	
	DS18B20_DATA_OUT(LOW);
	/* �������ٲ���480us�ĵ͵�ƽ��λ�ź� */
	Delay_us(750);
	
	/* �����ڲ�����λ�źź��轫�������� */
	DS18B20_DATA_OUT(HIGH);
	
	/*�ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������*/
	Delay_us(15);
}

/*
 * ���ӻ����������صĴ�������
 * 0���ɹ�
 * 1��ʧ��
 */
static uint8_t DS18B20_Presence(void)
{
	uint8_t pulse_time = 0;
	
	/* ��������Ϊ�������� */
	DS18B20_Mode_IPU();
	
	/* �ȴ���������ĵ�������������Ϊһ��60~240us�ĵ͵�ƽ�ź� 
	 * �����������û����������ʱ�������ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������
	 */
	while( DS18B20_DATA_IN() && pulse_time<100 )
	{
		pulse_time++;
		Delay_us(1);
	}	
	/* ����100us�󣬴������嶼��û�е���*/
	if( pulse_time >=100 )
		return 1;
	else
		pulse_time = 0;
	
	/* �������嵽�����Ҵ��ڵ�ʱ�䲻�ܳ���240us */
	while( !DS18B20_DATA_IN() && pulse_time<240 )
	{
		pulse_time++;
		Delay_us(1);
	}	
	if( pulse_time >=240 )
		return 1;
	else
		return 0;
}

/*
 * ��DS18B20��ȡһ��bit
 */
static uint8_t DS18B20_Read_Bit(void)
{
	uint8_t dat;
	
	/* ��0�Ͷ�1��ʱ������Ҫ����60us */	
	DS18B20_Mode_Out_PP();
	/* ��ʱ�����ʼ���������������� >1us <15us �ĵ͵�ƽ�ź� */
	DS18B20_DATA_OUT(LOW);
	Delay_us(10);
	
	/* ���ó����룬�ͷ����ߣ����ⲿ�������轫�������� */
	DS18B20_Mode_IPU();
	//Delay_us(2);
	
	if( DS18B20_DATA_IN() == SET )
		dat = 1;
	else
		dat = 0;
	
	/* �����ʱ������ο�ʱ��ͼ */
	Delay_us(45);
	
	return dat;
}

/*
 * ��DS18B20��һ���ֽڣ���λ����
 */
uint8_t DS18B20_Read_Byte(void)
{
	uint8_t i, j, dat = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = DS18B20_Read_Bit();		
		dat = (dat) | (j<<i);
	}
	
	return dat;
}

/*
 * дһ���ֽڵ�DS18B20����λ����
 */
void DS18B20_Write_Byte(uint8_t dat)
{
	uint8_t i, testb;
	DS18B20_Mode_Out_PP();
	
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;		
		/* д0��д1��ʱ������Ҫ����60us */
		if (testb)
		{			
			DS18B20_DATA_OUT(LOW);
			/* 1us < �����ʱ < 15us */
			Delay_us(8);
			
			DS18B20_DATA_OUT(HIGH);
			Delay_us(58);
		}		
		else
		{			
			DS18B20_DATA_OUT(LOW);
			/* 60us < Tx 0 < 120us */
			Delay_us(70);
			
			DS18B20_DATA_OUT(HIGH);			
			/* 1us < Trec(�ָ�ʱ��) < �����*/
			Delay_us(2);
		}
	}
}

void DS18B20_Start(void)
{
	DS18B20_Rst();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);		/* ���� ROM */
	DS18B20_Write_Byte(0X44);		/* ��ʼת�� */
}

uint8_t DS18B20_Init(void)
{
    uint8_t ReturnVal;
    OSSchedLock();
    OSIntEnter();
    OS_ENTER_CRITICAL();
	DS18B20_GPIO_Config();
	DS18B20_Rst();
	ReturnVal = DS18B20_Presence();    
    OS_EXIT_CRITICAL();
    OSIntExit();
    OSSchedUnlock();
	return ReturnVal;
}

/*
 * �洢���¶���16 λ�Ĵ�������չ�Ķ����Ʋ�����ʽ
 * ��������12λ�ֱ���ʱ������5������λ��7������λ��4��С��λ
 *
 *         |---------����----------|-----С�� �ֱ��� 1/(2^4)=0.0625----|
 * ���ֽ�  | 2^3 | 2^2 | 2^1 | 2^0 | 2^(-1) | 2^(-2) | 2^(-3) | 2^(-4) |
 *
 *
 *         |-----����λ��0->��  1->��-------|-----------����-----------|
 * ���ֽ�  |  s  |  s  |  s  |  s  |    s   |   2^6  |   2^5  |   2^4  |
 *
 * 
 * �¶� = ����λ + ���� + С��*0.0625
 */
float DS18B20_Get_Temp(void)
{
	uint8_t tpmsb, tplsb;
	short s_tem;
	float f_tem;
	
	DS18B20_Rst();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);				/* ���� ROM */
	DS18B20_Write_Byte(0X44);				/* ��ʼת�� */
	
	DS18B20_Rst();
    DS18B20_Presence();
    DS18B20_Write_Byte(0XCC);				/* ���� ROM */
    DS18B20_Write_Byte(0XBE);				/* ���¶�ֵ */
	
	tplsb = DS18B20_Read_Byte();		 
	tpmsb = DS18B20_Read_Byte(); 
	
	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
	
	if( s_tem < 0 )		/* ���¶� */
		f_tem = (~s_tem+1) * 0.0625;	
	else
		f_tem = s_tem * 0.0625;
	
	return f_tem; 	
}

float ReadTemperature = 34;
void DS18B20Task(void *arg)
{
    unsigned char str[10];
    (void)arg;

    printf("\r\n William DS18B20 Process \r\n");
	while( DS18B20_Init())	
	{

		printf("\r\n No ds18b20\r\n");
	}	
	printf("\r\n ds18b20 exit\r\n");
    
    while (1)
    {
        OSSchedLock();
        OS_ENTER_CRITICAL();
//      OSIntEnter();

        ReadTemperature = DS18B20_Get_Temp();
//      printf("%.1f, William\r\n",ReadTemperature);
        sprintf((char*)str, "%d",(int)(ReadTemperature-0.5));
      
//      OSIntExit();
		OS_EXIT_CRITICAL();
        OSSchedUnlock();
        printf("William current temperature %s\r\n",str);
		OSTimeDlyHMSM(0,0,10,0);

    }
}

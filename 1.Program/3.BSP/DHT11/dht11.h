/**
  ******************************************************************************
  * @file     ����������	  	dht11.h
  * @author ���������ߣ�		Lu
  * @version������汾��		V1.0
  * @date   ���޸����ڣ�		2023-12-07
  * @brief  �����ܼ�飩		STM32F103VET6�̼���ģ��
  * @parameter  ��������		��
  * @return   ������ֵ��		��
  ******************************************************************************
  * @attention ��ע�����
  * оƬ�ͺţ�STM32F103VET6
  * ʹ�����ţ�GPIOE6
  * �޸����ڣ�
  * ʵ�ֹ��ܣ�
  *	
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __DHT11_H
#define __DHT11_H
/* �û�ͷ�ļ� ------------------------------------------------------------------*/
//#include "stm32f10x.h"   //����stm32ͷ�ļ�
#include "main.h"

/* �궨�� ----------------------------------------------------------------------*/
/************************** DHT11 �������Ͷ���********************************/
typedef struct
{
    uint8_t DHT11_humi_int;     //DHT11������ʪ����������
    uint8_t DHT11_humi_deci;    //DHT11������ʪ��С������
    uint8_t DHT11_temp_int;     //DHT11�������¶���������
    uint8_t DHT11_temp_deci;    //DHT11�������¶�С������
    uint8_t DHT11_check_sum;    //DHT11����������У���
} DHT11_Data_TypeDef;

/************************** DHT11 �������Ŷ���********************************/
#define      DHT11_Dout_SCK_APBxClock_FUN              RCC_APB2PeriphClockCmd   
#define      DHT11_Dout_GPIO_CLK                       RCC_APB2Periph_GPIOE     //DHT11��ӦGPIOʱ��
#define      DHT11_Dout_GPIO_PORT                      GPIOE                    //DHT11��ӦGPIO�˿�
#define      DHT11_Dout_GPIO_PIN                       GPIO_Pin_6               //DHT11��ӦGPIO����

/************************** DHT11 �����궨��********************************/
#define      DHT11_Dout_0	                           GPIO_ResetBits ( DHT11_Dout_GPIO_PORT, DHT11_Dout_GPIO_PIN ) 
#define      DHT11_Dout_1	                           GPIO_SetBits ( DHT11_Dout_GPIO_PORT, DHT11_Dout_GPIO_PIN ) 
#define      DHT11_Dout_IN()	                       GPIO_ReadInputDataBit ( DHT11_Dout_GPIO_PORT, DHT11_Dout_GPIO_PIN ) 


/* �������� --------------------------------------------------------------------*/
void DHT11_Init(void);//DHT11 ��ʼ������
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data);//DHT11���������ݶ�ȡ

#endif  /*  __DHT11_H */

/* END OF FILE ------------------------------------------------------------------*/

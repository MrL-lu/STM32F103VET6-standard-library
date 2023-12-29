/**
  ******************************************************************************
  * @file     ����������	  	iic.h
  * @author ���������ߣ�		Lu
  * @version������汾��		V1.0
  * @date   ���޸����ڣ�		2023-12-29
  * @brief  �����ܼ�飩		STM32F103VET6�̼���ģ��
  * @parameter  ��������		��
  * @return   ������ֵ��		��
  ******************************************************************************
  * @attention ��ע�����
  * оƬ�ͺţ�STM32F103VET6
  * ʹ�����ţ�
  * �޸����ڣ�
  * ʵ�ֹ��ܣ�
  *	��ע    �� ��׼ģʽ����100kbit/s  ����ģʽ����400kbit/s  ����ģʽ����3.4Mbit/s
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __IIC_H
#define __IIC_H
/* �û�ͷ�ļ� ------------------------------------------------------------------*/
//#include "stm32f10x.h"   //����stm32ͷ�ļ�
#include "main.h"
/* �궨�� ----------------------------------------------------------------------*/
#define         DEBUG_IICx                              I2C1                                    //ѡ��IIC1��2
#define         DEBUG_IIC_APBxClock_FUN                 RCC_APB1PeriphClockCmd                  //IICʱ������
#define         DEBUG_IIC_CLK                           RCC_APB1Periph_I2C1                     //IICʱ��
#define         DEBUG_IIC_GPIO_APBxClock_FUN            RCC_APB2PeriphClockCmd
#define         DEBUG_IIC_GPIO_CLK                      RCC_APB2Periph_GPIOB

#define         DEBUG_IIC_SCL_PORT                      GPIOB
#define         DEBUG_IIC_SCL_PIN                       GPIO_Pin_6

#define         DEBUG_IIC_SDA_PORT                      GPIOB
#define         DEBUG_IIC_SDA_PIN                       GPIO_Pin_7

#define         DEBUG_IIC_Speed                         400000                                  //����IIC������
#define         DEBUG_IIC_OwnAddress                    0x01                                    //������ַ

/*�ȴ���ʱʱ��*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

/* �������� --------------------------------------------------------------------*/
void DEBUG_IIC_Init(void);//IIC �����ʼ��
uint8_t DEBUG_IIC_WriteByte(uint8_t WriteAddr,uint8_t pBuffer);//IIC дһ���ֽں���
uint8_t DEBUG_IIC_ReadBuffer(uint8_t *pBuffer,uint8_t SlaveAddress,uint8_t ReadAddr,uint16_t NumByteToRead);//��I2C�豸�����ȡһ������
void DEBUG_IIC_WaitStandbyState(uint8_t SlaveAddress);//�ȴ���������״̬

#endif  /*  __IIC_H */

/* END OF FILE ------------------------------------------------------------------*/

/**
  ******************************************************************************
  * @file     ����������	  	main.h
  * @author ���������ߣ�		Lu
  * @version������汾��		V1.0
  * @date   ���޸����ڣ�		2023-12-28
  * @brief  �����ܼ�飩		STM32F103VET6�̼���ģ��
  * @parameter  ��������		��
  * @return   ������ֵ��		��
  ******************************************************************************
  * @attention ��ע�����
  * оƬ�ͺţ�STM32F103VET6
  * ʹ�����ţ� USART1_TX -> PA9		USART1_RX -> PA10
  * �޸����ڣ�
  * ʵ�ֹ��ܣ�
  *	
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __USART_H
#define __USART_H
/* �û�ͷ�ļ� ------------------------------------------------------------------*/
#include "stm32f10x.h"   //����stm32ͷ�ļ�
#include "main.h"
#include <stdio.h>
/* �궨�� ----------------------------------------------------------------------*/
//��������
#define  DEBUG_USARTX                   USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           115200
// USART GPIO ���ź궨��
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10
#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler

/* �������� --------------------------------------------------------------------*/
void Usart_Config(void);//USART GPIO ����,������������
void Usart_SendByte(USART_TypeDef *pUSARTX,uint8_t data);//USART ����һ���ַ�
void Usart_SendString(USART_TypeDef *pUSARTX,char *data);//USART �����ַ���
void Usart_SendArray( USART_TypeDef *pUSARTX, uint8_t *array, uint16_t num);//USART ����8λ����
void Usart_SendHalfWord( USART_TypeDef * pUSARTX, uint16_t data);//USART ����һ��16λ��
void Usart_SendWord(USART_TypeDef * pUSARTX, uint32_t data);//USART ����һ��32λ��

#endif  /*  __USART_H */

/* END OF FILE ------------------------------------------------------------------*/

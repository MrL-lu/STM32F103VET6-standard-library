/**
  ******************************************************************************
  * @file     （程序名）	  	main.h
  * @author （程序作者）		Lu
  * @version（程序版本）		V1.0
  * @date   （修改日期）		2023-12-28
  * @brief  （功能简介）		STM32F103VET6固件库模板
  * @parameter  （参数）		无
  * @return   （返回值）		无
  ******************************************************************************
  * @attention （注意事项）
  * 芯片型号：STM32F103VET6
  * 使用引脚： USART1_TX -> PA9		USART1_RX -> PA10
  * 修改日期：
  * 实现功能：
  *	
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __USART_H
#define __USART_H
/* 用户头文件 ------------------------------------------------------------------*/
#include "stm32f10x.h"   //引用stm32头文件
#include "main.h"
#include <stdio.h>
/* 宏定义 ----------------------------------------------------------------------*/
//串口配置
#define  DEBUG_USARTX                   USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           115200
// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10
#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler

/* 函数声明 --------------------------------------------------------------------*/
void Usart_Config(void);//USART GPIO 配置,工作参数配置
void Usart_SendByte(USART_TypeDef *pUSARTX,uint8_t data);//USART 发送一个字符
void Usart_SendString(USART_TypeDef *pUSARTX,char *data);//USART 发送字符串
void Usart_SendArray( USART_TypeDef *pUSARTX, uint8_t *array, uint16_t num);//USART 发送8位数组
void Usart_SendHalfWord( USART_TypeDef * pUSARTX, uint16_t data);//USART 发送一个16位数
void Usart_SendWord(USART_TypeDef * pUSARTX, uint32_t data);//USART 发送一个32位数

#endif  /*  __USART_H */

/* END OF FILE ------------------------------------------------------------------*/

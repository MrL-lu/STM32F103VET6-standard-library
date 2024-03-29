/**
  ******************************************************************************
  * @file     （程序名）	  	rcc_clkconfig.h
  * @author （程序作者）		Lu
  * @version（程序版本）		V1.0
  * @date   （修改日期）		2023-11-23
  * @brief  （功能简介）		STM32F103VET6固件库模板
  * @parameter  （参数）		无
  * @return   （返回值）		无
  ******************************************************************************
  * @attention （注意事项）
  * 芯片型号：STM32F103VET6
  * 使用引脚：
  * 修改日期：
  * 实现功能：
  *	
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __RCC_CLKCONFIG_H
#define __RCC_CLKCONFIG_H
/* 用户头文件 ------------------------------------------------------------------*/
//#include "stm32f10x.h"   //引用stm32头文件
#include "main.h"   //引用STM32头文件
#include "stm32f10x_rcc.h"
/*
 * 使用HSE时，设置系统时钟的步骤
 * 1、开启HSE ，并等待 HSE 稳定
 * 2、设置 AHB、APB2、APB1的预分频因子
 * 3、设置PLL的时钟来源，和PLL的倍频因子，设置各种频率主要就是在这里设置
 * 4、开启PLL，并等待PLL稳定
 * 5、把PLLCK切换为系统时钟SYSCLK
 * 6、读取时钟切换状态位，确保PLLCLK被选为系统时钟
 */

/* 设置 系统时钟:SYSCLK, AHB总线时钟:HCLK, APB2总线时钟:PCLK2, APB1总线时钟:PCLK1
 * PCLK2 = HCLK = SYSCLK
 * PCLK1 = HCLK/2,最高只能是36M
 * 参数说明：pllmul是PLL的倍频因子，在调用的时候可以是：RCC_PLLMul_x , x:[2,3,...16]
 * 举例：User_SetSysClock(RCC_PLLMul_9);  则设置系统时钟为：8MHZ * 9 = 72MHZ
 *       User_SetSysClock(RCC_PLLMul_16); 则设置系统时钟为：8MHZ * 16 = 128MHZ，超频慎用
 *
 * HSE作为时钟来源，经过PLL倍频作为系统时钟，这是通常的做法
 */
 
/* 宏定义 ----------------------------------------------------------------------*/


/* 函数声明 --------------------------------------------------------------------*/

void HSE_SetSysClock(uint32_t pllmul);	// 使用HSE时，SYSCLK = 8M * RCC_PLLMul_x, x:[2,3,...16],最高是128M
void HSI_SetSysClock(uint32_t pllmul);	// 使用HSI时，SYSCLK = 4M * RCC_PLLMul_x, x:[2,3,...16],最高是64MH

#endif  /*  __RCC_CLKCONFIG_H */

/* END OF FILE ------------------------------------------------------------------*/

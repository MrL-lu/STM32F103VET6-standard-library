/**
  ******************************************************************************
  * @file     （程序名）	  	beep.h
  * @author （程序作者）		Lu
  * @version（程序版本）		V1.0
  * @date   （修改日期）		2024-1-5
  * @brief  （功能简介）		STM32F103VET6固件库模板
  * @parameter  （参数）		无
  * @return   （返回值）		无
  ******************************************************************************
  * @attention （注意事项）
  * 芯片型号：STM32F103VET6
  * 使用引脚： PA8
  * 修改日期：
  * 实现功能：
  *	
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __BEEP_H
#define __BEEP_H
/* 用户头文件 ------------------------------------------------------------------*/
//#include "stm32f10x.h"   //引用stm32头文件
#include "main.h"
/* 宏定义 ----------------------------------------------------------------------*/
#define         BEEP_GPIO_PORT    	    GPIOA			            /* GPIO端口 */
#define         BEEP_GPIO_CLK 	        RCC_APB2Periph_GPIOA		/* GPIO端口时钟 */
#define         BEEP_GPIO_PIN		    GPIO_Pin_8			        /* 连接到蜂鸣器的GPIO */

#define         BEEP_ON                 GPIO_SetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
#define         BEEP_OFF                GPIO_ResetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
/* 函数声明 --------------------------------------------------------------------*/
void BEEP_GPIO_Config(void);//初始化控制蜂鸣器的IO
void BEEP_ONTIME(uint32_t duration_ms);//控制蜂鸣器响一段时间
#endif  /*  __BEEP_H */

/* END OF FILE ------------------------------------------------------------------*/


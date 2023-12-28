/**
  ******************************************************************************
  * @file     （程序名）	  	dht11.h
  * @author （程序作者）		Lu
  * @version（程序版本）		V1.0
  * @date   （修改日期）		2023-12-07
  * @brief  （功能简介）		STM32F103VET6固件库模板
  * @parameter  （参数）		无
  * @return   （返回值）		无
  ******************************************************************************
  * @attention （注意事项）
  * 芯片型号：STM32F103VET6
  * 使用引脚：GPIOE6
  * 修改日期：
  * 实现功能：
  *	
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __DHT11_H
#define __DHT11_H
/* 用户头文件 ------------------------------------------------------------------*/
//#include "stm32f10x.h"   //引用stm32头文件
#include "main.h"

/* 宏定义 ----------------------------------------------------------------------*/
/************************** DHT11 数据类型定义********************************/
typedef struct
{
    uint8_t DHT11_humi_int;     //DHT11传感器湿度整数数据
    uint8_t DHT11_humi_deci;    //DHT11传感器湿度小数数据
    uint8_t DHT11_temp_int;     //DHT11传感器温度整数数据
    uint8_t DHT11_temp_deci;    //DHT11传感器温度小数数据
    uint8_t DHT11_check_sum;    //DHT11传感器数据校验和
} DHT11_Data_TypeDef;

/************************** DHT11 连接引脚定义********************************/
#define      DHT11_Dout_SCK_APBxClock_FUN              RCC_APB2PeriphClockCmd   
#define      DHT11_Dout_GPIO_CLK                       RCC_APB2Periph_GPIOE     //DHT11对应GPIO时钟
#define      DHT11_Dout_GPIO_PORT                      GPIOE                    //DHT11对应GPIO端口
#define      DHT11_Dout_GPIO_PIN                       GPIO_Pin_6               //DHT11对应GPIO引脚

/************************** DHT11 函数宏定义********************************/
#define      DHT11_Dout_0	                           GPIO_ResetBits ( DHT11_Dout_GPIO_PORT, DHT11_Dout_GPIO_PIN ) 
#define      DHT11_Dout_1	                           GPIO_SetBits ( DHT11_Dout_GPIO_PORT, DHT11_Dout_GPIO_PIN ) 
#define      DHT11_Dout_IN()	                       GPIO_ReadInputDataBit ( DHT11_Dout_GPIO_PORT, DHT11_Dout_GPIO_PIN ) 


/* 函数声明 --------------------------------------------------------------------*/
void DHT11_Init(void);//DHT11 初始化函数
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data);//DHT11传感器数据读取

#endif  /*  __DHT11_H */

/* END OF FILE ------------------------------------------------------------------*/

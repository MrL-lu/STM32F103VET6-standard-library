/**
  ******************************************************************************
  * @file     （程序名）	  	iic.h
  * @author （程序作者）		Lu
  * @version（程序版本）		V1.0
  * @date   （修改日期）		2023-12-29
  * @brief  （功能简介）		STM32F103VET6固件库模板
  * @parameter  （参数）		无
  * @return   （返回值）		无
  ******************************************************************************
  * @attention （注意事项）
  * 芯片型号：STM32F103VET6
  * 使用引脚：
  * 修改日期：
  * 实现功能：
  *	备注    ： 标准模式速率100kbit/s  快速模式速率400kbit/s  高速模式速率3.4Mbit/s
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __IIC_H
#define __IIC_H
/* 用户头文件 ------------------------------------------------------------------*/
//#include "stm32f10x.h"   //引用stm32头文件
#include "main.h"
/* 宏定义 ----------------------------------------------------------------------*/
#define         DEBUG_IICx                              I2C1                                    //选择IIC1或2
#define         DEBUG_IIC_APBxClock_FUN                 RCC_APB1PeriphClockCmd                  //IIC时钟总线
#define         DEBUG_IIC_CLK                           RCC_APB1Periph_I2C1                     //IIC时钟
#define         DEBUG_IIC_GPIO_APBxClock_FUN            RCC_APB2PeriphClockCmd
#define         DEBUG_IIC_GPIO_CLK                      RCC_APB2Periph_GPIOB

#define         DEBUG_IIC_SCL_PORT                      GPIOB
#define         DEBUG_IIC_SCL_PIN                       GPIO_Pin_6

#define         DEBUG_IIC_SDA_PORT                      GPIOB
#define         DEBUG_IIC_SDA_PIN                       GPIO_Pin_7

#define         DEBUG_IIC_Speed                         400000                                  //配置IIC的速率
#define         DEBUG_IIC_OwnAddress                    0x01                                    //主机地址

/*等待超时时间*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

/* 函数声明 --------------------------------------------------------------------*/
void DEBUG_IIC_Init(void);//IIC 外设初始化
uint8_t DEBUG_IIC_WriteByte(uint8_t WriteAddr,uint8_t pBuffer);//IIC 写一个字节函数
uint8_t DEBUG_IIC_ReadBuffer(uint8_t *pBuffer,uint8_t SlaveAddress,uint8_t ReadAddr,uint16_t NumByteToRead);//从I2C设备里面读取一块数据
void DEBUG_IIC_WaitStandbyState(uint8_t SlaveAddress);//等待从属待机状态

#endif  /*  __IIC_H */

/* END OF FILE ------------------------------------------------------------------*/

/**
  ******************************************************************************
  * @file     （程序名）	  	main.c
  * @author （程序作者）		Lu
  * @version（程序版本）		V1.0.0.0
  * @date   （修改日期）		2023-12-28
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

/* 用户头文件 ------------------------------------------------------------------*/
#include "main.h"

/* 定义全局变量 ------------------------------------------------------------------*/
/* ADC1转换的电压值通过MDA方式传到sram*/
extern __IO u16 ADC_ConvertedValue;
uint16_t chip_temp;

/* 程序开始 ----------------------------------------------------------------------*/
int main(void)
{
	// 程序来到main函数之前，启动文件：statup_stm32f10x_hd.s已经调用
	// SystemInit()函数把系统时钟初始化成72MHZ
	// SystemInit()在system_stm32f10x.c中定义
	// 如果用户想修改系统时钟，可自行编写程序修改
	
	/* 用户申明变量	--------------------------------------------------------------------*/
    DHT11_Data_TypeDef DHT11_Data;
    uint32_t uniqueID[3];
    
	/* 用户初始化开始 ------------------------------------------------------------------*/
	HSE_SetSysClock(RCC_PLLMul_9);  //配置系统时钟为72MHz
	SysTick_Init();	//初始化系统滴答定时器 SysTick
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Usart_Config();//串口配置函数
	BSP_LED_Config();//LED初始化函数
    DHT11_Init();//DHT11 初始化函数
    Get_ChipID(uniqueID);//获取芯片唯一ID
    Temp_ADC1_Init();
    Usart_SendString(DEBUG_USARTX,"程序初始化完毕\r\n");
    
    WS2812_Config(WS2812_Sendspeed);//WS2812 配置函数
    
	/* 主循环程序开始 ------------------------------------------------------------------*/
	while(1)
	{
		LED_Red_TOGGLE;
		SysTick_Delay_ms(1000);	//延时1s
        chip_temp = (1.43- ADC_ConvertedValue*3.3/4096)*1000 / 4.3+ 25;
        printf("\r\n The IC current tem= %3d ℃\r\n", chip_temp);
        printf("\r\n芯片的唯一ID为: 0x%08X-%08X-%08X\r\n",uniqueID[0],uniqueID[1],uniqueID[2]);
        printf("\r\n芯片flash的容量为: %dK \r\n", *(__IO u16 *)(0X1FFFF7E0));
        if(DHT11_Read_TempAndHumidity(&DHT11_Data) == SUCCESS)
        {
            printf("\r\n读取DHT11成功!\r\n\r\n湿度为%d.%d ％RH ，温度为 %d.%d℃ \r\n",\
			DHT11_Data.DHT11_humi_int,DHT11_Data.DHT11_humi_deci,DHT11_Data.DHT11_temp_int,DHT11_Data.DHT11_temp_deci);
        }
        else
        {
            printf("Read DHT11 ERROR!\r\n");
        }
        WS2812_Display_Color(8,0xf11f23);
	}//end of while 主循环结束
	
}//end of main
/* END OF FILE ------------------------------------------------------------------*/


/* 引用头文件 ------------------------------------------------------------------*/  
#include "./LED/led.h"  //引用LED配置头文件

/* 定义全局变量 ------------------------------------------------------------------*/

/* 程序开始 ----------------------------------------------------------------------*/

/**
 * @brief  ：LED初始化函数
 * @param  ：无
 * @retval ：无
 * @doc    ：无
 */
void BSP_LED_Config(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef BSP_LED_GPIO_InitStructure;
	/*开启LED相关的GPIO外设时钟*/
	RCC_APB2PeriphClockCmd( LED_Red_GPIO_CLK | LED_Green_GPIO_CLK | LED_Blue_GPIO_CLK, ENABLE);
	/*设置引脚模式为通用推挽输出*/
	BSP_LED_GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	/*设置引脚速率为50MHz */   
	BSP_LED_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*选择要控制的GPIO引脚*/
	BSP_LED_GPIO_InitStructure.GPIO_Pin = LED_Red_GPIO_PIN;	
	/*调用库函数，初始化GPIO*/
	GPIO_Init(LED_Red_GPIO_PORT, &BSP_LED_GPIO_InitStructure);
	
	/*设置引脚模式为通用推挽输出*/
	BSP_LED_GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	/*设置引脚速率为50MHz */   
	BSP_LED_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*选择要控制的GPIO引脚*/
	BSP_LED_GPIO_InitStructure.GPIO_Pin = LED_Green_GPIO_PIN;	
	/*调用库函数，初始化GPIO*/
	GPIO_Init(LED_Green_GPIO_PORT, &BSP_LED_GPIO_InitStructure);
	
	/*设置引脚模式为通用推挽输出*/
	BSP_LED_GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	/*设置引脚速率为50MHz */   
	BSP_LED_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*选择要控制的GPIO引脚*/
	BSP_LED_GPIO_InitStructure.GPIO_Pin = LED_Blue_GPIO_PIN;	
	/*调用库函数，初始化GPIO*/
	GPIO_Init(LED_Blue_GPIO_PORT, &BSP_LED_GPIO_InitStructure);
	/* 关闭所有led灯	*/
	GPIO_SetBits(LED_Red_GPIO_PORT, LED_Red_GPIO_PIN);
	GPIO_SetBits(LED_Green_GPIO_PORT, LED_Green_GPIO_PIN);
	GPIO_SetBits(LED_Blue_GPIO_PORT, LED_Blue_GPIO_PIN);
}


/* END OF FILE ------------------------------------------------------------------*/

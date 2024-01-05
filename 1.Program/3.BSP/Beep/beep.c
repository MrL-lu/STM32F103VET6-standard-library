/* 引用头文件 ------------------------------------------------------------------*/  
#include "./Beep/beep.h"  //引用蜂鸣器配置头文件

/* 定义全局变量 ------------------------------------------------------------------*/

/* 程序开始 ----------------------------------------------------------------------*/

/**
  * @brief  初始化控制蜂鸣器的IO
  * @param  无
  * @retval 无
  */
void BEEP_GPIO_Config(void)
{		
    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;
    /*开启控制蜂鸣器的GPIO的端口时钟*/
    RCC_APB2PeriphClockCmd( BEEP_GPIO_CLK, ENABLE); 
    /*选择要控制蜂鸣器的GPIO*/															   
    GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;	
    /*设置GPIO模式为通用推挽输出*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    /*设置GPIO速率为50MHz */   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    /*调用库函数，初始化控制蜂鸣器的GPIO*/
    GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);			 
    /* 关闭蜂鸣器*/
    GPIO_ResetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN);	 
}

/**
  * @brief  控制蜂鸣器响一段时间
  * @param  无
  * @retval 无
  */
void BEEP_ONTIME(uint32_t duration_ms)
{
    GPIO_SetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN);//设置蜂鸣器引脚为高电平，使其开始响
    SysTick_Delay_ms(duration_ms);
    GPIO_ResetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN);//设置蜂鸣器引脚为低电平，使其停止响
}

/* END OF FILE ------------------------------------------------------------------*/

/* 引用头文件 ------------------------------------------------------------------*/  
#include "./SysTick/systick.h"  //引用systick系统时钟配置头文件

/* 定义全局变量 ------------------------------------------------------------------*/
static __IO u32 TimingDelay;//定义变量 TimingDelay

/* 程序开始 ----------------------------------------------------------------------*/

/**
  * @brief  启动系统滴答定时器 SysTick
  * @param  无
  * @retval 无
  */
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
//	if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0库版本
	if (SysTick_Config(SystemCoreClock / 1000000))	// ST3.5.0库版本
	{ 
		/* Capture error */ 
		while (1);
	}
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk; // 失能滴答定时器
//	SysTick->CTRL &= 0xfffe; // 失能滴答定时器
}

/**
  * @brief   us延时程序,1us为一个单位
  * @param  
  *		@arg nTime: Delay_us( 1 ) 则实现的延时为 1 * nTime = nTime us
  * @retval  无
  */
void SysTick_Delay_us(__IO u32 nTime)
{ 
	TimingDelay = nTime;
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;	// 使能滴答定时器
	while(TimingDelay != 0);
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk; // 失能滴答定时器
}

/**
  * @brief  获取节拍程序
  * @param  无
  * @retval 无
  * @attention  在 SysTick 中断函数 SysTick_Handler()调用
  */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}

///**
//  * @brief  SysTick 中断函数.
//  * @param  None
//  * @retval None
//  */
//void SysTick_Handler(void)
//{
//	TimingDelay_Decrement();
//}

/* END OF FILE ------------------------------------------------------------------*/

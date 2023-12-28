/* ����ͷ�ļ� ------------------------------------------------------------------*/  
#include "./SysTick/systick.h"  //����systickϵͳʱ������ͷ�ļ�

/* ����ȫ�ֱ��� ------------------------------------------------------------------*/
static __IO u32 TimingDelay;//������� TimingDelay

/* ����ʼ ----------------------------------------------------------------------*/

/**
  * @brief  ����ϵͳ�δ�ʱ�� SysTick
  * @param  ��
  * @retval ��
  */
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
//	if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0��汾
	if (SysTick_Config(SystemCoreClock / 1000000))	// ST3.5.0��汾
	{ 
		/* Capture error */ 
		while (1);
	}
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk; // ʧ�ܵδ�ʱ��
//	SysTick->CTRL &= 0xfffe; // ʧ�ܵδ�ʱ��
}

/**
  * @brief   us��ʱ����,1usΪһ����λ
  * @param  
  *		@arg nTime: Delay_us( 1 ) ��ʵ�ֵ���ʱΪ 1 * nTime = nTime us
  * @retval  ��
  */
void SysTick_Delay_us(__IO u32 nTime)
{ 
	TimingDelay = nTime;
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;	// ʹ�ܵδ�ʱ��
	while(TimingDelay != 0);
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk; // ʧ�ܵδ�ʱ��
}

/**
  * @brief  ��ȡ���ĳ���
  * @param  ��
  * @retval ��
  * @attention  �� SysTick �жϺ��� SysTick_Handler()����
  */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}

///**
//  * @brief  SysTick �жϺ���.
//  * @param  None
//  * @retval None
//  */
//void SysTick_Handler(void)
//{
//	TimingDelay_Decrement();
//}

/* END OF FILE ------------------------------------------------------------------*/

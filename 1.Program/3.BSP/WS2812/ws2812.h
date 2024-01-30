/**
  ******************************************************************************
  * @file     ����������	  	ws2812.h
  * @author ���������ߣ�		Lu
  * @version������汾��		V1.0
  * @date   ���޸����ڣ�		2024-1-5
  * @brief  �����ܼ�飩		STM32F103VET6�̼���ģ��
  * @parameter  ��������		��
  * @return   ������ֵ��		��
  ******************************************************************************
  * @attention ��ע�����
  * оƬ�ͺ� : WS2812b   ���ݸ�ʽΪ GRB 3*8 =24bit MSB
  * ʹ������ : PB0 TIM3 ͨ��3 DMA1 ͨ��2
  * �޸����� : 
  * ʵ�ֹ��� : 
  *	
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __BSP_WS2812_H
#define __BSP_WS2812_H
/* �û�ͷ�ļ� ------------------------------------------------------------------*/
#include "stm32f10x.h"   //����stm32ͷ�ļ�

#include "main.h"

typedef struct
{
    uint8_t R;      //WS2812��Redֵ
    uint8_t G;      //WS2812��Greenֵ
    uint8_t B;      //WS2812��Blueֵ
    float h;        //WS2812��ɫ��ֵ
    float s;        //WS2812�ı��Ͷ�ֵ
    float v;        //WS2812������ֵ
} WS2812_Data_TypeDef;


/* �궨�� ----------------------------------------------------------------------*/
#define			WS2812_LED_Number			    8							/* WS2812 ��������� */
#define			WS2812_Data_Bit				    24
#define			WS2812_T0H					    30							/* 0����ߵ�ƽʱ��ռ1/3 */
#define			WS2812_T1H					    60							/* 1����ߵ�ƽʱ��ռ2/3 */
#define			WS2812_Sendspeed			    800							/* WS2812���ͽ��������ٶ�800Kbps */

#define			WS2812_GPIO_PORT			    GPIOB						/* GPIO�˿� */
#define			WS2812_GPIO_CLK				    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define			WS2812_GPIO_PIN				    GPIO_Pin_0					/* ���ӵ�SCLʱ���ߵ�GPIO */

#define			WS2812_TIM_CLK				    RCC_APB1Periph_TIM3			/* WS2812 ��Ӧ��TIMʱ�� */
#define			WS2812_TIM					    TIM3						/* WS2812 ��Ӧ�Ķ�ʱ�� */
#define			WS2812_TIM_DMA				    TIM_DMA_CC3					/* WS2812 ��Ӧ�Ķ�ʱ����Ӧ��DMAͨ�� */
#define			WS2812_DMA_Addr				    (WS2812_TIM->CCR3)			/* WS2812 DMA�����Ŀ�ĵ�ַ */
#define			WS2812_DMA_Channel			    DMA1_Channel2				/* WS2812 ��Ӧ��DMAͨ�� */
#define			WS2812_DMA_Flag				    DMA1_FLAG_TC2				/* WS2812 ��Ӧ��DMAͨ��������ɱ�־λ */

#define			WS2812_TIM_OCXInit				TIM_OC3Init				    /* WS2812 TIM�Ƚ������ʼ�� */
#define			WS2812_TIM_OCXPreloadConfig		TIM_OC3PreloadConfig		/* WS2812 TIM�Ƚ����Ԥװ�ؼĴ��� */



/* �������� --------------------------------------------------------------------*/
void WS2812_Config(uint16_t WS2812_Byte);	//WS2812 ���ú���
void WS2812_Reset(void);	//WS2812 RESET�ź�
void WS2812_SetColor(uint32_t RGB_Data);	//WS2812 ��ɫ���ú���
void WS2812_Display(uint8_t LED_Num);	//WS2812 ��ʾ����
void WS2812_Display_Color(uint8_t LED_Num,uint32_t RGB_Data);
void WS2812_Set_LED_Color(uint8_t LED_Num,uint32_t RGB_Data);

void WS2812_BreathingLight(uint16_t LED_Num,uint32_t RGB_Data);
#endif  /*  __BSP_WS2812_H */

/* END OF FILE ------------------------------------------------------------------*/


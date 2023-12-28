/**
  ******************************************************************************
  * @file     ����������	  	LED.h
  * @author ���������ߣ�		Lu
  * @version������汾��		V1.0
  * @date   ���޸����ڣ�		2022-8-4
  * @brief  �����ܼ�飩		STM32F103VET6�̼���ģ��
  * @parameter  ��������		��
  * @return   ������ֵ��		��
  ******************************************************************************
  * @attention ��ע�����
  * оƬ�ͺţ�STM32F103VET6
  * ʹ�����ţ�	LED_R -> PB5	LED_G -> PB0	LED_B -> PB1
  * �޸����ڣ� 
  * ʵ�ֹ��ܣ�
  *	
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __LED_H
#define __LED_H
/* �û�ͷ�ļ� ------------------------------------------------------------------*/
#include "stm32f10x.h"   //����stm32ͷ�ļ�

/* �궨�� ----------------------------------------------------------------------*/
//��ɫLED
#define LED_Red_GPIO_PORT				GPIOB			              /* GPIO�˿� */
#define LED_Red_GPIO_CLK				RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define LED_Red_GPIO_PIN				GPIO_Pin_5			        /* ���ӵ�SCLʱ���ߵ�GPIO */

//��ɫLED
#define LED_Green_GPIO_PORT				GPIOB			              /* GPIO�˿� */
#define LED_Green_GPIO_CLK				RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define LED_Green_GPIO_PIN				GPIO_Pin_0			        /* ���ӵ�SCLʱ���ߵ�GPIO */

//��ɫLED
#define LED_Blue_GPIO_PORT				GPIOB			              /* GPIO�˿� */
#define LED_Blue_GPIO_CLK				RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define LED_Blue_GPIO_PIN				GPIO_Pin_11			        /* ���ӵ�SCLʱ���ߵ�GPIO */

/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BRR=i;}	 //����͵�ƽ
#define digitalToggle(p,i) {p->ODR ^=i;} //�����ת״̬

/* �������IO�ĺ� */
#define LED_Red_TOGGLE		 	digitalToggle(LED_Red_GPIO_PORT,LED_Red_GPIO_PIN)
#define LED_Red_OFF		   		digitalHi(LED_Red_GPIO_PORT,LED_Red_GPIO_PIN)
#define LED_Red_ON			   	digitalLo(LED_Red_GPIO_PORT,LED_Red_GPIO_PIN)

#define LED_Green_TOGGLE		digitalToggle(LED_Green_GPIO_PORT,LED_Green_GPIO_PIN)
#define LED_Green_OFF		   	digitalHi(LED_Green_GPIO_PORT,LED_Green_GPIO_PIN)
#define LED_Green_ON			digitalLo(LED_Green_GPIO_PORT,LED_Green_GPIO_PIN)

#define LED_Blue_TOGGLE		 	digitalToggle(LED_Blue_GPIO_PORT,LED_Blue_GPIO_PIN)
#define LED_Blue_OFF		   	digitalHi(LED_Blue_GPIO_PORT,LED_Blue_GPIO_PIN)
#define LED_Blue_ON			   	digitalLo(LED_Blue_GPIO_PORT,LED_Blue_GPIO_PIN)

/* �������� --------------------------------------------------------------------*/
void BSP_LED_Config(void);	//LED��ʼ������

#endif  /*  __LED_H */

/* END OF FILE ------------------------------------------------------------------*/

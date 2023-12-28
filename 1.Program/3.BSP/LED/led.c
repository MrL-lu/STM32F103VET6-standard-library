/* ����ͷ�ļ� ------------------------------------------------------------------*/  
#include "./LED/led.h"  //����LED����ͷ�ļ�

/* ����ȫ�ֱ��� ------------------------------------------------------------------*/

/* ����ʼ ----------------------------------------------------------------------*/

/**
 * @brief  ��LED��ʼ������
 * @param  ����
 * @retval ����
 * @doc    ����
 */
void BSP_LED_Config(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef BSP_LED_GPIO_InitStructure;
	/*����LED��ص�GPIO����ʱ��*/
	RCC_APB2PeriphClockCmd( LED_Red_GPIO_CLK | LED_Green_GPIO_CLK | LED_Blue_GPIO_CLK, ENABLE);
	/*��������ģʽΪͨ���������*/
	BSP_LED_GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	/*������������Ϊ50MHz */   
	BSP_LED_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*ѡ��Ҫ���Ƶ�GPIO����*/
	BSP_LED_GPIO_InitStructure.GPIO_Pin = LED_Red_GPIO_PIN;	
	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(LED_Red_GPIO_PORT, &BSP_LED_GPIO_InitStructure);
	
	/*��������ģʽΪͨ���������*/
	BSP_LED_GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	/*������������Ϊ50MHz */   
	BSP_LED_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*ѡ��Ҫ���Ƶ�GPIO����*/
	BSP_LED_GPIO_InitStructure.GPIO_Pin = LED_Green_GPIO_PIN;	
	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(LED_Green_GPIO_PORT, &BSP_LED_GPIO_InitStructure);
	
	/*��������ģʽΪͨ���������*/
	BSP_LED_GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	/*������������Ϊ50MHz */   
	BSP_LED_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*ѡ��Ҫ���Ƶ�GPIO����*/
	BSP_LED_GPIO_InitStructure.GPIO_Pin = LED_Blue_GPIO_PIN;	
	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(LED_Blue_GPIO_PORT, &BSP_LED_GPIO_InitStructure);
	/* �ر�����led��	*/
	GPIO_SetBits(LED_Red_GPIO_PORT, LED_Red_GPIO_PIN);
	GPIO_SetBits(LED_Green_GPIO_PORT, LED_Green_GPIO_PIN);
	GPIO_SetBits(LED_Blue_GPIO_PORT, LED_Blue_GPIO_PIN);
}


/* END OF FILE ------------------------------------------------------------------*/

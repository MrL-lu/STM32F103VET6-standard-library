/* ����ͷ�ļ� ------------------------------------------------------------------*/  
#include "./ws2812/ws2812.h"  //����ws2812����ͷ�ļ�

/* ����ȫ�ֱ��� ------------------------------------------------------------------*/
uint16_t	WS2812_LED_Buffer[WS2812_Data_Bit*WS2812_LED_Number+1];

/* ����ʼ ----------------------------------------------------------------------*/

/**
 * @brief  ��WS2812 ���ú���
 * @param  ��WS2812_Byte 
 * @retval ����
 * @doc    ����
 */
void WS2812_Config(uint16_t WS2812_Byte)
{
	/*����ṹ��*/
	GPIO_InitTypeDef			WS2812_GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef		WS2812_TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  			WS2812_TIM_OCInitStructure;
	DMA_InitTypeDef 			WS2812_DMA_InitStructure;
	/*����WS2812��Ӧ��GPIO����ʱ��*/
	RCC_APB2PeriphClockCmd(WS2812_GPIO_CLK,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	/*����ΪGPIO����ģʽΪ�����������*/
	WS2812_GPIO_InitStructure.GPIO_Mode		=	GPIO_Mode_AF_PP;
	/*ѡ��Ҫ���Ƶ�GPIO����*/
	WS2812_GPIO_InitStructure.GPIO_Pin		=	WS2812_GPIO_PIN;
	/*����GPIO��������Ϊ50MHz*/
	WS2812_GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(WS2812_GPIO_PORT,&WS2812_GPIO_InitStructure);
	
	
	WS2812_TIM_TimeBaseStructure.TIM_Prescaler		=	0;	//��ʱ����Ƶ��(0+1)=1,����Ƶ
	WS2812_TIM_TimeBaseStructure.TIM_CounterMode	=	TIM_CounterMode_Up;	//���ϼ���ģʽ
	WS2812_TIM_TimeBaseStructure.TIM_Period			=	(SystemCoreClock/(WS2812_Byte*1000))-1;	//800K
	WS2812_TIM_TimeBaseStructure.TIM_ClockDivision	=	TIM_CKD_DIV1;//ʱ�ӷָ�,����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
	TIM_TimeBaseInit(WS2812_TIM,&WS2812_TIM_TimeBaseStructure);
	
	WS2812_TIM_OCInitStructure.TIM_OCMode			=	TIM_OCMode_PWM1;	//����ΪPWMģʽ1
	WS2812_TIM_OCInitStructure.TIM_OutputState		=	TIM_OutputState_Enable;	//ʹ�ܱȽ����
//	WS2812_TIM_OCInitStructure.TIM_Pulse			= 	0;	//��װ�벶��ȽϼĴ���������ֵ(�˳����üӳ�ֵ)
	WS2812_TIM_OCInitStructure.TIM_OCPolarity		=	TIM_OCPolarity_High;	//�������:TIM����Ƚϼ��Ը�
	
	TIM_OC3Init(WS2812_TIM,&WS2812_TIM_OCInitStructure);	//��ʼ������
	TIM_OC3PreloadConfig(WS2812_TIM,TIM_OCPreload_Enable);	//ʹ��TIM3��CCR3�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(WS2812_TIM,DISABLE);	//ʧ��TIM3 ��ֹ���ִ����źŲ���
	
	TIM_DMACmd(WS2812_TIM,WS2812_TIM_DMA,ENABLE);		//ʹ��TIM2_CH3��DMA����(CC2��Ӧͨ��3)
	
	WS2812_DMA_InitStructure.DMA_PeripheralBaseAddr	=	(uint32_t)&WS2812_DMA_Addr;		//����DMAĿ�ĵ�ַ
	WS2812_DMA_InitStructure.DMA_MemoryBaseAddr		=	(uint32_t)WS2812_LED_Buffer;	//����DMAԴ��ַ
	WS2812_DMA_InitStructure.DMA_DIR				=	DMA_DIR_PeripheralDST;			//���򣺴��ڴ�SendBuff���ڴ�ReceiveBuff 
	WS2812_DMA_InitStructure.DMA_BufferSize			=	(24*WS2812_LED_Number);			//һ�δ����СDMA_BufferSize=SendBuffSize
	WS2812_DMA_InitStructure.DMA_PeripheralInc		=	DMA_PeripheralInc_Disable;		//ReceiveBuff��ַ����
	WS2812_DMA_InitStructure.DMA_MemoryInc			=	DMA_MemoryInc_Enable;			//SendBuff��ַ����
	WS2812_DMA_InitStructure.DMA_PeripheralDataSize	=	DMA_PeripheralDataSize_HalfWord;//ReceiveBuff���ݵ�λ,16bit
	WS2812_DMA_InitStructure.DMA_MemoryDataSize		=	DMA_MemoryDataSize_HalfWord;	//SENDBUFF_SIZE���ݵ�λ,16bit
	WS2812_DMA_InitStructure.DMA_Mode				=	DMA_Mode_Normal;				//DMAģʽ������ģʽ(����һ��)
	WS2812_DMA_InitStructure.DMA_Priority			=	DMA_Priority_Medium;			//���ȼ�����
	WS2812_DMA_InitStructure.DMA_M2M				=	DMA_M2M_Disable;				//�ڴ浽�ڴ�Ĵ���
	
	DMA_Init(WS2812_DMA_Channel,&WS2812_DMA_InitStructure);									//����DMA1��2ͨ��(��ͬ��ʱ����ͨ����һ��)
	DMA_Cmd(WS2812_DMA_Channel,DISABLE);														//��ʧ��DMA����
	
}


/**
 * @brief  ��WS2812 RESET�ź� �͵�ƽ>280us
 * @param  ����
 * @retval ����
 * @doc    ����
 */
void  WS2812_Reset(void)
{
	TIM_Cmd(WS2812_TIM,DISABLE);	//ʧ��TIM��ʱ��
	GPIO_ResetBits(WS2812_GPIO_PORT,WS2812_GPIO_PIN);	//����͵�ƽ
	SysTick_Delay_ms(1);	//��ʱ1ms
}

/**
 * @brief  ��WS2812 ��ɫ���ú���-����ȫ����ɫΪͳһ��ɫ
 * @param  ��RGB_Data����RGB��ɫ ���磺0xffffff���׹⣩
 * @retval ����
 * @doc    ����
 */
void WS2812_SetColor(uint32_t RGB_Data)
{
	uint8_t R,G,B,i,j;	
	R = (uint8_t)(RGB_Data>>16);
	G = (uint8_t)((RGB_Data&0x00ff00)>>8);
	B = (uint8_t)(RGB_Data&0x0000ff);
	RGB_Data = (G<<16) | (R<<8) | B;
	for(j=0;j<WS2812_LED_Number;j++)
	{
		for(i=0;i<24;i++)
		{
			/*��Ϊ���ݷ��͵�˳����GRB����λ�ȷ������ԴӸ�λ��ʼ�жϣ��жϺ�Ƚ�ֵ�ȷ��뻺������*/
			WS2812_LED_Buffer[j*24+i] = ((RGB_Data << i) & 0x800000) ? WS2812_T1H : WS2812_T0H;
		}
	}
	WS2812_LED_Buffer[j*24+i+1] = 0;
}

/**
 * @brief  ��WS2812 ��ʾ����
 * @param  ��LED_Num������ʾLED�ĸ���
 * @retval ����
 * @doc    ����
 */
void  WS2812_Display(uint8_t LED_Num)
{
	/*DMA�������ݺ��������÷��͵�ͨ������������*/
	DMA_SetCurrDataCounter(WS2812_DMA_Channel,(24*LED_Num)+1);
	/*ʹ��DMA����*/
	DMA_Cmd(WS2812_DMA_Channel,ENABLE);
	/*ʹ��TIM����ʼPWM���*/
	TIM_Cmd(WS2812_TIM,ENABLE);
	/*�ȴ�DMA�������*/
	while(DMA_GetFlagStatus(WS2812_DMA_Flag) != SET);
	/*ʧ��DMA����*/
	DMA_Cmd(WS2812_DMA_Channel,DISABLE);
	/*���DMA��־λ*/
	DMA_ClearFlag(WS2812_DMA_Flag);
	TIM_Cmd(WS2812_TIM,DISABLE);
	WS2812_Reset();
}



/**
 * @brief  ��WS2812 ��ʾ����
 * @param  ��LED_Num������ʾLED�ĸ���
 * @param  ��RGB_Data����RGB��ɫ ���磺0xffffff���׹⣩
 * @retval ����
 * @doc    ����
 */
void WS2812_Display_Color(uint8_t LED_Num,uint32_t RGB_Data)
{
	uint8_t R,G,B,i,j;	
	R = (uint8_t)(RGB_Data>>16);
	G = (uint8_t)((RGB_Data&0x00ff00)>>8);
	B = (uint8_t)(RGB_Data&0x0000ff);
	RGB_Data = (G<<16) | (R<<8) | B;
	for(j=0;j<LED_Num;j++)
	{
		for(i=0;i<24;i++)
		{
			/*��Ϊ���ݷ��͵�˳����GRB����λ�ȷ������ԴӸ�λ��ʼ�жϣ��жϺ�Ƚ�ֵ�ȷ��뻺������*/
			WS2812_LED_Buffer[j*24+i] = ((RGB_Data << i) & 0x800000) ? WS2812_T1H : WS2812_T0H;
		}
	}
	WS2812_LED_Buffer[j*24+i+1] = 0;
	
	WS2812_Display(LED_Num);

}

/* END OF FILE ------------------------------------------------------------------*/


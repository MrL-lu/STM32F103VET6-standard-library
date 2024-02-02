/* ����ͷ�ļ� ------------------------------------------------------------------*/  
#include "./ws2812/ws2812.h"  //����ws2812����ͷ�ļ�

/* ����ȫ�ֱ��� ------------------------------------------------------------------*/
uint16_t	WS2812_LED_Buffer[WS2812_Data_Bit*WS2812_LED_Number+1];

#if WS2812Breath_Flag
const uint16_t index_wave[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 
                               4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 12, 12, 
                               13, 13, 14, 14, 15, 15, 16, 16, 17, 18, 18, 19, 20, 20, 21, 22, 23, 24, 25, 25, 26, 27, 28, 30, 31, 32, 33, 
                               34, 36, 37, 38, 40, 41, 43, 45, 46, 48, 50, 52, 54, 56, 58, 60, 62, 65, 67, 70, 72, 75, 78, 81, 84, 87, 90, 
                               94, 97, 101, 105, 109, 113, 117, 122, 126, 131, 136, 141, 146, 152, 158, 164, 170, 176, 183, 190, 197, 205, 
                               213, 221, 229, 238, 247, 255, 255, 247, 238, 229, 221, 213, 205, 197, 190, 183, 176, 170, 164, 158, 152, 146, 
                               141, 136, 131, 126, 122, 117, 113, 109, 105, 101, 97, 94, 90, 87, 84, 81, 78, 75, 72, 70, 67, 65, 62, 60, 58, 
                               56, 54, 52, 50, 48, 46, 45, 43, 41, 40, 38, 37, 36, 34, 33, 32, 31, 30, 28, 27, 26, 25, 25, 24, 23, 22, 21, 20, 
                               20, 19, 18, 18, 17, 16, 16, 15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10, 10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 7, 6, 
                               6, 6, 6, 6, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
                               2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};  

#endif


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
	
	WS2812_TIM_OCXInit(WS2812_TIM,&WS2812_TIM_OCInitStructure);	//��ʼ������
	WS2812_TIM_OCXPreloadConfig(WS2812_TIM,TIM_OCPreload_Enable);	//ʹ��TIM3��CCR3�ϵ�Ԥװ�ؼĴ���
	
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
	R = (uint8_t)(RGB_Data>>16);//ȡ��Red����ֵ
	G = (uint8_t)((RGB_Data&0x00ff00)>>8);//ȡ��Green����ֵ
	B = (uint8_t)(RGB_Data&0x0000ff);//ȡ��Blue����ֵ
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
	WS2812_Reset();//����WS2812�ĸ�λ�źţ����û�У��ᵼ����ɫ�������
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
	R = (uint8_t)(RGB_Data>>16);//ȡ��Red����ֵ
	G = (uint8_t)((RGB_Data&0x00ff00)>>8);//ȡ��Green����ֵ
	B = (uint8_t)(RGB_Data&0x0000ff);//ȡ��Blue����ֵ
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

/**
 * @brief  ��WS2812 ��ɫ���ú���-����ָ����LEDΪ�ض���ɫ
 * @param  ��LED_Num ��ָ����WS2812
 * @param  ��RGB_Data��RGB��ɫ ���磺0xffffff���׹⣩
 * @retval ����
 * @doc    ����
 */
void WS2812_Set_LED_Color(uint8_t LED_Num,uint32_t RGB_Data)
{
    uint8_t R,G,B,i,j;
    R = (uint8_t)(RGB_Data>>16);//ȡ��Red����ֵ
	G = (uint8_t)((RGB_Data&0x00ff00)>>8);//ȡ��Green����ֵ
	B = (uint8_t)(RGB_Data&0x0000ff);//ȡ��Blue����ֵ
	RGB_Data = (G<<16) | (R<<8) | B;
    for(i=0;i<24;i++)
	{
		/*��Ϊ���ݷ��͵�˳����GRB����λ�ȷ������ԴӸ�λ��ʼ�жϣ��жϺ�Ƚ�ֵ�ȷ��뻺������*/
		WS2812_LED_Buffer[(LED_Num-1)*24+i] = ((RGB_Data << i) & 0x800000) ? WS2812_T1H : WS2812_T0H;
	}
    WS2812_LED_Buffer[j*24+i+1] = 0;
    WS2812_Display(LED_Num);
}




static float min(float a,float b,float c)
{
    float m;
    m = a < b ? a : b;
    return ( m < c ? m : c);
}

static float max(float a,float b,float c)
{
    float m;
    m = a > b ? a : b;
    return ( m > c ? m : c);
}


/**
 * @brief  ��RGBתΪHSV
 * @param  ��r,g,b :RGBֵ
 * @param  ��h,s,v :HSVֵ
 * @retval ����
 * @doc    ����
 */
static void RGB_To_HSV(uint8_t r, uint8_t g, uint8_t b, float *h, float *s, float *v)
{
    float red, green ,blue;
    float cmax, cmin, delta;
  
    red = (float)r / 255;
    green = (float)g / 255;
    blue = (float)b / 255;
  
    cmax = max(red, green, blue);
    cmin = min(red, green, blue);
    delta = cmax - cmin;
    
    /* H */
    if(delta == 0)
    {
        *h = 0;
    }
    else
    {
        if(cmax == red)
        {
            if(green >= blue)
            {
                *h = 60 * ((green - blue) / delta);
            }
            else
            {
                *h = 60 * ((green - blue) / delta) + 360;
            }
        }
        else if(cmax == green)
        {
            *h = 60 * ((blue - red) / delta + 2);
        }
        else if(cmax == blue) 
        {
            *h = 60 * ((red - green) / delta + 4);
        }
  }
    /* S */
    if(cmax == 0)
    {
        *s = 0;
    }
    else
    {
        *s = delta / cmax;
    }
    /* V */
    *v = cmax;
}

/**
 * @brief  ��HSVתΪRGB
 * @param  ��h,s,v :HSVֵ
 * @param  ��r,g,b :RGBֵ
 * @retval ����
 * @doc    ����
 */
static void HSV_To_RGB(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b)
{
    int hi = ((int)h / 60) % 6;
    float f = h * 1.0 / 60 - hi;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1- (1 - f) * s);
    switch (hi)
    {
        case 0:
            *r = 255 * v;
            *g = 255 * t;
            *b = 255 * p;
            break;
        case 1:
            *r = 255 * q;
            *g = 255 * v;
            *b = 255 * p;
            break;
        case 2:
            *r = 255 * p;
            *g = 255 * v;
            *b = 255 * t;
            break;
        case 3:
            *r = 255 * p;
            *g = 255 * q;
            *b = 255 * v;
            break;
        case 4:
            *r = 255 * t;
            *g = 255 * p;
            *b = 255 * v;
            break;
        case 5:
            *r = 255 * v;
            *g = 255 * p;
            *b = 255 * q;
            break;
    }
}

/**
 * @brief  ��WS2812ʵ�ֺ����Ƴ���
 * @retval ����
 * @doc    ����
 */
void WS2812_BreathingLight(uint16_t LED_Num,uint32_t RGB_Data)
{
    uint8_t R,G,B,i,j;	
    float H,S,V;

	R = (uint8_t)(RGB_Data>>16);//ȡ��Red����ֵ
	G = (uint8_t)((RGB_Data&0x00ff00)>>8);//ȡ��Green����ֵ
	B = (uint8_t)(RGB_Data&0x0000ff);//ȡ��Blue����ֵ

    RGB_Data = (G<<16) | (R<<8) | B;

    RGB_To_HSV(R,G,B,&H,&S,&V); //RGBת��ΪHSV
    
    for(uint16_t n =0;n < 166 ;n++) //ͨ���������ߵ�������ֵ
    {
        V = ((float)(index_wave[n] % 255));
        HSV_To_RGB(H,S,V,&R,&G,&B); //��HSVתΪRGB
        RGB_Data = (G<<16) | (R<<8) | B;
        
        RGB_Data = ~RGB_Data;   //����������Ӱ���������
        
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
        SysTick_Delay_ms(50);	//��ʱ30ms
    }
}

/* END OF FILE ------------------------------------------------------------------*/


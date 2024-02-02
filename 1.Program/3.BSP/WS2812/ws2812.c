/* 引用头文件 ------------------------------------------------------------------*/  
#include "./ws2812/ws2812.h"  //引用ws2812配置头文件

/* 定义全局变量 ------------------------------------------------------------------*/
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


/* 程序开始 ----------------------------------------------------------------------*/

/**
 * @brief  ：WS2812 配置函数
 * @param  ：WS2812_Byte 
 * @retval ：无
 * @doc    ：无
 */
void WS2812_Config(uint16_t WS2812_Byte)
{
	/*定义结构体*/
	GPIO_InitTypeDef			WS2812_GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef		WS2812_TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  			WS2812_TIM_OCInitStructure;
	DMA_InitTypeDef 			WS2812_DMA_InitStructure;
	/*开启WS2812对应的GPIO外设时钟*/
	RCC_APB2PeriphClockCmd(WS2812_GPIO_CLK,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	/*设置为GPIO引脚模式为复用推挽输出*/
	WS2812_GPIO_InitStructure.GPIO_Mode		=	GPIO_Mode_AF_PP;
	/*选择要控制的GPIO引脚*/
	WS2812_GPIO_InitStructure.GPIO_Pin		=	WS2812_GPIO_PIN;
	/*设置GPIO引脚速率为50MHz*/
	WS2812_GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
	/*调用库函数，初始化GPIO*/
	GPIO_Init(WS2812_GPIO_PORT,&WS2812_GPIO_InitStructure);
	
	
	WS2812_TIM_TimeBaseStructure.TIM_Prescaler		=	0;	//定时器分频：(0+1)=1,不分频
	WS2812_TIM_TimeBaseStructure.TIM_CounterMode	=	TIM_CounterMode_Up;	//向上计数模式
	WS2812_TIM_TimeBaseStructure.TIM_Period			=	(SystemCoreClock/(WS2812_Byte*1000))-1;	//800K
	WS2812_TIM_TimeBaseStructure.TIM_ClockDivision	=	TIM_CKD_DIV1;//时钟分割,设置时钟分频系数：不分频(这里用不到)
	TIM_TimeBaseInit(WS2812_TIM,&WS2812_TIM_TimeBaseStructure);
	
	WS2812_TIM_OCInitStructure.TIM_OCMode			=	TIM_OCMode_PWM1;	//配置为PWM模式1
	WS2812_TIM_OCInitStructure.TIM_OutputState		=	TIM_OutputState_Enable;	//使能比较输出
//	WS2812_TIM_OCInitStructure.TIM_Pulse			= 	0;	//待装入捕获比较寄存器的脉冲值(此程序不用加初值)
	WS2812_TIM_OCInitStructure.TIM_OCPolarity		=	TIM_OCPolarity_High;	//输出极性:TIM输出比较极性高
	
	WS2812_TIM_OCXInit(WS2812_TIM,&WS2812_TIM_OCInitStructure);	//初始化参数
	WS2812_TIM_OCXPreloadConfig(WS2812_TIM,TIM_OCPreload_Enable);	//使能TIM3在CCR3上的预装载寄存器
	
	TIM_Cmd(WS2812_TIM,DISABLE);	//失能TIM3 防止出现错误信号波形
	
	TIM_DMACmd(WS2812_TIM,WS2812_TIM_DMA,ENABLE);		//使能TIM2_CH3的DMA功能(CC2对应通道3)
	
	WS2812_DMA_InitStructure.DMA_PeripheralBaseAddr	=	(uint32_t)&WS2812_DMA_Addr;		//设置DMA目的地址
	WS2812_DMA_InitStructure.DMA_MemoryBaseAddr		=	(uint32_t)WS2812_LED_Buffer;	//设置DMA源地址
	WS2812_DMA_InitStructure.DMA_DIR				=	DMA_DIR_PeripheralDST;			//方向：从内存SendBuff到内存ReceiveBuff 
	WS2812_DMA_InitStructure.DMA_BufferSize			=	(24*WS2812_LED_Number);			//一次传输大小DMA_BufferSize=SendBuffSize
	WS2812_DMA_InitStructure.DMA_PeripheralInc		=	DMA_PeripheralInc_Disable;		//ReceiveBuff地址不增
	WS2812_DMA_InitStructure.DMA_MemoryInc			=	DMA_MemoryInc_Enable;			//SendBuff地址自增
	WS2812_DMA_InitStructure.DMA_PeripheralDataSize	=	DMA_PeripheralDataSize_HalfWord;//ReceiveBuff数据单位,16bit
	WS2812_DMA_InitStructure.DMA_MemoryDataSize		=	DMA_MemoryDataSize_HalfWord;	//SENDBUFF_SIZE数据单位,16bit
	WS2812_DMA_InitStructure.DMA_Mode				=	DMA_Mode_Normal;				//DMA模式：正常模式(传输一次)
	WS2812_DMA_InitStructure.DMA_Priority			=	DMA_Priority_Medium;			//优先级：中
	WS2812_DMA_InitStructure.DMA_M2M				=	DMA_M2M_Disable;				//内存到内存的传输
	
	DMA_Init(WS2812_DMA_Channel,&WS2812_DMA_InitStructure);									//配置DMA1的2通道(不同定时器的通道不一样)
	DMA_Cmd(WS2812_DMA_Channel,DISABLE);														//先失能DMA传输
	
}

/**
 * @brief  ：WS2812 RESET信号 低电平>280us
 * @param  ：无
 * @retval ：无
 * @doc    ：无
 */
void  WS2812_Reset(void)
{
	TIM_Cmd(WS2812_TIM,DISABLE);	//失能TIM定时器
	GPIO_ResetBits(WS2812_GPIO_PORT,WS2812_GPIO_PIN);	//输出低电平
	SysTick_Delay_ms(1);	//延时1ms
}

/**
 * @brief  ：WS2812 颜色设置函数-设置全部颜色为统一颜色
 * @param  ：RGB_Data——RGB颜色 例如：0xffffff（白光）
 * @retval ：无
 * @doc    ：无
 */
void WS2812_SetColor(uint32_t RGB_Data)
{
	uint8_t R,G,B,i,j;	
	R = (uint8_t)(RGB_Data>>16);//取出Red的数值
	G = (uint8_t)((RGB_Data&0x00ff00)>>8);//取出Green的数值
	B = (uint8_t)(RGB_Data&0x0000ff);//取出Blue的数值
	RGB_Data = (G<<16) | (R<<8) | B;
	for(j=0;j<WS2812_LED_Number;j++)
	{
		for(i=0;i<24;i++)
		{
			/*因为数据发送的顺序是GRB，高位先发，所以从高位开始判断，判断后比较值先放入缓存数组*/
			WS2812_LED_Buffer[j*24+i] = ((RGB_Data << i) & 0x800000) ? WS2812_T1H : WS2812_T0H;
		}
	}
	WS2812_LED_Buffer[j*24+i+1] = 0;
}

/**
 * @brief  ：WS2812 显示函数
 * @param  ：LED_Num——显示LED的个数
 * @retval ：无
 * @doc    ：无
 */
void  WS2812_Display(uint8_t LED_Num)
{
	/*DMA发送数据函数，设置发送的通道和数据数量*/
	DMA_SetCurrDataCounter(WS2812_DMA_Channel,(24*LED_Num)+1);
	/*使能DMA传输*/
	DMA_Cmd(WS2812_DMA_Channel,ENABLE);
	/*使能TIM，开始PWM输出*/
	TIM_Cmd(WS2812_TIM,ENABLE);
	/*等待DMA发送完毕*/
	while(DMA_GetFlagStatus(WS2812_DMA_Flag) != SET);
	/*失能DMA传输*/
	DMA_Cmd(WS2812_DMA_Channel,DISABLE);
	/*清除DMA标志位*/
	DMA_ClearFlag(WS2812_DMA_Flag);
	TIM_Cmd(WS2812_TIM,DISABLE);
	WS2812_Reset();//发送WS2812的复位信号，如果没有，会导致颜色传输错码
}



/**
 * @brief  ：WS2812 显示函数
 * @param  ：LED_Num——显示LED的个数
 * @param  ：RGB_Data——RGB颜色 例如：0xffffff（白光）
 * @retval ：无
 * @doc    ：无
 */
void WS2812_Display_Color(uint8_t LED_Num,uint32_t RGB_Data)
{
	uint8_t R,G,B,i,j;	
	R = (uint8_t)(RGB_Data>>16);//取出Red的数值
	G = (uint8_t)((RGB_Data&0x00ff00)>>8);//取出Green的数值
	B = (uint8_t)(RGB_Data&0x0000ff);//取出Blue的数值
	RGB_Data = (G<<16) | (R<<8) | B;
	for(j=0;j<LED_Num;j++)
	{
		for(i=0;i<24;i++)
		{
			/*因为数据发送的顺序是GRB，高位先发，所以从高位开始判断，判断后比较值先放入缓存数组*/
			WS2812_LED_Buffer[j*24+i] = ((RGB_Data << i) & 0x800000) ? WS2812_T1H : WS2812_T0H;
		}
	}
	WS2812_LED_Buffer[j*24+i+1] = 0;
	WS2812_Display(LED_Num);
}

/**
 * @brief  ：WS2812 颜色设置函数-设置指定的LED为特定颜色
 * @param  ：LED_Num ：指定的WS2812
 * @param  ：RGB_Data：RGB颜色 例如：0xffffff（白光）
 * @retval ：无
 * @doc    ：无
 */
void WS2812_Set_LED_Color(uint8_t LED_Num,uint32_t RGB_Data)
{
    uint8_t R,G,B,i,j;
    R = (uint8_t)(RGB_Data>>16);//取出Red的数值
	G = (uint8_t)((RGB_Data&0x00ff00)>>8);//取出Green的数值
	B = (uint8_t)(RGB_Data&0x0000ff);//取出Blue的数值
	RGB_Data = (G<<16) | (R<<8) | B;
    for(i=0;i<24;i++)
	{
		/*因为数据发送的顺序是GRB，高位先发，所以从高位开始判断，判断后比较值先放入缓存数组*/
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
 * @brief  ：RGB转为HSV
 * @param  ：r,g,b :RGB值
 * @param  ：h,s,v :HSV值
 * @retval ：无
 * @doc    ：无
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
 * @brief  ：HSV转为RGB
 * @param  ：h,s,v :HSV值
 * @param  ：r,g,b :RGB值
 * @retval ：无
 * @doc    ：无
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
 * @brief  ：WS2812实现呼吸灯程序
 * @retval ：无
 * @doc    ：无
 */
void WS2812_BreathingLight(uint16_t LED_Num,uint32_t RGB_Data)
{
    uint8_t R,G,B,i,j;	
    float H,S,V;

	R = (uint8_t)(RGB_Data>>16);//取出Red的数值
	G = (uint8_t)((RGB_Data&0x00ff00)>>8);//取出Green的数值
	B = (uint8_t)(RGB_Data&0x0000ff);//取出Blue的数值

    RGB_Data = (G<<16) | (R<<8) | B;

    RGB_To_HSV(R,G,B,&H,&S,&V); //RGB转换为HSV
    
    for(uint16_t n =0;n < 166 ;n++) //通过呼吸曲线调整亮度值
    {
        V = ((float)(index_wave[n] % 255));
        HSV_To_RGB(H,S,V,&R,&G,&B); //将HSV转为RGB
        RGB_Data = (G<<16) | (R<<8) | B;
        
        RGB_Data = ~RGB_Data;   //上下拉电阻影响这个程序
        
        for(j=0;j<LED_Num;j++)
        {
            for(i=0;i<24;i++)
            {
                /*因为数据发送的顺序是GRB，高位先发，所以从高位开始判断，判断后比较值先放入缓存数组*/
                WS2812_LED_Buffer[j*24+i] = ((RGB_Data << i) & 0x800000) ? WS2812_T1H : WS2812_T0H;
            }
        }
        WS2812_LED_Buffer[j*24+i+1] = 0;
        WS2812_Display(LED_Num);
        SysTick_Delay_ms(50);	//延时30ms
    }
}

/* END OF FILE ------------------------------------------------------------------*/


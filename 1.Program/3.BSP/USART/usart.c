/* 引用头文件 ------------------------------------------------------------------*/  
#include "./USART/usart.h"  //引用usart串口配置头文件

/* 定义全局变量 ------------------------------------------------------------------*/


/* 程序开始 ----------------------------------------------------------------------*/
/**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  /* 嵌套向量中断控制器组选择 */
  /* 提示 NVIC_PriorityGroupConfig() 在整个工程只需要调用一次来配置优先级分组*/
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  USART GPIO 配置,工作参数配置
  * @param  无
  * @retval 无
  */
void Usart_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    // 打开串口GPIO的时钟
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	// 打开串口外设的时钟
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);
	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
    // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(DEBUG_USARTX, &USART_InitStructure);
	// 串口中断优先级配置
	NVIC_Configuration();
	// 使能串口接收中断
	USART_ITConfig(DEBUG_USARTX, USART_IT_RXNE, ENABLE);	
	// 使能串口
	USART_Cmd(DEBUG_USARTX, ENABLE);
}

/**
  * @brief  USART 发送一个字符
  * @param  pUSARTX : 填写串口
  * @param  ch      : 输入字符
  * @retval 无
  */
void Usart_SendByte(USART_TypeDef *pUSARTX,uint8_t data)
{
    USART_SendData(pUSARTX,data); //发送一个字符数据到USART
    while(USART_GetFlagStatus(pUSARTX,USART_FLAG_TXE) == RESET);    //等待发送数据寄存器为空
}

/**
  * @brief  USART 发送字符串
  * @param  pUSARTX : 填写串口
  * @param  ch      : 输入字符
  * @retval 无
  */
void Usart_SendString(USART_TypeDef *pUSARTX,char *data)
{
    uint32_t i=0;
    do{
        Usart_SendByte(pUSARTX,*(data+i));
    }while(*(data+i) != '\0');
    while(USART_GetFlagStatus(pUSARTX,USART_FLAG_TC) == RESET);    //等待发送完成
}

/**
  * @brief  ：USART 发送8位数组
  * @param  ：pUSARTX：USART1
  * @param  ：array：数组
  * @param  ：num：传输个数
  * @retval ：返回值说明。
  * @doc    ：无
  */
void Usart_SendArray( USART_TypeDef *pUSARTX, uint8_t *array, uint16_t num)
{
    uint8_t i;
	for(i=0; i<num; i++)
    {
	    /* 发送一个字节数据到USART */
	    Usart_SendByte(pUSARTX,array[i]);	
    }
	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTX,USART_FLAG_TC)==RESET);
}

/**
  * @brief  ：USART 发送一个16位数
  * @param  ：pUSARTx：USART1
  * @param  ：data：16位数
  * @retval ：返回值说明。
  * @doc    ：无
  */
void Usart_SendHalfWord( USART_TypeDef * pUSARTX, uint16_t data)
{
	uint8_t temp_h, temp_l;
	/* 取出高八位 */
	temp_h = (data&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = data&0XFF;
	/* 发送高八位 */
	USART_SendData(pUSARTX,temp_h);	
	while (USART_GetFlagStatus(pUSARTX, USART_FLAG_TXE) == RESET);
	/* 发送低八位 */
	USART_SendData(pUSARTX,temp_l);	
	while (USART_GetFlagStatus(pUSARTX, USART_FLAG_TXE) == RESET);	
}

/**
  * @brief  ：USART 发送一个32位数
  * @param  ：pUSARTx：USART1
  * @param  ：data：16位数
  * @retval ：返回值说明。
  * @doc    ：无
  */
void Usart_SendWord(USART_TypeDef * pUSARTX, uint32_t data)
{
    uint8_t temp[4];
    temp[0] = (data >> 24) & 0xFF;
    temp[1] = (data >> 16) & 0xFF;
    temp[2] = (data >> 8) & 0xFF;
    temp[3] = data & 0xFF;
    for(uint8_t i=0;i<4;i++)
    {
        USART_SendData(pUSARTX,temp[i]);
        while (USART_GetFlagStatus(pUSARTX, USART_FLAG_TXE) == RESET);
    }
}


//重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(DEBUG_USARTX, (uint8_t) ch);
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(DEBUG_USARTX, USART_FLAG_TXE) == RESET);		
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(DEBUG_USARTX, USART_FLAG_RXNE) == RESET);
		return (int)USART_ReceiveData(DEBUG_USARTX);
}

/* END OF FILE ------------------------------------------------------------------*/

/* ����ͷ�ļ� ------------------------------------------------------------------*/  
#include "./USART/usart.h"  //����usart��������ͷ�ļ�

/* ����ȫ�ֱ��� ------------------------------------------------------------------*/


/* ����ʼ ----------------------------------------------------------------------*/
/**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  /* Ƕ�������жϿ�������ѡ�� */
  /* ��ʾ NVIC_PriorityGroupConfig() ����������ֻ��Ҫ����һ�����������ȼ�����*/
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  USART GPIO ����,������������
  * @param  ��
  * @retval ��
  */
void Usart_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    // �򿪴���GPIO��ʱ��
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	// �򿪴��������ʱ��
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);
	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
    // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(DEBUG_USARTX, &USART_InitStructure);
	// �����ж����ȼ�����
	NVIC_Configuration();
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(DEBUG_USARTX, USART_IT_RXNE, ENABLE);	
	// ʹ�ܴ���
	USART_Cmd(DEBUG_USARTX, ENABLE);
}

/**
  * @brief  USART ����һ���ַ�
  * @param  pUSARTX : ��д����
  * @param  ch      : �����ַ�
  * @retval ��
  */
void Usart_SendByte(USART_TypeDef *pUSARTX,uint8_t data)
{
    USART_SendData(pUSARTX,data); //����һ���ַ����ݵ�USART
    while(USART_GetFlagStatus(pUSARTX,USART_FLAG_TXE) == RESET);    //�ȴ��������ݼĴ���Ϊ��
}

/**
  * @brief  USART �����ַ���
  * @param  pUSARTX : ��д����
  * @param  ch      : �����ַ�
  * @retval ��
  */
void Usart_SendString(USART_TypeDef *pUSARTX,char *data)
{
    uint32_t i=0;
    do{
        Usart_SendByte(pUSARTX,*(data+i));
    }while(*(data+i) != '\0');
    while(USART_GetFlagStatus(pUSARTX,USART_FLAG_TC) == RESET);    //�ȴ��������
}

/**
  * @brief  ��USART ����8λ����
  * @param  ��pUSARTX��USART1
  * @param  ��array������
  * @param  ��num���������
  * @retval ������ֵ˵����
  * @doc    ����
  */
void Usart_SendArray( USART_TypeDef *pUSARTX, uint8_t *array, uint16_t num)
{
    uint8_t i;
	for(i=0; i<num; i++)
    {
	    /* ����һ���ֽ����ݵ�USART */
	    Usart_SendByte(pUSARTX,array[i]);	
    }
	/* �ȴ�������� */
	while(USART_GetFlagStatus(pUSARTX,USART_FLAG_TC)==RESET);
}

/**
  * @brief  ��USART ����һ��16λ��
  * @param  ��pUSARTx��USART1
  * @param  ��data��16λ��
  * @retval ������ֵ˵����
  * @doc    ����
  */
void Usart_SendHalfWord( USART_TypeDef * pUSARTX, uint16_t data)
{
	uint8_t temp_h, temp_l;
	/* ȡ���߰�λ */
	temp_h = (data&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = data&0XFF;
	/* ���͸߰�λ */
	USART_SendData(pUSARTX,temp_h);	
	while (USART_GetFlagStatus(pUSARTX, USART_FLAG_TXE) == RESET);
	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTX,temp_l);	
	while (USART_GetFlagStatus(pUSARTX, USART_FLAG_TXE) == RESET);	
}

/**
  * @brief  ��USART ����һ��32λ��
  * @param  ��pUSARTx��USART1
  * @param  ��data��16λ��
  * @retval ������ֵ˵����
  * @doc    ����
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


//�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(DEBUG_USARTX, (uint8_t) ch);
		/* �ȴ�������� */
		while (USART_GetFlagStatus(DEBUG_USARTX, USART_FLAG_TXE) == RESET);		
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(DEBUG_USARTX, USART_FLAG_RXNE) == RESET);
		return (int)USART_ReceiveData(DEBUG_USARTX);
}

/* END OF FILE ------------------------------------------------------------------*/

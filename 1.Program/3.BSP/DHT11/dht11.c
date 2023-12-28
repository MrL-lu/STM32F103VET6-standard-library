/* ����ͷ�ļ� ------------------------------------------------------------------*/  
#include "./DHT11/dht11.h"  //������ʪ�ȴ�����DHT11����ͷ�ļ�

/* ����ȫ�ֱ��� ------------------------------------------------------------------*/

/* ����ʼ ----------------------------------------------------------------------*/

 /**
  * @brief  DHT11 GPIO���ű�Ϊ�������ģʽ
  * @param  ��
  * @retval ��
  */
static void DHT11_Mode_Out_PP(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef DHT11_GPIO_InitStructure;
	DHT11_GPIO_InitStructure.GPIO_Mode		= GPIO_Mode_Out_PP;
	DHT11_GPIO_InitStructure.GPIO_Pin		= DHT11_Dout_GPIO_PIN;
	DHT11_GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_Init(DHT11_Dout_GPIO_PORT,&DHT11_GPIO_InitStructure);
}


 /**
  * @brief  DHT11 GPIO���ű�Ϊ��������ģʽ
  * @param  ��
  * @retval ��
  */
static void DHT11_Mode_IPU(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef DHT11_GPIO_InitStructure;
	DHT11_GPIO_InitStructure.GPIO_Mode		= GPIO_Mode_IPU;
	DHT11_GPIO_InitStructure.GPIO_Pin		= DHT11_Dout_GPIO_PIN;
	DHT11_GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_Init(DHT11_Dout_GPIO_PORT,&DHT11_GPIO_InitStructure);
}

/**
  * @brief  DHT11 ��ʼ������
  * @param  ��
  * @retval ��
  */
void DHT11_Init(void)
{
	/*����DHT11_Dout_GPIO_PORT������ʱ��*/
	DHT11_Dout_SCK_APBxClock_FUN(DHT11_Dout_GPIO_CLK,ENABLE);
	DHT11_Mode_Out_PP();//��ʼ������Ϊ���ģʽ
	GPIO_SetBits (DHT11_Dout_GPIO_PORT,DHT11_Dout_GPIO_PIN);//����GPIO������1s
}


/**
  * @brief  DHT11 ��ȡһ���ֽ� MSB����
  * @param  ��
  * @retval ��
  */
static uint8_t DHT11_ReadByte(void)
{
	uint8_t i,data=0;
	for(i=0;i<8;i++)
	{
		/*ÿbit��50us�͵�ƽ���ÿ�ʼ����ѯֱ���ӻ����� ��50us �͵�ƽ ����*/ 
		while(GPIO_ReadInputDataBit(DHT11_Dout_GPIO_PORT,DHT11_Dout_GPIO_PIN) == Bit_RESET);
		/*DHT11 ��26~28us�ĸߵ�ƽ��ʾ��0������70us�ߵ�ƽ��ʾ��1����
		 *ͨ����� x us��ĵ�ƽ��������������״ ��x ���������ʱ 
		 */
		SysTick_Delay_us(40);//��ʱ40us
		if(GPIO_ReadInputDataBit(DHT11_Dout_GPIO_PORT,DHT11_Dout_GPIO_PIN) == 1)
		{
			data |=(uint8_t)(0x01<<(7-i));  //�ѵ�7-iλ��1��MSB���� 
			/* �ȴ�����1�ĸߵ�ƽ���� */
			while(GPIO_ReadInputDataBit(DHT11_Dout_GPIO_PORT,DHT11_Dout_GPIO_PIN) == Bit_SET);
		}
		else
		{
			data&=(uint8_t)~(0x01<<(7-i)); //�ѵ�7-iλ��0��MSB����
		}
	}
	return data;
}


/**
 * @brief		��DHT11���������ݶ�ȡ
 * @retval	������ֵ˵��
 * @doc		����
 */
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data)
{
	DHT11_Mode_Out_PP();
	GPIO_ResetBits (DHT11_Dout_GPIO_PORT,DHT11_Dout_GPIO_PIN);//����������
	SysTick_Delay_ms(18);//��ʱ18ms������С�� 18ms����󲻵ó��� 30ms��
	GPIO_SetBits (DHT11_Dout_GPIO_PORT,DHT11_Dout_GPIO_PIN);//����GPIO�������ͷ�����
	SysTick_Delay_us(30);//��ʱ30us
	DHT11_Mode_IPU();//��������Ϊ��������ģʽ
	if(GPIO_ReadInputDataBit(DHT11_Dout_GPIO_PORT,DHT11_Dout_GPIO_PIN) == Bit_RESET)//�ж�DHT11�Ƿ���Ӧ
	{
		while(GPIO_ReadInputDataBit(DHT11_Dout_GPIO_PORT,DHT11_Dout_GPIO_PIN) == Bit_RESET);
		while(GPIO_ReadInputDataBit(DHT11_Dout_GPIO_PORT,DHT11_Dout_GPIO_PIN) == Bit_SET);
		
		/************************** DHT11 ���ݽ���********************************/
		DHT11_Data->DHT11_humi_int		=	DHT11_ReadByte();
		DHT11_Data->DHT11_humi_deci		=	DHT11_ReadByte();
		DHT11_Data->DHT11_temp_int		=	DHT11_ReadByte();
		DHT11_Data->DHT11_temp_deci		=	DHT11_ReadByte();
		DHT11_Data->DHT11_check_sum		=	DHT11_ReadByte();
		
		DHT11_Mode_Out_PP();
		GPIO_SetBits (DHT11_Dout_GPIO_PORT,DHT11_Dout_GPIO_PIN);//����GPIO
		
		/************************** DHT11 ����У��********************************/
		if(DHT11_Data->DHT11_check_sum == DHT11_Data->DHT11_humi_int + DHT11_Data->DHT11_humi_deci + DHT11_Data->DHT11_temp_int + DHT11_Data->DHT11_temp_deci)
		{
			return 1;	//�ɹ�
		}
		else
		{
			return 0;
		}
	}
	else
		return 0;
}



/* END OF FILE ------------------------------------------------------------------*/

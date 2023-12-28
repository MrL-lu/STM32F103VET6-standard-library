/* 引用头文件 ------------------------------------------------------------------*/  
#include "./DHT11/dht11.h"  //引用温湿度传感器DHT11配置头文件

/* 定义全局变量 ------------------------------------------------------------------*/

/* 程序开始 ----------------------------------------------------------------------*/

 /**
  * @brief  DHT11 GPIO引脚变为推挽输出模式
  * @param  无
  * @retval 无
  */
static void DHT11_Mode_Out_PP(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef DHT11_GPIO_InitStructure;
	DHT11_GPIO_InitStructure.GPIO_Mode		= GPIO_Mode_Out_PP;
	DHT11_GPIO_InitStructure.GPIO_Pin		= DHT11_Dout_GPIO_PIN;
	DHT11_GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_Init(DHT11_Dout_GPIO_PORT,&DHT11_GPIO_InitStructure);
}


 /**
  * @brief  DHT11 GPIO引脚变为上拉输入模式
  * @param  无
  * @retval 无
  */
static void DHT11_Mode_IPU(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef DHT11_GPIO_InitStructure;
	DHT11_GPIO_InitStructure.GPIO_Mode		= GPIO_Mode_IPU;
	DHT11_GPIO_InitStructure.GPIO_Pin		= DHT11_Dout_GPIO_PIN;
	DHT11_GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_Init(DHT11_Dout_GPIO_PORT,&DHT11_GPIO_InitStructure);
}

/**
  * @brief  DHT11 初始化函数
  * @param  无
  * @retval 无
  */
void DHT11_Init(void)
{
	/*开启DHT11_Dout_GPIO_PORT的外设时钟*/
	DHT11_Dout_SCK_APBxClock_FUN(DHT11_Dout_GPIO_CLK,ENABLE);
	DHT11_Mode_Out_PP();//初始化配置为输出模式
	GPIO_SetBits (DHT11_Dout_GPIO_PORT,DHT11_Dout_GPIO_PIN);//拉高GPIO，保持1s
}


/**
  * @brief  DHT11 读取一个字节 MSB先行
  * @param  无
  * @retval 无
  */
static uint8_t DHT11_ReadByte(void)
{
	uint8_t i,data=0;
	for(i=0;i<8;i++)
	{
		/*每bit以50us低电平标置开始，轮询直到从机发出 的50us 低电平 结束*/ 
		while(GPIO_ReadInputDataBit(DHT11_Dout_GPIO_PORT,DHT11_Dout_GPIO_PIN) == Bit_RESET);
		/*DHT11 以26~28us的高电平表示“0”，以70us高电平表示“1”，
		 *通过检测 x us后的电平即可区别这两个状 ，x 即下面的延时 
		 */
		SysTick_Delay_us(40);//延时40us
		if(GPIO_ReadInputDataBit(DHT11_Dout_GPIO_PORT,DHT11_Dout_GPIO_PIN) == 1)
		{
			data |=(uint8_t)(0x01<<(7-i));  //把第7-i位置1，MSB先行 
			/* 等待数据1的高电平结束 */
			while(GPIO_ReadInputDataBit(DHT11_Dout_GPIO_PORT,DHT11_Dout_GPIO_PIN) == Bit_SET);
		}
		else
		{
			data&=(uint8_t)~(0x01<<(7-i)); //把第7-i位置0，MSB先行
		}
	}
	return data;
}


/**
 * @brief		：DHT11传感器数据读取
 * @retval	：返回值说明
 * @doc		：无
 */
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data)
{
	DHT11_Mode_Out_PP();
	GPIO_ResetBits (DHT11_Dout_GPIO_PORT,DHT11_Dout_GPIO_PIN);//拉低数据线
	SysTick_Delay_ms(18);//延时18ms，不能小于 18ms（最大不得超过 30ms）
	GPIO_SetBits (DHT11_Dout_GPIO_PORT,DHT11_Dout_GPIO_PIN);//拉高GPIO，主机释放总线
	SysTick_Delay_us(30);//延时30us
	DHT11_Mode_IPU();//配置主机为上拉输入模式
	if(GPIO_ReadInputDataBit(DHT11_Dout_GPIO_PORT,DHT11_Dout_GPIO_PIN) == Bit_RESET)//判断DHT11是否响应
	{
		while(GPIO_ReadInputDataBit(DHT11_Dout_GPIO_PORT,DHT11_Dout_GPIO_PIN) == Bit_RESET);
		while(GPIO_ReadInputDataBit(DHT11_Dout_GPIO_PORT,DHT11_Dout_GPIO_PIN) == Bit_SET);
		
		/************************** DHT11 数据接收********************************/
		DHT11_Data->DHT11_humi_int		=	DHT11_ReadByte();
		DHT11_Data->DHT11_humi_deci		=	DHT11_ReadByte();
		DHT11_Data->DHT11_temp_int		=	DHT11_ReadByte();
		DHT11_Data->DHT11_temp_deci		=	DHT11_ReadByte();
		DHT11_Data->DHT11_check_sum		=	DHT11_ReadByte();
		
		DHT11_Mode_Out_PP();
		GPIO_SetBits (DHT11_Dout_GPIO_PORT,DHT11_Dout_GPIO_PIN);//拉高GPIO
		
		/************************** DHT11 数据校验********************************/
		if(DHT11_Data->DHT11_check_sum == DHT11_Data->DHT11_humi_int + DHT11_Data->DHT11_humi_deci + DHT11_Data->DHT11_temp_int + DHT11_Data->DHT11_temp_deci)
		{
			return 1;	//成功
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

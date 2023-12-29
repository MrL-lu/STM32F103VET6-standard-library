/* 引用头文件 ------------------------------------------------------------------*/  
#include "./IIC/iic.h"  //引用iic配置头文件

/* 定义全局变量 ------------------------------------------------------------------*/
static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT; 
/* 程序开始 ----------------------------------------------------------------------*/

/**
 * @brief	：IIC GPIO配置函数
 * @param	：参数说明
 * @arg		：参数里面可选择参量列举
 * @note	：注释
 * @retval	：返回值说明
 * @doc		：无
 */
static void IIC_GPIO_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure; 
    
    /* 使能与 I2C 有关的时钟 */
    DEBUG_IIC_APBxClock_FUN(DEBUG_IIC_CLK,ENABLE);
    DEBUG_IIC_GPIO_APBxClock_FUN(DEBUG_IIC_GPIO_CLK,ENABLE);
    /* SCL */
    GPIO_InitStructure.GPIO_Pin = DEBUG_IIC_SCL_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // 开漏输出
    GPIO_Init(DEBUG_IIC_SCL_PORT, &GPIO_InitStructure);
    /* SDA */
    GPIO_InitStructure.GPIO_Pin = DEBUG_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // 开漏输出
    GPIO_Init(DEBUG_IIC_SDA_PORT, &GPIO_InitStructure);	
}

/**
 * @brief	：IIC 工作模式配置函数
 * @param	：无
 * @doc		：无
 */
static void IIC_Mode_Config(void)
{
    I2C_InitTypeDef  I2C_InitStructure;
    
    /* I2C 配置 */
    I2C_InitStructure.I2C_Mode                  =   I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle             =   I2C_DutyCycle_2;//占空比
    I2C_InitStructure.I2C_OwnAddress1           =   DEBUG_IIC_OwnAddress;//配置地址
    I2C_InitStructure.I2C_Ack                   =   I2C_Ack_Enable;//开启应答
    
    I2C_InitStructure.I2C_AcknowledgedAddress   =   I2C_AcknowledgedAddress_7bit;//寻址模式
    I2C_InitStructure.I2C_ClockSpeed            =   DEBUG_IIC_Speed;//通信速率
    
    I2C_Init(DEBUG_IICx,&I2C_InitStructure);//初始化
    I2C_Cmd(DEBUG_IICx,ENABLE);
    
}    

/**
 * @brief	：IIC 超时处理函数
 * @param	：无
 * @doc		：无
 */
static  uint8_t I2C_TIMEOUT_UserCallback(void)
{
  /* Block communication and all processes */
//  MPU_ERROR("I2C Timeout error!"); 
  
  return 0;
}

/**
 * @brief	：IIC 外设初始化
 * @param	：无
 * @doc		：无
 */
void DEBUG_IIC_Init(void)
{
    IIC_GPIO_Config();
    IIC_Mode_Config();
}

/**
 * @brief	：IIC 写一个字节函数
 * @param	：WriteAddr - 设备的地址
 * @param	：pBuffer   - 
 * @arg		：参数里面可选择参量列举
 * @note	：注释
 * @retval	：返回值说明
 * @doc		：无
 */
uint8_t DEBUG_IIC_WriteByte(uint8_t WriteAddr,uint8_t pBuffer)
{
    I2C_GenerateSTART(DEBUG_IICx,ENABLE);//
    I2CTimeout = I2CT_FLAG_TIMEOUT;//IIC超时时间
    
    while(!I2C_CheckEvent(DEBUG_IICx, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }
    
    I2C_Send7bitAddress(DEBUG_IICx, WriteAddr, I2C_Direction_Transmitter);
    I2CTimeout = I2CT_FLAG_TIMEOUT;//IIC超时时间
    
    while(!I2C_CheckEvent(DEBUG_IICx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
	{
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }
    
    I2C_SendData(DEBUG_IICx, WriteAddr);//发送地址
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;//IIC超时时间
    while(!I2C_CheckEvent(DEBUG_IICx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
	{
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }
    
    I2C_SendData(DEBUG_IICx,pBuffer);
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;//IIC超时时间
    while(!I2C_CheckEvent(DEBUG_IICx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))	
	{
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }
    
    I2C_GenerateSTOP(DEBUG_IICx, ENABLE);
    return 1; //正常返回1
}

/**
  * @brief   从I2C设备里面读取一块数据 
  * @param   
  *		@arg pBuffer:存放从slave读取的数据的缓冲区指针
  *		@arg SlaveAddress:从设备的地址
  *		@arg ReadAddr:接收数据的从设备的地址
  *     @arg NumByteToWrite:要从从设备读取的字节数
  * @retval  正常返回1，异常返回0
  */
uint8_t DEBUG_IIC_ReadBuffer(uint8_t *pBuffer,uint8_t SlaveAddress,uint8_t ReadAddr,uint16_t NumByteToRead)
{
    I2CTimeout = I2CT_FLAG_TIMEOUT;//IIC超时时间
    while(I2C_GetFlagStatus(DEBUG_IICx, I2C_FLAG_BUSY)) // Added by Najoua 27/08/2008    
    {
        if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }
    I2C_GenerateSTART(DEBUG_IICx, ENABLE);
    
    I2C_Send7bitAddress(DEBUG_IICx, SlaveAddress, I2C_Direction_Transmitter);
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;//IIC超时时间
    while(!I2C_CheckEvent(DEBUG_IICx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
	{
        if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }
    
    I2C_Cmd(DEBUG_IICx, ENABLE);
    
    I2C_SendData(DEBUG_IICx,ReadAddr);
    I2CTimeout = I2CT_FLAG_TIMEOUT;//IIC超时时间
    while(!I2C_CheckEvent(DEBUG_IICx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
	{
        if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }
    
    I2C_GenerateSTART(DEBUG_IICx,ENABLE);
    I2CTimeout = I2CT_FLAG_TIMEOUT;//IIC超时时间
    while(!I2C_CheckEvent(DEBUG_IICx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
	{
        if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }
    
    I2C_Send7bitAddress(DEBUG_IICx, SlaveAddress, I2C_Direction_Receiver);
    I2CTimeout = I2CT_FLAG_TIMEOUT;//IIC超时时间
    while(!I2C_CheckEvent(DEBUG_IICx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
	{
        if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }
    while(NumByteToRead)
    {
        if(NumByteToRead == 1)
        {
            I2C_AcknowledgeConfig(DEBUG_IICx, DISABLE);//关闭应答
            I2C_GenerateSTOP(DEBUG_IICx,ENABLE);
        }
        if(I2C_CheckEvent(DEBUG_IICx, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            *pBuffer = I2C_ReceiveData(DEBUG_IICx);//读取数据
            pBuffer++;
            NumByteToRead--;
        }
    }
    I2C_AcknowledgeConfig(DEBUG_IICx,ENABLE);//应答
    return 1;
}

/**
  * @brief  等待从属待机状态 
  * @param  无
  * @retval 无
  */
void DEBUG_IIC_WaitStandbyState(uint8_t SlaveAddress)
{
    __IO uint16_t i =0;
    do
    {
        I2C_GenerateSTART(DEBUG_IICx, ENABLE);
        i = I2C_ReadRegister(DEBUG_IICx, I2C_Register_SR1);
        I2C_Send7bitAddress(DEBUG_IICx, SlaveAddress, I2C_Direction_Transmitter);
    }while(!(I2C_ReadRegister(DEBUG_IICx, I2C_Register_SR1) & 0x0002));
    
    I2C_ClearFlag(DEBUG_IICx, I2C_FLAG_AF);
    I2C_GenerateSTOP(DEBUG_IICx, ENABLE);
}


/* END OF FILE ------------------------------------------------------------------*/

/* ����ͷ�ļ� ------------------------------------------------------------------*/  
#include "./IIC/iic.h"  //����iic����ͷ�ļ�

/* ����ȫ�ֱ��� ------------------------------------------------------------------*/
static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT; 
/* ����ʼ ----------------------------------------------------------------------*/

/**
 * @brief	��IIC GPIO���ú���
 * @param	������˵��
 * @arg		�����������ѡ������о�
 * @note	��ע��
 * @retval	������ֵ˵��
 * @doc		����
 */
static void IIC_GPIO_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure; 
    
    /* ʹ���� I2C �йص�ʱ�� */
    DEBUG_IIC_APBxClock_FUN(DEBUG_IIC_CLK,ENABLE);
    DEBUG_IIC_GPIO_APBxClock_FUN(DEBUG_IIC_GPIO_CLK,ENABLE);
    /* SCL */
    GPIO_InitStructure.GPIO_Pin = DEBUG_IIC_SCL_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // ��©���
    GPIO_Init(DEBUG_IIC_SCL_PORT, &GPIO_InitStructure);
    /* SDA */
    GPIO_InitStructure.GPIO_Pin = DEBUG_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // ��©���
    GPIO_Init(DEBUG_IIC_SDA_PORT, &GPIO_InitStructure);	
}

/**
 * @brief	��IIC ����ģʽ���ú���
 * @param	����
 * @doc		����
 */
static void IIC_Mode_Config(void)
{
    I2C_InitTypeDef  I2C_InitStructure;
    
    /* I2C ���� */
    I2C_InitStructure.I2C_Mode                  =   I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle             =   I2C_DutyCycle_2;//ռ�ձ�
    I2C_InitStructure.I2C_OwnAddress1           =   DEBUG_IIC_OwnAddress;//���õ�ַ
    I2C_InitStructure.I2C_Ack                   =   I2C_Ack_Enable;//����Ӧ��
    
    I2C_InitStructure.I2C_AcknowledgedAddress   =   I2C_AcknowledgedAddress_7bit;//Ѱַģʽ
    I2C_InitStructure.I2C_ClockSpeed            =   DEBUG_IIC_Speed;//ͨ������
    
    I2C_Init(DEBUG_IICx,&I2C_InitStructure);//��ʼ��
    I2C_Cmd(DEBUG_IICx,ENABLE);
    
}    

/**
 * @brief	��IIC ��ʱ������
 * @param	����
 * @doc		����
 */
static  uint8_t I2C_TIMEOUT_UserCallback(void)
{
  /* Block communication and all processes */
//  MPU_ERROR("I2C Timeout error!"); 
  
  return 0;
}

/**
 * @brief	��IIC �����ʼ��
 * @param	����
 * @doc		����
 */
void DEBUG_IIC_Init(void)
{
    IIC_GPIO_Config();
    IIC_Mode_Config();
}

/**
 * @brief	��IIC дһ���ֽں���
 * @param	��WriteAddr - �豸�ĵ�ַ
 * @param	��pBuffer   - 
 * @arg		�����������ѡ������о�
 * @note	��ע��
 * @retval	������ֵ˵��
 * @doc		����
 */
uint8_t DEBUG_IIC_WriteByte(uint8_t WriteAddr,uint8_t pBuffer)
{
    I2C_GenerateSTART(DEBUG_IICx,ENABLE);//
    I2CTimeout = I2CT_FLAG_TIMEOUT;//IIC��ʱʱ��
    
    while(!I2C_CheckEvent(DEBUG_IICx, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }
    
    I2C_Send7bitAddress(DEBUG_IICx, WriteAddr, I2C_Direction_Transmitter);
    I2CTimeout = I2CT_FLAG_TIMEOUT;//IIC��ʱʱ��
    
    while(!I2C_CheckEvent(DEBUG_IICx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
	{
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }
    
    I2C_SendData(DEBUG_IICx, WriteAddr);//���͵�ַ
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;//IIC��ʱʱ��
    while(!I2C_CheckEvent(DEBUG_IICx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
	{
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }
    
    I2C_SendData(DEBUG_IICx,pBuffer);
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;//IIC��ʱʱ��
    while(!I2C_CheckEvent(DEBUG_IICx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))	
	{
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }
    
    I2C_GenerateSTOP(DEBUG_IICx, ENABLE);
    return 1; //��������1
}

/**
  * @brief   ��I2C�豸�����ȡһ������ 
  * @param   
  *		@arg pBuffer:��Ŵ�slave��ȡ�����ݵĻ�����ָ��
  *		@arg SlaveAddress:���豸�ĵ�ַ
  *		@arg ReadAddr:�������ݵĴ��豸�ĵ�ַ
  *     @arg NumByteToWrite:Ҫ�Ӵ��豸��ȡ���ֽ���
  * @retval  ��������1���쳣����0
  */
uint8_t DEBUG_IIC_ReadBuffer(uint8_t *pBuffer,uint8_t SlaveAddress,uint8_t ReadAddr,uint16_t NumByteToRead)
{
    I2CTimeout = I2CT_FLAG_TIMEOUT;//IIC��ʱʱ��
    while(I2C_GetFlagStatus(DEBUG_IICx, I2C_FLAG_BUSY)) // Added by Najoua 27/08/2008    
    {
        if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }
    I2C_GenerateSTART(DEBUG_IICx, ENABLE);
    
    I2C_Send7bitAddress(DEBUG_IICx, SlaveAddress, I2C_Direction_Transmitter);
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;//IIC��ʱʱ��
    while(!I2C_CheckEvent(DEBUG_IICx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
	{
        if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }
    
    I2C_Cmd(DEBUG_IICx, ENABLE);
    
    I2C_SendData(DEBUG_IICx,ReadAddr);
    I2CTimeout = I2CT_FLAG_TIMEOUT;//IIC��ʱʱ��
    while(!I2C_CheckEvent(DEBUG_IICx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
	{
        if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }
    
    I2C_GenerateSTART(DEBUG_IICx,ENABLE);
    I2CTimeout = I2CT_FLAG_TIMEOUT;//IIC��ʱʱ��
    while(!I2C_CheckEvent(DEBUG_IICx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
	{
        if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }
    
    I2C_Send7bitAddress(DEBUG_IICx, SlaveAddress, I2C_Direction_Receiver);
    I2CTimeout = I2CT_FLAG_TIMEOUT;//IIC��ʱʱ��
    while(!I2C_CheckEvent(DEBUG_IICx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
	{
        if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }
    while(NumByteToRead)
    {
        if(NumByteToRead == 1)
        {
            I2C_AcknowledgeConfig(DEBUG_IICx, DISABLE);//�ر�Ӧ��
            I2C_GenerateSTOP(DEBUG_IICx,ENABLE);
        }
        if(I2C_CheckEvent(DEBUG_IICx, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            *pBuffer = I2C_ReceiveData(DEBUG_IICx);//��ȡ����
            pBuffer++;
            NumByteToRead--;
        }
    }
    I2C_AcknowledgeConfig(DEBUG_IICx,ENABLE);//Ӧ��
    return 1;
}

/**
  * @brief  �ȴ���������״̬ 
  * @param  ��
  * @retval ��
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

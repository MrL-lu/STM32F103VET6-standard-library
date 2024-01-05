/**
  ******************************************************************************
  * @file     ����������	  	main.c
  * @author ���������ߣ�		Lu
  * @version������汾��		V1.0.0.0
  * @date   ���޸����ڣ�		2023-12-28
  * @brief  �����ܼ�飩		STM32F103VET6�̼���ģ��
  * @parameter  ��������		��
  * @return   ������ֵ��		��
  ******************************************************************************
  * @attention ��ע�����
  * оƬ�ͺţ�STM32F103VET6
  * ʹ�����ţ�
  * �޸����ڣ� 
  * ʵ�ֹ��ܣ�
  *	
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

/* �û�ͷ�ļ� ------------------------------------------------------------------*/
#include "main.h"

/* ����ȫ�ֱ��� ------------------------------------------------------------------*/
/* ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����sram*/
extern __IO u16 ADC_ConvertedValue;
uint16_t chip_temp;

/* ����ʼ ----------------------------------------------------------------------*/
int main(void)
{
	// ��������main����֮ǰ�������ļ���statup_stm32f10x_hd.s�Ѿ�����
	// SystemInit()������ϵͳʱ�ӳ�ʼ����72MHZ
	// SystemInit()��system_stm32f10x.c�ж���
	// ����û����޸�ϵͳʱ�ӣ������б�д�����޸�
	
	/* �û���������	--------------------------------------------------------------------*/
    DHT11_Data_TypeDef DHT11_Data;
    uint32_t uniqueID[3];
    
	/* �û���ʼ����ʼ ------------------------------------------------------------------*/
	HSE_SetSysClock(RCC_PLLMul_9);  //����ϵͳʱ��Ϊ72MHz
	SysTick_Init();	//��ʼ��ϵͳ�δ�ʱ�� SysTick
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Usart_Config();//�������ú���
	BSP_LED_Config();//LED��ʼ������
    DHT11_Init();//DHT11 ��ʼ������
    Get_ChipID(uniqueID);//��ȡоƬΨһID
    Temp_ADC1_Init();
    Usart_SendString(DEBUG_USARTX,"�����ʼ�����\r\n");
    
    WS2812_Config(WS2812_Sendspeed);//WS2812 ���ú���
    
	/* ��ѭ������ʼ ------------------------------------------------------------------*/
	while(1)
	{
		LED_Red_TOGGLE;
		SysTick_Delay_ms(1000);	//��ʱ1s
        chip_temp = (1.43- ADC_ConvertedValue*3.3/4096)*1000 / 4.3+ 25;
        printf("\r\n The IC current tem= %3d ��\r\n", chip_temp);
        printf("\r\nоƬ��ΨһIDΪ: 0x%08X-%08X-%08X\r\n",uniqueID[0],uniqueID[1],uniqueID[2]);
        printf("\r\nоƬflash������Ϊ: %dK \r\n", *(__IO u16 *)(0X1FFFF7E0));
        if(DHT11_Read_TempAndHumidity(&DHT11_Data) == SUCCESS)
        {
            printf("\r\n��ȡDHT11�ɹ�!\r\n\r\nʪ��Ϊ%d.%d ��RH ���¶�Ϊ %d.%d�� \r\n",\
			DHT11_Data.DHT11_humi_int,DHT11_Data.DHT11_humi_deci,DHT11_Data.DHT11_temp_int,DHT11_Data.DHT11_temp_deci);
        }
        else
        {
            printf("Read DHT11 ERROR!\r\n");
        }
        WS2812_Display_Color(8,0xf11f23);
	}//end of while ��ѭ������
	
}//end of main
/* END OF FILE ------------------------------------------------------------------*/


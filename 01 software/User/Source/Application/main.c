/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : Main.c
* Author             : Fortiortech Appliction Team
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains main function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx_Sys.h>
#include <FU68xx.h>
#include <Myproject.h>


/* Private typedef ------------------------------------------------------------------------------*/
/* Private define -------------------------------------------------------------------------------*/
/* Private macro --------------------------------------------------------------------------------*/
/* Private variables ----------------------------------------------------------------------------*/
unsigned char code SysTable[] = {0x1a,0x40,0x00,0x00,0x00};

/* Private function prototypes ------------------------------------------------------------------*/
/* Private functions ----------------------------------------------------------------------------*/
void SoftwareInit(void);
void HardwareInit(void);
void MotorControlInit(void);
int16 foc_ud;
int16 foc_uq;
/*-------------------------------------------------------------------------------------------------
	Function Name :	void main(void)
	Description   :	��������Ҫ�����ǳ�ʼ��������ϵͳ��ʼ���������ʼ����Ӳ����ʼ������ѭ��
                  ɨ�衣
                  ϵͳ��ʼ��--ϵͳʱ�����ã�ϵͳ��Դ���ã����Ź�����
                  �����ʼ��--��ʼ�����ж���ı���
                  Ӳ����ʼ��--��ʼ��Ӳ���豸����
                  ��ѭ��ɨ��--�������״̬ɨ�衢�������ӹ���
	Input         :	��
	Output				:	��
-------------------------------------------------------------------------------------------------*/
void main(void)
{
 	uint32 PowerUpCnt = 0;	//�ϵ�ȴ���ʱ����
	
	/*System Init,ϵͳ��ʼ��*/
	SystemInit();

	/*Wait For Power Up*/
	for(PowerUpCnt=0;PowerUpCnt<SystemPowerUpTime;PowerUpCnt++){};

	/*Software Init�������ʼ��*/	
	SoftwareInit();
		
	/*Hardware Init��Ӳ����ʼ��*/	
	HardwareInit();
		
	motor_FR = CCW;			
	SPI_Init();
	OLED_Init();
		
	do
	{
		if (GP00 == 0)
		{
			motor_FR = !motor_FR;
		}
	} while(Time.START_Flag == 0);
		
	while(1)
	{
		if(mcCurOffset.OffsetFlag==0)
		{
			/****�ϵ��Ƚ���Ӳ��������׼�ɼ����ٽ������غ���ɨ��***/
			 GetCurrentOffset();
		}
		else
		{
			/****���غ�����״̬ɨ��****/
			MC_Control();
			foc_ud=FOC_UD;
			foc_uq=FOC_UQ;
			#if (SpeedMode==Usart_Speed)	                          //ѡ�񴮿���Ϊ���Է�ʽ
			{
				UartDealComm();	
			}
			#endif	
		}
	}
}


/*-------------------------------------------------------------------------------------------------
	Function Name :	void SoftwareInit(void)
	Description   :	�����ʼ������ʼ�����ж������
	Input         :	��
	Output				:	��
-------------------------------------------------------------------------------------------------*/
void SoftwareInit(void)
{
 /****��ʼ�����ж���Ĳ�������****/
	MotorcontrolInit();
 /****�����ʼ״̬Ϊmcwait******/
	mcState = mcwait;
	mcFaultSource = 0;  
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void HardwareInit(void)
	Description   :	Ӳ����ʼ������ʼ����Ҫʹ�õ�Ӳ���豸���ã�FOC�������õ����˷ŵ�ѹ���˷ų�ʼ����ADC��ʼ����Driver��ʼ��
                  TIM4��ʼ���������Ŀɸ���ʵ������ӡ�
	Input         :	��
	Output				:	��
-------------------------------------------------------------------------------------------------*/
void HardwareInit(void)
{
	/****����IO��ʼ��***********/
	GPIO_Init();	
	/******Ӳ��FO�������Ƚ�����ʼ��������Ӳ�������Ƚϱ���������Ӳ������ѡ������ģʽ******/
	#if (HardwareCurrent_Protect==Hardware_FO_Protect)	//�ⲿ�жϳ�ʼ���������ⲿ�ж�Ӳ������FO�ı���
	{
		EXTI_Init();		
	}
	#elif (HardwareCurrent_Protect==Hardware_CMP_Protect)//ѡ��ȽϹ������Ƚ�����ʼ��
	{
		CMP_Iint();	
	}
	#elif (HardwareCurrent_Protect==Hardware_FO_CMP_Protect)	//���߶�ѡ��
	{
		EXTI_Init();	
		CMP_Iint();			
	}
	#endif
	
	/*****����Ŵ�����ʼ��*********/
	AMP_VHALF_Init();																				// �˷ŵ�ѹƫ��ѡ��
	
	AMP_Iint();                                             // �˷Źܽų�ʼ��
	
	/*****ADC��ʼ��*********/
 	ADC_Init();	
	
	/*****MDU��ʼ��*********/
	MDU_16MUL16_INT(15,1);		                             //����15λ ���з��ų˷�                 
	
	/*****Driver��ʼ��******/
	Driver_Init();
	
	/*****UART��ʼ��*********/	
	#if (SpeedMode==Usart_Speed)	                          //ѡ�񴮿���Ϊ���Է�ʽ
	{
		UART_Init();	
	}
	#endif

/*****Timer��ʼ��*******/
	 TIM3_Init();            //������ʱ��3�ж���ΪPWM�����ж�
	 TIM4_Init();              //���ö�ʱ��4�ж���Ϊ�����ж�,ʱ��Ϊ1ms��������ϱ����ȸ��ӹ���*
//	 TIM5_Init();            //����
}

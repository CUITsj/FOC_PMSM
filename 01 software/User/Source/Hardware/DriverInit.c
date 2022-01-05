/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : DriverInit.c
* Author             : Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains driver initial function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>


/*-------------------------------------------------------------------------------------------------
	Function Name :	void Driver_Init(void)
	Description   :	Driver��ʼ������
	Input         :	��
  Output				:	��
-------------------------------------------------------------------------------------------------*/
void Driver_Init(void)
{
	uint32 PRDRVCnt;
	
/*-------------------------------------------------------------------------------------------------
	DRV_CTL��PWM��Դѡ��
	OCS1 = 0, OCS1 = X: TIM0 PWM
	OCS1 = 1, OCS1 = 0: TIM1 PWM
	OCS1 = 1, OCS1 = 1: FOC 6 PWM
-------------------------------------------------------------------------------------------------*/
	/*FOC 6 PWM output*/
	SetBit(DRV_CTL, OCS1, 1);			                        	// FOC 6 PWM output
	SetBit(DRV_CTL, OCS0, 1);			                        	// Predriverʹ��λ
	
  /*Predriver & Gatedriverģʽѡ������*/	
	#if (DriverMode == Driver_3P3N)
	{
		SetBit(DRV_CTL, PRDRVEN, 1);			                   	// Predriverʹ��λ
		for(PRDRVCnt=0;PRDRVCnt<24000;PRDRVCnt++);
	}
	#elif (DriverMode == Driver_6N)
	{
		SetBit(DRV_CTL, PRDRVEN, 0);			                    // ��ֹpredrive driverʹ��λ		
		#if (PWM_Level_Mode == High_Level)										
		{ 
	    DRV_OUT=0x00;			                                  // �ر�MOE�����ÿ��е�ƽΪ�͵�ƽ
		}
	 #elif (PWM_Level_Mode == Low_Level)										
		{
			DRV_OUT=0x3F;                                       // �ر�MOE�����ÿ��е�ƽΪ�ߵ�ƽ
		}
	 #endif
	}
	#endif	
	SetBit(DRV_CTL, DRVOE, 1);															// gata driverʹ��λ
}



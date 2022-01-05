/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : TimerInit.c
* Author             : Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains Timer Initial function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>

/* Private variables ----------------------------------------------------------------------------*/
PWMINCtl_TypeDef xdata PWMINCtl;


/*-------------------------------------------------------------------------------------------------
	Function Name :	void TIM0_Init(void)
	Description   :	TIM0��ʼ������
	Input         :	��
  Output				:	��
-------------------------------------------------------------------------------------------------*/
void TIM0_Init(void)
{
	SetBit(TIM0_CR, CEN, 0);					                       // ֹͣ������	
	
/*-------------------------------------------------------------------------------------------------              
	ʱ����Ԫ
	CMS = 00�����ض������ģʽ
	CMS = 01������������ģʽ1���Ƚ�����жϱ�־λ�����¼���ʱ��λ
	CMS = 10������������ģʽ2���Ƚ�����жϱ�־λ�����ϼ���ʱ��λ
	CMS = 11������������ģʽ3���Ƚ�����жϱ�־λ�����Ϻ����¼���ʱ��λ
-------------------------------------------------------------------------------------------------*/	
  /* ����ģʽ */
	SetBit(TIM0_CR, DIR, 0);					                       // 0:���ϼ�����1:���¼���
	SetBit(TIM0_CR, CMS1, 1);
	SetBit(TIM0_CR, CMS0, 1);				                         // 00:���ض������ģʽ��other:����������ģʽ
		
	/* ARR��ת��ʹ�� */
	SetBit(TIM0_CR, ARPE, 1);    		                         // 0:��ֹARRԤװ�أ�1:ʹ��ARRԤװ��

	/* OPMģʽ */
	SetBit(TIM0_CR, OPM, 0);        	                       // 0:��ֹOPMģʽ��1:ʹ��ARRԤװ��
	
	/* RCR��PSCR��ARR��CNTR��ֵ��ʼ�� */
	TIM0_RCR = 1;	
	TIM0_PSCR = 0;								                           // Fck_cnt=Fck_psc/(TIM0_PSCR+1)
	TIM0_ARR = PWM_VALUE_LOAD;					
	TIM0_CNTR = 1;					
	
/*-------------------------------------------------------------------------------------------------
	�����Ԫ
	OCiM = 000������OCiREF��OCiREF����ԭ��״̬����
	OCiM = 001��ƥ��ʱOCiREF�����Ч��ƽ
	OCiM = 010��ƥ��ʱOCiREF�����Ч��ƽ
	OCiM = 011��ƥ��ʱOCiREF�����ת��ƽ
	OCiM = 100��OCiREFǿ������͵�ƽ,OCiǿ�������Ч��ƽ
	OCiM = 101��OCiREFǿ������ߵ�ƽ,OCiǿ�������Ч��ƽ
	OCiM = 110��PWMģʽ1��CNTR<CCRi�����Ч��ƽ��CNTR>CCRi�����Ч��ƽ
	OCiM = 111��PWMģʽ2��CNTR<CCRi�����Ч��ƽ��CNTR>CCRi�����Ч��ƽ
-------------------------------------------------------------------------------------------------*/	
	/* �ȽϿ���λCCiE/CCiNE/CCiP/CCiNP/OCiMԤװ��ʹ�� */
	SetBit(TIM0_CCER2, CCPC, 0);     	                       // 0:�ȽϿ���λԤװ�ؽ�ֹ��1:�ȽϿ���λԤװ��ʹ��

  /* OC1ͨ������ */
	SetBit(TIM0_CCMR1, OC1M2, 1);
	SetBit(TIM0_CCMR1, OC1M1, 1);		
	SetBit(TIM0_CCMR1, OC1M0, 1);		                         // OC1�Ƚ����ģʽ	
	SetBit(TIM0_CCMR1, OC1PE, 0);                            // OC1�Ƚ�����Ĵ���Ԥװ��ʹ��
	#if (DriverMode == Driver_3P3N)
	{
		SetBit(TIM0_CCER1, CC1P, 1);                           // OC1������ԣ�0������Ч��1������Ч
	}
	#elif (DriverMode == Driver_6N)
	{
		SetBit(TIM0_CCER1, CC1P, 0);                           // OC1������ԣ�0������Ч��1������Ч
	}
	#endif
	SetBit(TIM0_CCER1, CC1E, 1);                             // OC1���ʹ��
	SetBit(TIM0_CCER1, CC1NP, 0);                            // OC1N������ԣ�0������Ч��1������Ч
	SetBit(TIM0_CCER1, CC1NE, 1);                            // OC1N���ʹ��

	/* OC2ͨ������ */
	SetBit(TIM0_CCMR1, OC2M2, 1);
	SetBit(TIM0_CCMR1, OC2M1, 1);
	SetBit(TIM0_CCMR1, OC2M0, 1);		                         // OC2�Ƚ����ģʽ	
	SetBit(TIM0_CCMR1, OC2PE, 0);		                         // OC2�Ƚ�����Ĵ���Ԥװ��ʹ��
	#if (DriverMode == Driver_3P3N)
	{
		SetBit(TIM0_CCER1, CC2P, 1);                           // OC2������ԣ�0������Ч��1������Ч
	}
	#elif (DriverMode == Driver_6N)
	{
		SetBit(TIM0_CCER1, CC2P, 0);                           // OC2������ԣ�0������Ч��1������Ч
	}	
	#endif
	SetBit(TIM0_CCER1, CC2E, 1);                             // OC2���ʹ��
	SetBit(TIM0_CCER1, CC2NP, 0);                            // OC2N������ԣ�0������Ч��1������Ч
	SetBit(TIM0_CCER1, CC2NE, 1);                            // OC2N���ʹ��

	/* OC3ͨ������ */
	SetBit(TIM0_CCMR2, OC3M2, 1);
	SetBit(TIM0_CCMR2, OC3M1, 1);
	SetBit(TIM0_CCMR2, OC3M0, 1);		                         // OC3�Ƚ����ģʽ	
	SetBit(TIM0_CCMR2, OC3PE, 0);		                         // OC3�Ƚ�����Ĵ���Ԥװ��ʹ��
	#if (DriverMode == Driver_3P3N)
	{
		SetBit(TIM0_CCER2, CC3P, 1);                           // OC3������ԣ�0������Ч��1������Ч
	}
	#elif (DriverMode == Driver_6N)
	{
		SetBit(TIM0_CCER2, CC3P, 0);                           // OC3������ԣ�0������Ч��1������Ч
	}
	#endif	
	SetBit(TIM0_CCER2, CC3E, 1);                             // OC3���ʹ��
	SetBit(TIM0_CCER2, CC3NP, 0);                            // OC3N������ԣ�0������Ч��1������Ч
	SetBit(TIM0_CCER2, CC3NE, 1);                            // OC3N���ʹ��
		
	/* OC4ͨ������ */
	SetBit(TIM0_CCMR2, OC4M2, 1);
	SetBit(TIM0_CCMR2, OC4M1, 1);
	SetBit(TIM0_CCMR2, OC4M0, 1);		                         // OC4�Ƚ����ģʽ	
	SetBit(TIM0_CCMR2, OC4PE, 0);		                         // OC4�Ƚ�����Ĵ���Ԥװ��ʹ��
	SetBit(TIM0_CCER2, CC4P, 0);                             // OC4������ԣ�0������Ч��1������Ч
	SetBit(TIM0_CCER2, CC4E, 1);                             // OC4���ʹ��
	

	/* ����ʱ������ */
	TIM0_DTR = 48;							                             // count 24, time = 24/24MHZ = 1us

	/* CCRi��ֵ��ʼ�� */				
	TIM0_CCR4 = PWM_VALUE_LOAD/2;								
	TIM0_CCR3 = PWM_VALUE_LOAD;			
	TIM0_CCR2 = PWM_VALUE_LOAD;		
	TIM0_CCR1 = PWM_VALUE_LOAD;		
	
	/* ���IOʹ�� */
	SetBit(DRV_CTL, OCS1, 1);					
	SetBit(DRV_CTL, OCS0, 0);						                       // OCi��OCiNѡ��TIM0���		
	MOE = 0;
	OIS4 = 0;										                             // OC4���е�ƽ����
	#if (DriverMode == Driver_3P3N)
	{
		OISWL = 0;											                       // OC3N���е�ƽ����
		OISWH = 1;											                       // OC3���е�ƽ����
		OISVL = 0;											                       // OC2N���е�ƽ����
		OISVH = 1;											                       // OC2���е�ƽ����
		OISUL = 0;											                       // OC1N���е�ƽ����
		OISUH = 1;											                       // OC1���е�ƽ����		
	}
	#elif (DriverMode == Driver_6N)
	{
		OISWL = 0;											                       // OC3N���е�ƽ����
		OISWH = 0;											                       // OC3���е�ƽ����
		OISVL = 0;											                       // OC2N���е�ƽ����
		OISVH = 0;											                       // OC2���е�ƽ����
		OISUL = 0;											                       // OC1N���е�ƽ����
		OISUH = 0;											                       // OC1���е�ƽ����		
	}		
	#endif
	
/*-------------------------------------------------------------------------------------------------
	�¼�/�ж���Ӧ��Ԫ
	UEV�����¼�������PSCR��ARR��CCRi
	COM�����¼�������CCiE/CCiNE��CCiP/CCiNP��OCiM
-------------------------------------------------------------------------------------------------*/	
	/* UEV����ʹ��UEV��������Դ���� */
	SetBit(TIM0_CR, URS, 0);                                 // 0���������������λUG��1�����������
	SetBit(TIM0_CR, UDIS, 0);                                // 0������UEV�����¼���1����ֹUEV�����¼�
	
	/*�ж�״̬��־����*/
	CLR(TIM0_SR1, COMIF);    			                           // COM�жϱ�־����
	CLR(TIM0_SR1, CC4IF); 	 			                           // �Ƚ�ͨ��4�жϱ�־����
	CLR(TIM0_SR1, CC3IF);    			                           // �Ƚ�ͨ��3�жϱ�־����
	CLR(TIM0_SR1, CC2IF); 	 			                           // �Ƚ�ͨ��2�жϱ�־����
	CLR(TIM0_SR1, CC1IF); 				                           // �Ƚ�ͨ��1�жϱ�־����
	CLR(TIM0_SR1, UIF);      			                           // �����¼��жϱ�־����
	
	/*�ж�ʹ��*/
	SetBit(TIM0_IER, COMIE, 0);                              // COM�ж�ʹ��
	SetBit(TIM0_IER, CC4IE, 1); 	                           // �Ƚ�ͨ��4�ж�ʹ��
	SetBit(TIM0_IER, CC3IE, 0);                              // �Ƚ�ͨ��3�ж�ʹ��
	SetBit(TIM0_IER, CC2IE, 0); 	                           // �Ƚ�ͨ��2�ж�ʹ��
	SetBit(TIM0_IER, CC1IE, 0); 	                           // �Ƚ�ͨ��1�ж�ʹ��
	SetBit(TIM0_IER, UIE, 1);                                // �����¼��ж�ʹ��
	PTIM01 = 1;
	PTIM00 = 1;
	EA = 1;
	
	/*�¼�����*/
	SetBit(TIM0_EGR, COMG, 0);                               // COM�¼�
	SetBit(TIM0_EGR, CC4G, 0); 	 	                           // �Ƚ�ͨ��4�Ƚ��¼�
	SetBit(TIM0_EGR, CC3G, 0);                               // �Ƚ�ͨ��3�Ƚ��¼�
	SetBit(TIM0_EGR, CC2G, 0); 	                             // �Ƚ�ͨ��2�Ƚ��¼�
	SetBit(TIM0_EGR, CC1G, 0); 	                             // �Ƚ�ͨ��1�Ƚ��¼�
	SetBit(TIM0_EGR, UG, 0);                                 // �����¼�
	
	SetBit(TIM0_CR, CEN, 1);					                       // ����������	

}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void TIM1_Init(void)
	Description   :	TIM1��ʼ������
	Input         :	��
  Output				:	��
-------------------------------------------------------------------------------------------------*/
void TIM1_Init(void)
{
/*-------------------------------------------------------------------------------------------------
	�����˲�ʹ�ܼ�ģʽѡ��	
	TIM1_CR2 = 0x00;		4-machine cycle noise,3MHz
	TIM1_CR2 = 0x15;		8-machine cycle noise,1.5MHz
	TIM1_CR2 = 0x2a;		16-machine cycle noise,0.75MHz	
	TIM1_CR2 = 0x3f;		32-machine cycle noise,0.375MHz		
-------------------------------------------------------------------------------------------------*/
	TIM1_CR2 = 0x3f;
	SetBit(TIM1_CR0, INC2, 1);		                           // 0:��ֹTI2�����˲�,1:ʹ��TI2�����˲�
	SetBit(TIM1_CR0, INC1, 1);		                           // 0:��ֹTI1�����˲�,1:ʹ��TI1�����˲�
	SetBit(TIM1_CR0, INC0, 1);		                           // 0:��ֹTI0�����˲�,1:ʹ��TI0�����˲�
	
/*-------------------------------------------------------------------------------------------------
	�����ź�ѡ��
	0: TI0--COMP0_OUT    TI1--COMP1_OUT    TI2--COMP2_OUT
	1: TI0--P1.4_IN      TI1--P1.6_IN      TI2--P2.1_IN
-------------------------------------------------------------------------------------------------*/
	SetBit(TIM1_CR2, TIS, 1);		
	
/*-------------------------------------------------------------------------------------------------
	���뼫�Լ��
	00:��������	  01:�����ؼ��	  10:�����ؼ��   11:��/�����ؼ��	
-------------------------------------------------------------------------------------------------*/
	SetBit(TIM1_CR0, CPE1, 1);	
	SetBit(TIM1_CR0, CPE0, 1);			
	
/*���ؼ��ʹ��-----------------------------------------------------------------------------------*/
	SetBit(TIM1_CR1, CMPE, 0);			
	SetBit(TIM1_CR0, IEE2, 0);			                         // 0:��ֹTI2���ؼ��,1:ʹ��TI2���ؼ��
	SetBit(TIM1_CR0, IEE1, 0);			                         // 0:��ֹTI1���ؼ��,1:ʹ��TI1���ؼ��
	SetBit(TIM1_CR0, IEE0, 1);			                         // 0:��ֹTI0���ؼ��,1:ʹ��TI0���ؼ��
	SetBit(TIM1_CR1, CMPE, 0);		                           // 0:���ؼ��Ƚ�,   1:λ�ü��Ƚ�

/*CPDֵ����--------------------------------------------------------------------------------------*/
	SetBit(TIM1_CR1, CPD2, 0);
	SetBit(TIM1_CR1, CPD2, 0);
	SetBit(TIM1_CR1, CPD2, 0);				
	
/*RDAֵ����--------------------------------------------------------------------------------------*/
	SetBit(TIM1_DRH, RDA2, 0);
	SetBit(TIM1_DRH, RDA1, 0);
	SetBit(TIM1_DRH, RDA0, 0);				
	
/*-------------------------------------------------------------------------------------------------
	���ݸ���ģʽ����
	000:���дTIM1_BDBR����TIM1_BDR
	001:PTimer���紥��TIM1_BDBR����TIM1_BDR
	010:λ�ü�����봥��TIM1_BDBR����TIM1_BDR
	011:λ�ü����������PTimer��PTimer�������TIM1_BDR
	100:λ�ü���������PTimer�������TIM1_BDR
	101:λ�ü��Ƚϵ�����Ч�����PTimer�����������TIM1_BDR
	110:λ�ü��Ƚϵ�����Ч���봥��TIM1_BDBR����TIM1_BDR
	111:λ�ü��Ƚϵ�����Ч��������PTimer��PTimer�������TIM1_DBR										
-------------------------------------------------------------------------------------------------*/
	SetBit(TIM1_CR3, OPS2, 0);
	SetBit(TIM1_CR3, OPS1, 0);
	SetBit(TIM1_CR3, OPS0, 0);
	
/*Basic Timer & Preload Timer����----------------------------------------------------------------*/
	SetBit(TIM1_CR3, BCEN, 0);			                         // ��ֹ��ʱ��
	SetBit(TIM1_CR3, T1PSC3, 0);
	SetBit(TIM1_CR3, T1PSC2, 1);
	SetBit(TIM1_CR3, T1PSC1, 0);
	SetBit(TIM1_CR3, T1PSC0, 1);		                         // ��ʱ��Ԥ��Ƶ: 32
	SetBit(TIM1_CR2, BRS, 1);	                               // 0:д��ʱ���źŸ�λ��ʱ��,1:λ�ü���źŸ�λ��ʱ��
	TIM1_BARR = 60000;                                       // 80000us
	TIM1_BCNTR = 1;	
	SetBit(TIM1_CR3, BCEN, 1);
	
	SetBit(TIM1_CR1, RCEN, 0);
	SetBit(TIM1_CR1, ROPM, 1);			                         // 0:��ֹOPMģʽ,1:ʹ��OPMģʽ
	TIM1_RARR = 65535;
	TIM1_RCNTR = 65534;	
// 	SetBit(TIM1_CR1, RCEN, 1);
	
/*�ж�ʹ��---------------------------------------------------------------------------------------*/
	TIM1_SR &= 0x0f;
	SetBit(TIM1_IER, BOIE, 1);			                         // TIM1������ʱ������ж�	
	SetBit(TIM1_IER, RUIE, 1);			                         // TIM1���ض�ʱ�������ж�
	SetBit(TIM1_IER, WTIE, 0);			                         // д��ʱ���ж�
	SetBit(TIM1_IER, PDIE, 1);			                         // λ�ü���ж�
	SetBit(TIM1_IER, CPIE, 0);			                         // �Ƚ��ж�
	EA = 1;												                           // ʹ��ȫ���ж�	
	PTIM11 = 1;
	PTIM10 = 0;										                           // TIM1�ж����ȼ���Ϊ0


/*��������---------------------------------------------------------------------------------------*/
	TIM1_DTR = 48;                                           // DeadTime = (TIM1_DTR+1)/cpuclk, 

/*-------------------------------------------------------------------------------------------------
	PPGͬ����ʽ����
	WTS = 00: ��ͬ���������������   
	WTS = 01: ������ͬ�����   
	WTS = 10: �½���ͬ�����  
	WTS = 11: ����/�½���ͬ�����
-------------------------------------------------------------------------------------------------*/
    #if (DriverMode == Driver_3P3N)
  	{
  		SetBit(TIM1_CR1, WTS1, 1);
  		SetBit(TIM1_CR1, WTS0, 0);
  	}
  	#elif (DriverMode == Driver_6N)
  	{
  		SetBit(TIM1_CR1, WTS1, 0);
  		SetBit(TIM1_CR1, WTS0, 1);		
  	}	
  	#endif
	
/*-------------------------------------------------------------------------------------------------
	���ģʽѡ��ʹ��
	OCM = 00��ǿ������͵�ƽ
	OCM = 01�����ͬ��PPG��TIM2�����
	OCM = 10���������PPG��TIM2�����
	OCM = 11��ǿ������ߵ�ƽ
-------------------------------------------------------------------------------------------------*/
	#if (DriverMode == Driver_3P3N)
	{  
    TIM1_DBR = 0x0333;                                     // UH/VH/WH = 1; UL/VL/WL = 0
	} 
	#elif (DriverMode == Driver_6N)
	{
    TIM1_DBR = 0x0000;                                     // UH/VH/WH = 0; UL/VL/WL = 0
	}		
	#endif
	
	SetBit(DRV_CTL, OCS1, 0);					
	SetBit(DRV_CTL, OCS0, 0);						                       // OCi��OCiNѡ��BLDC���	
//	MOE = 1;	
	OIS4 = 0;										                             // OC4���е�ƽ����
	#if (DriverMode == Driver_3P3N)
	{
		OISWL = 0;											                       // OC5���е�ƽ����
		OISWH = 1;											                       // OC4���е�ƽ����
		OISVL = 0;											                       // OC3���е�ƽ����
		OISVH = 1;											                       // OC2���е�ƽ����
		OISUL = 0;											                       // OC1���е�ƽ����
		OISUH = 1;											                       // OC0���е�ƽ����		
	}
	#elif (DriverMode == Driver_6N)
	{
		OISWL = 0;											                       // OC5���е�ƽ����
		OISWH = 0;											                       // OC4���е�ƽ����
		OISVL = 0;											                       // OC3���е�ƽ����
		OISVH = 0;											                       // OC2���е�ƽ����
		OISUL = 0;											                       // OC1���е�ƽ����
		OISUH = 0;											                       // OC0���е�ƽ����		
	}	
	#endif
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void TIM2_Init(void)
	Description   :	TIM2��ʼ������
	Input         :	��
	Output				:	��
-------------------------------------------------------------------------------------------------*/
//void TIM2_Init(void)
//{
////	SetBit(PH_SEL,T2SEL,1);					                       // GP10ѡ��Capture Timer����/���(for test)
//	SetBit(TIM2_CR1, T2CEN, 0);			                         // ֹͣ����*/
//	
///*-------------------------------------------------------------------------------------------------
//	ʱ�ӷ�Ƶ����(T4PSC)
//  000:cpuclk			001:cpuclk/2^1	010:cpuclk/2^2	011:cpuclk/2^3
//  100:cpuclk/2^4	101:cpuclk/2^5	110:cpuclk/2^6	111:cpuclk/2^7	
//-------------------------------------------------------------------------------------------------*/
//	SetBit(TIM2_CR0, T2PSC2, 0);
//	SetBit(TIM2_CR0, T2PSC1, 0);
//	SetBit(TIM2_CR0, T2PSC0, 0);

///*-------------------------------------------------------------------------------------------------
//	�Ƚ����ģʽ
//	OCM = 00��ǿ������͵�ƽ
//	OCM = 01��ǿ������ߵ�ƽ
//	OCM = 10��PWMģʽ1��CNTR<DR����͵�ƽ��CNTR>DR����ߵ�ƽ
//	OCM = 11��PWMģʽ2��CNTR<DR����ߵ�ƽ��CNTR>DR����͵�ƽ
//-------------------------------------------------------------------------------------------------*/
//  SetBit(TIM2_CR0, T2MODE, 1);			                       // ѡ�����ģʽ				
//	SetBit(TIM2_CR0, T2OCM1, 1);
//	SetBit(TIM2_CR0, T2OCM0, 0);			                       // ����Ƚ�ģʽ2,1-->0
// 	
//	CLRFlag_T2IR;
//	CLRFlag_T2IP;
//	CLRFlag_T2IF;
//	SetBit(TIM2_CR1, T2IDE, 0);				                       // ��ֹ����Ƚ�/�½��ؼ���жϺ������ؼ���ж�									
//	SetBit(TIM2_CR1, T2IFE, 0);				                       // ʹ������ж�	
//	PTIM231 = 0;
//	PTIM230 = 1;										                         // TIM2/3�ж����ȼ���Ϊ3
//	EA = 1;

//	TIM2_ARR = PWM_VALUE_LOAD;								                       // PPG PWMfreq = 24MHz/1500 = 16KHz				
//	TIM2_CNTR = 1;	
//	TIM2_DR = PWM_VALUE_LOAD >> 1;	
//	TIM2_CMTR = 0;
//	TIM2_ADTR = PWM_VALUE_LOAD >> 1;
//	
//	SetBit(TIM2_CR1,T2CEN,1);				                         // ��������		
//}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void TIM3_Init(void)
	Description   :	TIM3��ʼ������
	Input         :	��
  Output				:	��
-------------------------------------------------------------------------------------------------*/
void TIM3_Init(void)
{
	SetBit(PH_SEL, T3SEL, 1);					                       // GP11ѡ��Capture Timer����/���(for test)
	SetBit(TIM3_CR1, T3CEN, 0);			                         // ֹͣ����*/

/*-------------------------------------------------------------------------------------------------
	ʱ�ӷ�Ƶ����(T5PSC)
	000:cpuclk			001:cpuclk/2^1	010:cpuclk/2^2	011:cpuclk/2^3
	100:cpuclk/2^4	101:cpuclk/2^5	110:cpuclk/2^6	111:cpuclk/2^7	
-------------------------------------------------------------------------------------------------*/
	SetBit(TIM3_CR0, T3PSC2, 0);
	SetBit(TIM3_CR0, T3PSC1, 0);
	SetBit(TIM3_CR0, T3PSC0, 0);

/*-------------------------------------------------------------------------------------------------
	�Ƚ����ģʽ
	OCM = 00��ǿ������͵�ƽ
	OCM = 01��ǿ������ߵ�ƽ
	OCM = 10��PWMģʽ1��CNTR<DR����͵�ƽ��CNTR>DR����ߵ�ƽ
	OCM = 11��PWMģʽ2��CNTR<DR����ߵ�ƽ��CNTR>DR����͵�ƽ
-------------------------------------------------------------------------------------------------*/
	SetBit(TIM3_CR0, T3MODE, 0);			                       // ѡ������ģʽ				
//	SetBit(TIM3_CR0, T3OCM1, 1);
//	SetBit(TIM3_CR0, T3OCM0, 0);			                       // ����Ƚ�ģʽ2,0-->1
	SetBit(TIM3_CR0, T3CTM, 0);	// ����Timerģʽ
	
	
	CLRFlag_T3IR;
	CLRFlag_T3IP;
	CLRFlag_T3IF;
//	SetBit(TIM3_CR1, T3FE, 0);
	SetBit(TIM3_CR1, T3IDE, 0);				                       // ��ֹ����Ƚ�/�½��ؼ���жϺ������ؼ���ж�									
	SetBit(TIM3_CR1, T3IFE, 1);				                       // ʹ������ж�	
	PTIM231 = 1;
	PTIM230 = 0;										                         // TIM2/3�ж����ȼ���Ϊ1
	EA = 1;
	
	TIM3_ARR = 0;							                               // TIM3 Period = 100us				
	TIM3_DR = 0;	
	TIM3_CNTR = 1;
	
	SetBit(TIM3_CR1, T3CEN, 1);				                       // ��������	
	
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void TIM4_Init(void)
	Description   :	TIM4��ʼ������
	Input         :	��
  Output				:	��
-------------------------------------------------------------------------------------------------*/
void TIM4_Init(void)
{
//	SetBit(PH_SEL, T4SEL, 1);					                     	 // GP01ѡ��Capture Timer����/���(for test)
	SetBit(TIM4_CR1, T4CEN, 0);			                           // 0��ֹͣ������1,ʹ�ܼ���

/*-------------------------------------------------------------------------------------------------
	ʱ�ӷ�Ƶ����(T5PSC)
	000:cpuclk			001:cpuclk/2^1	010:cpuclk/2^2	011:cpuclk/2^3
	100:cpuclk/2^4	101:cpuclk/2^5	110:cpuclk/2^6	111:cpuclk/2^7	
-------------------------------------------------------------------------------------------------*/
	SetBit(TIM4_CR0, T4PSC2, 0);
	SetBit(TIM4_CR0, T4PSC1, 0);
	SetBit(TIM4_CR0, T4PSC0, 0);

/*-------------------------------------------------------------------------------------------------
	�Ƚ����ģʽ
	OCM = 00��ǿ������͵�ƽ
	OCM = 01��ǿ������ߵ�ƽ
	OCM = 10��PWMģʽ1��CNTR<DR����͵�ƽ��CNTR>DR����ߵ�ƽ
	OCM = 11��PWMģʽ2��CNTR<DR����ߵ�ƽ��CNTR>DR����͵�ƽ
-------------------------------------------------------------------------------------------------*/
	SetBit(TIM4_CR0, T4MODE, 1);			                       // 1��ѡ�����ģʽ	;0��ѡ������ģʽ			
	SetBit(TIM4_CR0, T4OCM1, 1);
	SetBit(TIM4_CR0, T4OCM0, 0);			                       // ����Ƚ�ģʽ2,0-->1

	CLRFlag_T4IR;
	CLRFlag_T4IP;
	CLRFlag_T4IF;
	SetBit(TIM4_CR1, T4IDE, 0);				                       // ��ֹ����Ƚ�/�½��ؼ���жϺ������ؼ���ж�									
	SetBit(TIM4_CR1, T4IFE, 1);				                       // ʹ������жϣ������ж�
	PTIM451 = 0;
	PTIM450 = 0;										                         // TIM4/5�ж����ȼ���Ϊ1
	EA = 1;
	
	TIM4_ARR = 24000;							                           // TIM4 Period = 1ms				
	TIM4_DR = TIM4_ARR >> 1;	
	TIM4_CNTR = 1;
	
	SetBit(TIM4_CR1, T4CEN, 1);				                       //��������	
	
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void TIM5_Init(void)
	Description   :	TIM5��ʼ������
	Input         :	��
  Output				:	��
-------------------------------------------------------------------------------------------------*/

void TIM5_Init(void)
{
//	SetBit(PH_SEL, T5SEL, 1);				                       // ѡ��Capture Timer����/���(for test)
	SetBit(TIM5_CR1, T5CEN, 0);			                         // ֹͣ����
	
/*-------------------------------------------------------------------------------------------------
	ʱ�ӷ�Ƶ����(T5PSC)
  000:cpuclk			001:cpuclk/2^1	010:cpuclk/2^2	011:cpuclk/2^3
  100:cpuclk/2^4	101:cpuclk/2^5	110:cpuclk/2^6	111:cpuclk/2^7	
-------------------------------------------------------------------------------------------------*/
	SetBit(TIM5_CR0, T5PSC2, 1);
	SetBit(TIM5_CR0, T5PSC1, 1);
	SetBit(TIM5_CR0, T5PSC0, 1);				// 187.5K

/*-------------------------------------------------------------------------------------------------
	�Ƚ����ģʽ
	OCM = 00��ǿ������͵�ƽ
	OCM = 01��ǿ������ߵ�ƽ
	OCM = 10��PWMģʽ1��CNTR<DR����͵�ƽ��CNTR>DR����ߵ�ƽ
	OCM = 11��PWMģʽ2��CNTR<DR����ߵ�ƽ��CNTR>DR����͵�ƽ
-------------------------------------------------------------------------------------------------*/
	SetBit(TIM5_CR0, T5MODE, 1);			                       // ѡ�����ģʽ				
	SetBit(TIM5_CR0, T5OCM1, 1);
	SetBit(TIM5_CR0, T5OCM0, 0);			                       // PWMģʽ1
  
	CLRFlag_T5IR;
	CLRFlag_T5IP;
	CLRFlag_T5IF;
	SetBit(TIM5_CR1, T5IDE, 0);				                       // ��ֹ����Ƚ�/�½��ؼ���жϺ������ؼ���ж�									
	SetBit(TIM5_CR1, T5IFE, 0);				                       // ʹ������ж�	
	PTIM451 = 0;
	PTIM450 = 0;										                         // TIM4/5�ж����ȼ���Ϊ1
	EA = 1;
	
	TIM5_ARR = 60000;								                         // TIM5 Period = 80000us				
	TIM5_DR = TIM5_ARR >> 1;	
	TIM5_CNTR = 1;
	
	SetBit(TIM5_CR1, T5CEN, 1);				                       // ��������	

}




/*-------------------------------------------------------------------------------------------------
	Function Name :	void TIM5_Timing_us(uint16 DR, uint16 ARR)
	Description   :	��ʱ����������ʱ��750KHz��������ʱͨ��
	Input         :	DR--��ʱͨ��1���Ƚ�ƥ���жϲ�����ʱ
                  ARR--��ʱͨ��2������жϲ�����ʱ
  Output				:	��
-------------------------------------------------------------------------------------------------*/
void TIM5_Timing_us(uint16 DR, uint16 ARR)
{
	SetBit(TIM5_CR1, T5CEN, 0);				                       // ��������
	CLRFlag_T5IR;
	CLRFlag_T5IF;

	if(DR > 0)
	{
		TIM5_DR = DR;		
		SetBit(TIM5_CR1, T5IDE, 1);				                     // ����Ƚ�/�½��ؼ���жϺ������ؼ���ж�					
	}
	else
	{
		SetBit(TIM5_CR1, T5IDE, 0);				                     // ��ֹ����Ƚ�/�½��ؼ���жϺ������ؼ���ж�			
	}
	if(ARR > 0)
	{
		TIM5_ARR = ARR;
		SetBit(TIM5_CR1, T5IFE, 1);				                     // ʹ������ж�				
	}
	else
	{
		TIM5_ARR = DR;
		SetBit(TIM5_CR1, T5IFE, 0);				                     // ʹ������ж�	
	}

	TIM5_CNTR = 0;
	SetBit(TIM5_CR1, T5CEN, 1);				                       // ��������
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void PWMIN_Init(void)
	Description   :	PWM��������ʼ��
	Input         :	��
  Output				:	��
-------------------------------------------------------------------------------------------------*/
void PWMIN_Init(void)
{
	PWMINCtl.PWMINCnt = 0;
	PWMINCtl.PWMINHigh = 0;
	PWMINCtl.PWMINPeriod = 0;
	PWMINCtl.PWMINHighDuty = 0;
}

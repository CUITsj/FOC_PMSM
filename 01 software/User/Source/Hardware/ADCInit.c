/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : ADCInit.c
* Author             : Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains ADC function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>




/*-------------------------------------------------------------------------------------------------
	Function Name :	void ADC_Init(void)
	Description   :	ADCӲ���豸��ʼ������
	Input         :	��
	Output				:	��
-------------------------------------------------------------------------------------------------*/
void ADC_Init(void)
{
/*-------------------------------------------------------------------------------------------------
	ADC��������
	1��ʹ��ADC
	4���������ݸ�ʽΪ12bit
	5����ֹADC Trigger
-------------------------------------------------------------------------------------------------*/
	SetBit(ADC_STA, ADCEN, 1);		                           // ʹ��ADC
	SetBit(ADC_STA, ADCBS, 0);                               // 12bit���ݸ�ʽ��0Ϊ12��1Ϊ10λ
	SetBit(ADC_CFG, ADTRIGEN, 0);                            // ��ֹADC Trigger

	
/*-------------------------------------------------------------------------------------------------
	ADC�ο���ѹѡ��
	ADCREF = 00��ѡ��VDD5��ΪADC�ο���ѹ
	ADCREF = 01��ѡ���ⲿVREF��ΪADC�ο���ѹ
	ADCREF = 10��ѡ���ڲ�VREF��ΪADC�ο���ѹ
	ADCREF = 11��ѡ���ڲ�VREF��ΪADC�ο���ѹ��ͬʱ�ο���ѹ�����VREF�ܽ�
-------------------------------------------------------------------------------------------------*/
	SetBit(ADC_CFG, ADCREF1, 1);
	SetBit(ADC_CFG, ADCREF0, 0);
	SetBit(VREF_CR, VREFEN , 1);//ʹ��VREF

/*-------------------------------------------------------------------------------------------------
	ADCͨ������
	1��ʹ����ӦPinΪģ��Pin����ֹ���ֹ���
	2��ʹ��ͨ��MASK������Pin��ADCģ��
-------------------------------------------------------------------------------------------------*/
//	SetBit(P3_AN, P34, 1);	
//	SetBit(ADC_MASK, CH7EN, 1);		                          //Config P3.4 as ADC7
	SetBit(P3_AN, P33, 1);	
	SetBit(ADC_MASK, CH6EN, 1);		                       		  //Config P3.3 as ADC6,VSP,ģ�����
// 	SetBit(P3_AN, P32, 1);	
// 	SetBit(ADC_MASK, CH5EN, 1);		                         	//Config P3.2 as ADC5
	SetBit(P2_AN, P27, 1);	
	SetBit(ADC_MASK, CH4EN, 1);		                            //Config P2.7 as ADC4 IBUS,�ܵ���
 	SetBit(P2_AN, P25, 1);	
 	SetBit(ADC_MASK, CH3EN, 1);		                        	  //Config P2.5 as ADC3 
	SetBit(P2_AN, P24, 1);	
	SetBit(ADC_MASK, CH2EN, 1);		                            //Config P2.4 as ADC2 DCBUS,ĸ�ߵ�ѹ
 	SetBit(P2_AN, P23, 1);	
 	SetBit(ADC_MASK, CH1EN, 1);		                         	  //Config P2.3 as ADC1 IV,V�����
 	SetBit(P2_AN, P20, 1);				
 	SetBit(ADC_MASK, CH0EN, 1);                            	  //Config P2.0 as ADC0 IU,U�����
	
	/*ADCͨ������ʱ����������*/
	ADC_SCYC1 = 0x33;								                        // 3��ADC Clock
	ADC_SCYC0 = 0x33;								                        // ADC2,ADC3 15 clock,3��ADC Clock
	
/*-------------------------------------------------------------------------------------------------
	ADC�ж�����
  1��ʹ��ADC�ж�
	2����ֹADC Trigger�ж�
	3������ADC�ж����ȼ���Ϊ0
-------------------------------------------------------------------------------------------------*/	
	CLR(ADC_STA, ADCIF);
	SetBit(ADC_STA, ADCIE, 0);	                            // 0����ֹADC�жϣ�1��ʹ��ADC�ж�
	CLR(ADC_CFG, ADTRIGIF);
	SetBit(ADC_CFG, ADTRIGIE, 0);	                          // 0����ֹADC Trigger�жϣ�1��ʹ��ADC Trigger�ж�
	
	PADC1 = 0;
	PADC0 = 0;										                          // ADC�ж����ȼ���Ϊ0
	EA = 1;	
	
}




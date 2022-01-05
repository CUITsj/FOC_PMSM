/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_POWER.h
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_POWER_H_
#define __FU68xx_POWER_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)


/* Register Define ------------------------------------------------------------------------------*/
sfr LVSR = 0xdb;
#define LVWF				1
#define LVWIF				0

#define VREF_CR XBYTE[0x404f]
#define VREFVSEL1		7
#define VREFVSEL0		6
#define VREFISEL		5
#define VREFEN			4
#define VHALFM			1
#define VHALFEN			0

#define VDD2_8      0
#define VDD3_0			1
#define VDD3_5			2
#define VDD3_8			3

#define VDD7_0      0
#define VDD8_0			1
#define VDD9_0			2
#define VDD10_0			3		

#define VREF3_0     0
#define VREF4_0			1
#define VREF4_5			2
#define VREF5_0			3	

#define VDD5_Half   0
#define VREF_Half		1		


/*-------------------------------------------------------------------------------------------------
	Function Name :	LVDPowerDownDetectConfig(Status)
	Description   :	������ʹ�ܣ���⵽VCC��ѹ�����趨ֵ�󴥷�Ԥ������VDD5��ѹ�����趨
	                ֵ�󴥷���λ��
	Input         :	Statue--ʹ�ܿ��ƣ�1--Disable��0--Enable	
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void LVDPowerDownDetectConfig(bit Status);


/*-------------------------------------------------------------------------------------------------
	Function Name :	LVDPowerDownWarnIEConfig(Status)
	Description   :	�����ѹԤ���ж�ʹ�ܣ���⵽VCC��ѹ�����趨ֵ�󴥷�Ԥ���������ѹԤ
                  ���ж�����Ӧ�жϡ�
	Input         :	Statue--ʹ�ܿ��ƣ�Disable��Enable	
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void LVDPowerDownWarnIEConfig(bit Status);


/*-------------------------------------------------------------------------------------------------
	Function Name :	LVRPowerDownResetConfig(Value)
	Description   :	���縴λֵ���ã���⵽VDD5��ѹ�����趨ֵ�󴥷��͵�ѹ��λ
	Input         :	Value--VDD5�͵�ѹ��λֵ
                  0: VDD2_8, VDD5�͵�ѹ��λֵΪ2.8V
									1: VDD3_0, VDD5�͵�ѹ��λֵΪ3.0V
									2: VDD3_5, VDD5�͵�ѹ��λֵΪ3.5V
									3: VDD3_8, VDD5�͵�ѹ��λֵΪ3.8V															
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void LVRPowerDownResetConfig(uint8 Value);


/*-------------------------------------------------------------------------------------------------
	Function Name :	LVWPowerDownWarnConfig(Value) 
	Description   :	����Ԥ��ֵ���ã���⵽VCC��ѹ�����趨ֵ�󴥷��͵�ѹԤ��
	Input         :	Value--VCC�͵�ѹԤ��ֵ
                  0: VDD7_0, VCC�͵�ѹԤ��ֵΪ7.0V
									1: VDD8_0, VCC�͵�ѹԤ��ֵΪ8.0V
									2: VDD9_0, VCC�͵�ѹԤ��ֵΪ9.0V
									3: VDD10_0, VCC�͵�ѹԤ��ֵΪ10.0V															
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void LVWPowerDownWarnConfig(uint8 Value);


/*-------------------------------------------------------------------------------------------------
	Function Name :	VREFConfig(Value, Status) 
	Description   :	�ο���ѹVREF���ã����ʹ��VREFģ�飬���ڲ�����VREF���뵽ADC��ΪADC
									�ο���ѹ�������ֹVREFģ�飬�����ⲿ�ṩVREF���뵽ADC��ΪADC�ο���ѹ��
	Input         :	Value--VCC�͵�ѹԤ��ֵ
                  0: VREF3_0, �ο���ѹVREF���Ϊ3.0V
									1: VREF4_0, �ο���ѹVREF���Ϊ4.0V
									2: VREF4_5, �ο���ѹVREF���Ϊ4.5V
									3: VREF5_0, �ο���ѹVREF���Ϊ5.0V	
                  Statue--ʹ�ܿ��ƣ�Disable��Enable�� 																								
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void VREFConfig(uint8 Value, bit Status);


/*-------------------------------------------------------------------------------------------------
	Function Name :	VHALFConfig(Mode, Status)
	Description   :	��׼��ѹVHALF���ã�ʹ��VHALF����ģ�飬ѡ��VREF/2����VDD5/2��Ϊ��׼
									��ѹVHALF�����VHALF�������˷Ż�׼��ѹ��
	Input         :	Mode--VHALF���ѡ��VREF_Half����VDD5_Half��
                  Statue--ʹ�ܿ��ƣ�Disable��Enable�� 																								
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void VHALFConfig(bit Mode, bit Status);




#endif

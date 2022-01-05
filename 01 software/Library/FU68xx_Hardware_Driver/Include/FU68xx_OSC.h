/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_OSC.h
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_OSC_H_
#define __FU68xx_OSC_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)

/* Register Define ------------------------------------------------------------------------------*/
sfr OSC_CFG = 0xf1;
#define EFCKEN 		  7
#define IFCKEN			6
#define CKFLAG			5
#define ESOSCAE			4
#define ESOSCEN			3


#define Crystal     0
#define EClock			1
#define InternalFastClock    0
#define ExternalFastClock    1	

/*-------------------------------------------------------------------------------------------------
	Function Name :	ExternalFastClockConfig(Mode, Status)
	Description   :	�ⲿ��ʱ�����ã�ѡ���ⲿ��ʱ������ģʽ��ǿ��ʹ���ⲿ��ʱ�Ӿ����·��
									�л�ϵͳʱ��Ϊ�ⲿ��ʱ��
	Input         :	Mode--�ⲿ��ʱ������ģʽ��Crystal(��Դ���壩��EClock(�ⲿʱ������)
                  Statue--ʹ�ܿ��ƣ�Disable��Enable	
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void ExternalFastClockConfig(bit Mode, bit Status);


/*-------------------------------------------------------------------------------------------------
	Function Name :	InternalFastClockConfig(Status)
	Description   :	�ڲ���ʱ�����ã�ǿ��ʹ���ڲ���ʱ�Ӿ����·���л�ϵͳʱ��Ϊ�ڲ���ʱ��
	Input         :	Statue--ʹ�ܿ��ƣ�Disable��Enable	
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void InternalFastClockConfig(bit Status);


/*-------------------------------------------------------------------------------------------------
	Function Name :	SystemFastClockConfig(Mode)
	Description   :	ϵͳ��ʱ�������л����ⲿ��ʱ��Ĭ��Ϊ��Դ����ģʽ��
	Input         :	Mode--ϵͳ��ʱ��ģʽ��InternalFastClock��ExternalFastClock
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void SystemFastClockConfig(bit Mode);


/*-------------------------------------------------------------------------------------------------
	Function Name :	ExternalSlowClockConfig(Status)
	Description   :	�ⲿ��ʱ�����ã�ʹ���ⲿ��ʱ�Ӿ����·
	Input         :	Statue--ʹ�ܿ��ƣ�Disable��Enable	
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void ExternalSlowClockConfig(bit Status);


/*-------------------------------------------------------------------------------------------------
	Function Name :	SystemFastClockMCDConfig(Status)
	Description   :	ϵͳ��ʱ�Ӷ�ʧ�������
	Input         :	Statue--ʹ�ܿ��ƣ�Disable��Enable	
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void SystemFastClockMCDConfig(bit Status); 


/*-------------------------------------------------------------------------------------------------
	Function Name :	SystemFastClockSPD2Config(Status)
	Description   :	ϵͳ��ʱ��˫��ģʽ����
	Input         :	Statue--ʹ�ܿ��ƣ�Disable��Enable	
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void SystemFastClockSPD2Config(bit Status);





#endif
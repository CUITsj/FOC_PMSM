/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_WDT.h
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_WDT_H_
#define __FU68xx_WDT_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)

/* Register Define ------------------------------------------------------------------------------*/
#define WDT_CSR XBYTE[0x4026]
#define WDTF	 	    1
#define WDTRF  			0
#define WDT_REL XBYTE[0x4027]


/*-------------------------------------------------------------------------------------------------
	Function Name ��void WatchDogRefresh(void
	Description   �����Ź���ʱ������ʼ�������Ź�ʹ�ܺ͸�λʹ�ܣ���ʱ����
	Input         ��Value--��ʱʱ�䣬��λms����С��ʱʱ��8ms�����ʱ1800ms
                  Statue--ʹ�ܿ��ƣ�Disable��Enable	
  Output				��None
-------------------------------------------------------------------------------------------------*/
extern void WatchDogConfig(uint16 Value, bit Status);

/*-------------------------------------------------------------------------------------------------
	Function Name ��WatchDogRefresh(Value ,Status)
	Description   ��ˢ�¿��Ź�������
	Input         ��None
  Output				��None
-------------------------------------------------------------------------------------------------*/
extern void WatchDogRefresh(void);


#endif

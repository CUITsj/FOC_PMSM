/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_FLASH.h
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_FLASH_H_
#define __FU68xx_FLASH_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)

/* Register Define ------------------------------------------------------------------------------*/
sfr FLKEY = 0x84;

sfr PSCTL = 0x85;
#define FLAVIO			4
#define	FLAACT			3
#define FLAERS			1
#define FLAWEN			0


/*-------------------------------------------------------------------------------------------------
	Function Name :	uint8 Flash_Sector_Erase(uint8 xdata *FlashAddress)
	Description   :	�����Բ���: ָ����Ҫ������Flash������ÿ������128Byte����128��������
									����0~127��ӦFlash��ַ0x0000~0x3fff��ͨ��ָ��Flash��ַ��ָ��Ҫ����
									��Flash��ַ����������һ��ֻ�ܲ���һ���������Բ�������Ϊ����ֵ��һ��
									Ҫ�ڽ�����Ÿ�DPTR��ֵ��
	Input         :	FlashAddress--Flash�Բ��������������ַ
  Output				:	0--Flash�Բ����ɹ���1--Flash�Բ���ʧ��
-------------------------------------------------------------------------------------------------*/
extern uint8 Flash_Sector_Erase(uint8 xdata *FlashAddress);

/*-------------------------------------------------------------------------------------------------
	Function Name :	uint8 Flash_Sector_Write(uint8 xdata *FlashAddress, uint8 FlashData)
	Description   :	Flash����д: ������Ԥ��̺��Բ����󣬿��Զ������ڵĵ�ַ����Flash��д��
                  һ����дһ��byte,һ��Ҫ�ڽ�����Ÿ�DPTR��ֵ
	Input         :	FlashAddress--Flash��д��ַ
									FlashData--Flash��д����
  Output				:	0--Flash����д�ɹ���1--Flash����дʧ��	
-------------------------------------------------------------------------------------------------*/
extern uint8 Flash_Sector_Write(uint8 xdata *FlashAddress, uint8 FlashData);

#endif

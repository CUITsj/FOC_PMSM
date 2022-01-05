
/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : PIInit.c
* Author             : Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains CMP initial function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>

void PI_Init(void)
{
	PI_KP = SKP;		// �ٶȻ� ����ϵ��KP							
	PI_KI = SKI;		// �ٶȻ� ����ϵ��KI		
	
	PI_EK = 0;			// �ٶȻ� �������
	PL_CR |= 0x02;		// Start PI		����PIģ��
	PI_UKMAX = SOUTMAX; // �ٶȻ� ��� ����޷�
	PI_UKMIN = SOUTMIN; // �ٶȻ� ��� ��С�޷�
	_nop_(); _nop_(); _nop_(); _nop_();	
	PI_UK =   0;		// �����ѹ��0
	PL_CR &= 0xFD;		// stop PI		ֹͣPIģ��
}
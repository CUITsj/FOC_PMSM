/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : GPIOInit.c
* Author             : Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains GPIO initial function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>


/*-------------------------------------------------------------------------------------------------
	Function Name :	void GPIO_Init(void)
	Description   :	GPIO��ʼ������,�ɽ�I/O�����ó���������ģʽ���������ǲ�������ģ����������������
	Input         :	��
  Output				:	��
-------------------------------------------------------------------------------------------------*/
void GPIO_Init(void)
{
//////	SetBit(P0_OE, P01, 1);  // config P01 as output,0 as input,1 as output
//////	SetBit(P0_PU, P01, 1);  // enable P01 pull up,0 as disable,1 as enable	
////	
////    SetBit(P0_OE, P02, 1);  // config P02 as input,0 as input,1 as output
////		GP02 = 0;
////	
//////	  SetBit(P0_OE, P05, 1);  // config P02 as input,0 as input,1 as output
//////		GP05 = 0;

////beep enable	
//	SetBit(P1_OE, P10, 1);	// config P11 as output,0 as input,1 as output
//	SetBit(P1_PU, P10, 0);  // enable P11 pull up	,0 as disable,1 as enable	
//	GP10 = 0;


//	
////	FG�ź�
//	SetBit(P0_OE, P01, 1);	// config P11 as output,0 as input,1 as output
//	SetBit(P0_PU, P01, 1);  // enable P01 pull up,0 as disable,1 as enable	
//	GP01 = 1;	

//// ON/OFF����	
//	SetBit(P0_OE, P02, 0);	// config P11 as output,0 as input,1 as output
//	SetBit(P0_PU, P02, 1);  // enable P01 pull up,0 as disable,1 as enable	
//	
//	
//// FR����	
//	SetBit(P0_OE, P03, 0);	// config P11 as output,0 as input,1 as output
//	SetBit(P0_PU, P03, 1);  // enable P01 pull up,0 as disable,1 as enable	
////	GP03 = 1;		
//	
//////	EER��
//	SetBit(P3_OE, P37, 1);	// config P11 as output,0 as input,1 as output
//	SetBit(P3_PU, P37, 0);  // enable P01 pull up,0 as disable,1 as enable	
//	GP37 = 0;

////	
//////	SDA��
////	SetBit(P0_OE, P00, 1);	// config P11 as output,0 as input,1 as output
////	SetBit(P0_PU, P00, 1);  // enable P01 pull up,0 as disable,1 as enable	
////	GP00 = 1;	

//	MODE
	SetBit(P0_OE, P00, 0);	// config P11 as output,0 as input,1 as output
	SetBit(P0_PU, P00, 1);  // enable P01 pull up,0 as disable,1 as enable	
	GP00 = 1;
}


/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : AddFunction.c
* Author             : Fortiortech  Appliction Team
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the add function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>
/*-------------------------------------------------------------------------------------------------
	Function Name : uint16 MDU_DIV_U16(uint16 xdata *TData0, uint16 xdata *TData1)
	Description   :	16λ�޷��ų���������16λ�޷���ֵ������ʱ��6.4us�������ڵ��ù��������
                  ���жϴ������ظ�����
	Input         :	TData0-��������TData1-������32λ��������  24/8=3  24�Ǳ�����
	Output				:	MD_DAL-�̵�16λ
-------------------------------------------------------------------------------------------------*/
uint16 MDU_DIV_U16(uint16 xdata *TData0, uint16 xdata *TData1)
{
	MD_DAH = *TData0;
	MD_DAL = *(TData0+1);

	MD_DBH = *TData1;
	MD_DBL = *(TData1+1);
							

	SetBit(MD_MODE,DIVSTART,1);
	while(!GetBit(MD_MODE,DIVDONE)); // �ȴ�MDU�������			

	return MD_DAL;			
}
/*---------------------------------------------------------------------------*/
/* Name		:	void MDU_16MUL16_INT(uint8 ALIGN, uint8 SIGN)
/* Input	:	NO
/* Output	:	NO
/* Description:	Inital MDU parameter
/*---------------------------------------------------------------------------*/
void MDU_16MUL16_INT(uint8 ALIGN, uint8 SIGN)
{
	if(ALIGN == 0)
	{
		MD_MODE &= ~(1<<ALIGN1);
		MD_MODE &= ~(1<<ALIGN0);				/*00:�˷��������0λ*/
	}
	else if(ALIGN == 8)
	{
		MD_MODE &= ~(1<<ALIGN1);
		MD_MODE |= (1<<ALIGN0);					/*01:�˷��������8λ*/
	}	
	else if(ALIGN == 12)
	{
		MD_MODE |= (1<<ALIGN1);
		MD_MODE &= ~(1<<ALIGN0);					/*10:�˷��������12λ*/
	}		
	else if(ALIGN == 15)
	{
		MD_MODE |= (1<<ALIGN1);
		MD_MODE |= (1<<ALIGN0);					/*11:�˷��������15λ*/
	}		
	else ;
	
	if(SIGN == 0)
	{
		MD_MODE &= ~(1<<MDSN);					/*ѡ���޷��ų˷�*/
	}
	else if(SIGN == 1)
	{
		MD_MODE |= (1<<MDSN);						/*ѡ���з��ų˷�*/
	}
	else ;
}
/*-------------------------------------------------------------------------------------------------
	Function Name :	void MDU_MUL_U32(uint16 TData0, uint16 TData1, uint16 xdata *Result32)
	Description   :	16λ�޷��ų˷�������32λ�޷���ֵ������ʱ��3.2us�������ڵ��ù��������
                  ���жϴ������ظ�����
	Input         :	TData0-����0; TData1-����1; Result32-32bi�˻�����ָ��
-------------------------------------------------------------------------------------------------*/
void MDU_MUL_U32(uint16 TData0, uint16 TData1, uint16 xdata *Result32)
{

	SetBit(MD_MODE, ALIGN1, 0);
	SetBit(MD_MODE, ALIGN0, 0);				                     // 00:�˷��������0λ				
	SetBit(MD_MODE, MDSN, 0);							                 // ѡ���޷��ų˷�

	MD_MA = TData0;
	MD_MB = TData1;								                         // д�������ͳ���
	
	*Result32 = MD_MA;
	*(Result32+1) = MD_MB;
}
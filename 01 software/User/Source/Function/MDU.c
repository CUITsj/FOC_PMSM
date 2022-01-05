/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : MDU.c
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the MDU  used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>

/* Private variables ----------------------------------------------------------------------------*/
MDUControl_TypeDef xdata MDUControl;


/*-------------------------------------------------------------------------------------------------
	Function Name :	void MDU_Init(void)
	Description   :	MDUģ���ʼ�����˷������λ���˷�ģʽ
	Input         :	None
  Output				:	None
-------------------------------------------------------------------------------------------------*/
void MDU_Init(void)
{
/*-------------------------------------------------------------------------------------------------
	�˷�����ʼ��
  00:�˷��������0λ	01:�˷��������8λ	10:�˷��������12λ	  11:�˷��������15λ
-------------------------------------------------------------------------------------------------*/
	SetBit(MD_MODE, ALIGN1, 1);
	SetBit(MD_MODE, ALIGN0, 1);				                       // 00:�˷��������0λ	
	
	SetBit(MD_MODE, MDSN, 0);						                     // ѡ���޷��ų˷�
	
	MDUControlInit();
}


/*-------------------------------------------------------------------------------------------------
	Function Name :	uint16 MDU_MUL_U16(uint16 TData0, uint16 TData1)
	Description   :	16λ�޷��ų˷�������16λ�޷���ֵ���˻�����65535�������������ʱ��
									1.2us�������ڵ��ù�����������жϴ������ظ�����
	Input         :	TData0-����0��TData1-����1
  Output				:	MD_MB-�˻���16λ
-------------------------------------------------------------------------------------------------*/
uint16 MDU_MUL_U16(uint16 TData0, uint16 TData1)
{
	SetBit(MD_MODE, ALIGN1, 0);
	SetBit(MD_MODE, ALIGN0, 0);				                     // 00:�˷��������0λ						
	SetBit(MD_MODE, MDSN, 0);						                   // ѡ���޷��ų˷�

	MD_MA = TData0;
	MD_MB = TData1;								                         // д�������ͳ���

	return MD_MB;	
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	uint32 MDU_MUL_U32(uint16 TData0, uint16 TData1)
	Description   :	16λ�޷��ų˷�������32λ�޷���ֵ������ʱ��10us�������ڵ��ù��������
                  ���жϴ������ظ�����
	Input         :	TData0-����0��TData1-����1
  Output				:	RData32-32λ�˻�
-------------------------------------------------------------------------------------------------*/
uint32 MDU_MUL_U32(uint16 TData0, uint16 TData1)
{
	uint32 RData32;
	uint16 *PData16; 
	
	SetBit(MD_MODE, ALIGN1, 0);
	SetBit(MD_MODE, ALIGN0, 0);				                     // 00:�˷��������0λ				
	SetBit(MD_MODE, MDSN, 0);							                 // ѡ���޷��ų˷�

	MD_MA = TData0;
	MD_MB = TData1;								                         // д�������ͳ���
	
	PData16 = &RData32;
	*PData16 = MD_MA;
	PData16 += 1;
	*PData16 = MD_MB;

	return RData32;
	
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	int16 MDU_MUL_U16(int16 TData0, int16 TData1)
	Description   :	16λ�з��ų˷�������16λ�з���ֵ���˻�����32767�������,����ʱ��
									1.2us�������ڵ��ù�����������жϴ������ظ�����
	Input         :	TData0-����0��TData1-����1
  Output				:	MD_MB-�˻���16λ
-------------------------------------------------------------------------------------------------*/
int16 MDU_MUL_S16(int16 TData0, int16 TData1)
{
	SetBit(MD_MODE, ALIGN1, 0);
	SetBit(MD_MODE, ALIGN0, 0);				                     // 00:�˷��������0λ				
	SetBit(MD_MODE, MDSN, 1);						                   // ѡ���з��ų˷�

	MD_MA = TData0;
	MD_MB = TData1;								                         // д�������ͳ���
	
	return MD_MB;
}

/*-------------------------------------------------------------------------------------------------
	Function Name : int32 MDU_MUL_S32(int16 TData0, int16 TData1)
	Description   :	32�з��ų˷�������32λ�з���ֵ������ʱ��10us�������ڵ��ù��������
                  ���жϴ������ظ�����
	Input         :	TData0-����0��TData1-����1
  Output				:	RData32-32λ�˻�
-------------------------------------------------------------------------------------------------*/
int32 MDU_MUL_S32(int16 TData0, int16 TData1)
{
	int32 RData32;
	int16 *PData16;
	
	SetBit(MD_MODE, ALIGN1, 0);
	SetBit(MD_MODE, ALIGN0, 0);				                     // 00:�˷��������0λ				
	SetBit(MD_MODE, MDSN, 1);						                   // ѡ���з��ų˷�

	MD_MA = TData0;
	MD_MB = TData1;								                         // д�������ͳ���

	PData16 = &RData32;
	*PData16 = MD_MA;
	PData16 += 1;
	*PData16 = MD_MB;

	return RData32;
	
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	uint16 MDU_MULA_U16(uint16 TData0, uint16 TData1, uint8 ALIGN)
	Description   :	16λ�޷��ų˷�������16λ�޷���ֵ���˻�����65535�����������ѡ������
									�����λ������ʱ��2.5us�������ڵ��ù�����������жϴ������ظ�����
	Input         :	TData0-����0��TData1-����1
  Output				:	MD_MB-�˻���16λ
-------------------------------------------------------------------------------------------------*/
uint16 MDU_MULA_U16(uint16 TData0, uint16 TData1, uint8 ALIGN)
{
	switch(ALIGN)
	{
		case 0:
			SetBit(MD_MODE, ALIGN1, 0);
			SetBit(MD_MODE, ALIGN0, 0);				                 // 00:�˷��������0λ		
			break;
		case 8:
			SetBit(MD_MODE, ALIGN1, 0);
			SetBit(MD_MODE, ALIGN0, 1);					               // 01:�˷��������8λ
			break;
		case 12:
			SetBit(MD_MODE, ALIGN1, 1);
			SetBit(MD_MODE, ALIGN0, 0);				                 // 10:�˷��������12λ
			break;
		case 15:
			SetBit(MD_MODE, ALIGN1, 1);
			SetBit(MD_MODE, ALIGN0, 1);					               // 11:�˷��������15λ
			break;
		default:
			break;
	}
	
	SetBit(MD_MODE, MDSN, 0);						                   // ѡ���޷��ų˷�

	MD_MA = TData0;
	MD_MB = TData1;								                         // д�������ͳ���

	return MD_MB;
	
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	uint32 MDU_MULA_U32(uint16 TData0, uint16 TData1, uint8 ALIGN)
	Description   :	16λ�޷��ų˷�������32λ�޷���ֵ����ѡ����������λ������ʱ��10us��
									�����ڵ��ù�����������жϴ������ظ�����
	Input         :	TData0-����0��TData1-����1
  Output				:	RData32-32λ�˻�
-------------------------------------------------------------------------------------------------*/
uint32 MDU_MULA_U32(uint16 TData0, uint16 TData1, uint8 ALIGN)
{
	uint32 RData32;
	uint16 *PData16; 
	
	switch(ALIGN)
	{
		case 0:
			SetBit(MD_MODE, ALIGN1, 0);
			SetBit(MD_MODE, ALIGN0, 0);				                 // 00:�˷��������0λ		
			break;
		case 8:
			SetBit(MD_MODE, ALIGN1, 0);
			SetBit(MD_MODE, ALIGN0, 1);					               // 01:�˷��������8λ
			break;
		case 12:
			SetBit(MD_MODE, ALIGN1, 1);
			SetBit(MD_MODE, ALIGN0, 0);				                 // 10:�˷��������12λ
			break;
		case 15:
			SetBit(MD_MODE, ALIGN1, 1);
			SetBit(MD_MODE, ALIGN0, 1);					               // 11:�˷��������15λ
			break;
		default:
			break;
	}

	SetBit(MD_MODE, MDSN, 0);							                 // ѡ���޷��ų˷�

	MD_MA = TData0;
	MD_MB = TData1;								                         // д�������ͳ���
	
	PData16 = &RData32;
	*PData16 = MD_MA;
	PData16 += 1;
	*PData16 = MD_MB;

	return RData32;
	
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	int16 MDU_MUL_U16(int16 TData0, int16 TData1)
	Description   :	16λ�з��ų˷�������16λ�з���ֵ���˻�����32767�������,����ʱ��
									2.5us�������ڵ��ù�����������жϴ������ظ�����
	Input         :	TData0-����0��TData1-����1
  Output				:	MD_MB-�˻���16λ
-------------------------------------------------------------------------------------------------*/
int16 MDU_MULA_S16(int16 TData0, int16 TData1, uint8 ALIGN)
{
	switch(ALIGN)
	{
		case 0:
			SetBit(MD_MODE, ALIGN1, 0);
			SetBit(MD_MODE, ALIGN0, 0);				                 // 00:�˷��������0λ		
			break;
		case 8:
			SetBit(MD_MODE, ALIGN1, 0);
			SetBit(MD_MODE, ALIGN0, 1);					               // 01:�˷��������8λ
			break;
		case 12:
			SetBit(MD_MODE, ALIGN1, 1);
			SetBit(MD_MODE, ALIGN0, 0);				                 // 10:�˷��������12λ
			break;
		case 15:
			SetBit(MD_MODE, ALIGN1, 1);
			SetBit(MD_MODE, ALIGN0, 1);					               // 11:�˷��������15λ
			break;
		default:
			break;
	}
	
	SetBit(MD_MODE, MDSN, 1);						                   // ѡ���з��ų˷�

	MD_MA = TData0;
	MD_MB = TData1;								                         // д�������ͳ���

	return MD_MB;		
	
}

/*-------------------------------------------------------------------------------------------------
	Function Name : int32 MDU_MULA_S32(int16 TData0, int16 TData1, uint8 ALIGN)
	Description   :	32�з��ų˷�������32λ�з���ֵ����ѡ����������λ������ʱ��10us��
									�����ڵ��ù�����������жϴ������ظ�����
	Input         :	TData0-����0��TData1-����1
  Output				:	RData32-32λ�˻�
-------------------------------------------------------------------------------------------------*/
int32 MDU_MULA_S32(int16 TData0, int16 TData1, uint8 ALIGN)
{
	int32 RData32;
	int16 *PData16;  	
		
	switch(ALIGN)
	{
		case 0:
			SetBit(MD_MODE, ALIGN1, 0);
			SetBit(MD_MODE, ALIGN0, 0);				                 // 00:�˷��������0λ		
			break;
		case 8:
			SetBit(MD_MODE, ALIGN1, 0);
			SetBit(MD_MODE, ALIGN0, 1);					               // 01:�˷��������8λ
			break;
		case 12:
			SetBit(MD_MODE, ALIGN1, 1);
			SetBit(MD_MODE, ALIGN0, 0);				                 // 10:�˷��������12λ
			break;
		case 15:
			SetBit(MD_MODE, ALIGN1, 1);
			SetBit(MD_MODE, ALIGN0, 1);					               // 11:�˷��������15λ
			break;
		default:
			break;
	}
	
	SetBit(MD_MODE, MDSN, 1);						                   // ѡ���з��ų˷�

	MD_MA = TData0;
	MD_MB = TData1;								                         // д�������ͳ���

	PData16 = &RData32;
	*PData16 = MD_MA;
	PData16 += 1;
	*PData16 = MD_MB;
	
	return RData32;
	
}


/*-------------------------------------------------------------------------------------------------
	Function Name : uint16 MDU_DIV_U16(uint16 TData0, uint16 TData1)
	Description   :	16λ�޷��ų˷�������16λ�޷���ֵ������ʱ��7.5us�������ڵ��ù��������
                  ���жϴ������ظ�����
	Input         :	TData0-��������TData1-����
  Output				:	MD_DAL-�̵�16λ
-------------------------------------------------------------------------------------------------*/
uint16 MDU_DIV_U16(uint8 xdata *TData0, uint8 xdata *TData1)
{
	MD_DA3 = *TData0;
	MD_DA2 = *(TData0+1);
	MD_DA1 = *(TData0+2);
	MD_DA0 = *(TData0+3);

	MD_DB3 = *TData1;
	MD_DB2 = *(TData1+1);
	MD_DB1 = *(TData1+2);
	MD_DB0 = *(TData1+3);
							

	SetBit(MD_MODE,DIVSTART,1);
	while(!GetBit(MD_MODE,DIVDONE));			                 // �ȴ�MDU�������			

  return MD_DAL;
					
}


/*-------------------------------------------------------------------------------------------------
	Function Name : uint32 MDU_DIV_U32(uint8 xdata *TData0, uint8 xdata *TData1)
	Description   :	32λ�޷��ų˷�������32λ�޷���ֵ������ʱ��10us�������ڵ��ù��������
                  ���жϴ������ظ�����
	Input         :	TData0-��������TData1-����
  Output				:	RData32-32λ��
-------------------------------------------------------------------------------------------------*/
uint32 MDU_DIV_U32(uint8 xdata *TData0, uint8 xdata *TData1)
{
	uint32 RData32;
	uint16 *PData16;
	
	MD_DA3 = *TData0;
	MD_DA2 = *(TData0+1);
	MD_DA1 = *(TData0+2);
	MD_DA0 = *(TData0+3);

	MD_DB3 = *TData1;
	MD_DB2 = *(TData1+1);
	MD_DB1 = *(TData1+2);
	MD_DB0 = *(TData1+3);

	SetBit(MD_MODE,DIVSTART,1);
	while(!GetBit(MD_MODE,DIVDONE));			                 // �ȴ�MDU�������			

	PData16 = &RData32;
	*PData16 = MD_DAH;
	PData16 += 1;
	*PData16 = MD_DAL;

  return RData32;
					
}


/*-------------------------------------------------------------------------------------------------
	Function Name : void MDUControlInit(void)
	Description   :	MDU���ƺ�����־λ��ʼ��
	Input         :	��
  Output				:	��
-------------------------------------------------------------------------------------------------*/
void MDUControlInit(void)
{
	MDUControl.FlagTheta = 0;
	MDUControl.FlagAngleErrCW = 0;
	MDUControl.FlagAngleErrCCW = 0;
	
	MDUControl.FlagSpeed = 0;
	MDUControl.FlagPISpeed = 0;
	MDUControl.FlagTargetSpeed = 0;
	MDUControl.FlagPITargetSpeed = 0;
	
	MDUControl.FlagCurrentBUS = 0;                          
	MDUControl.FlagPICurrentBUS = 0;
	MDUControl.FlagPITargetCurrentBUS = 0;
	
	MDUControl.FlagPWMINHighDuty = 0;
}

/*-------------------------------------------------------------------------------------------------
	Function Name : void MDUAPP(void)
	Description   :	MDU���ƺ���������Ƶ��ΪPWMFre(16K)��������ȼ������ϵͳ������MDU���㶼�ڴ˺���
                  ִ�У�����ɨ�赽��Ӧ�ı�־λ�������Ӧ�����㡣
	Input         :	��
  Output				:	��
-------------------------------------------------------------------------------------------------*/
void MDUAPP(void)
{
	uint32 Temp32;
	
  /*Angle Calculate*/
//	if(MDUControl.FlagAngleErrCW)
//	{		
//		MC.AngleErr = MDU_MULA_U16(MC.SpeedCtl.Speed, TempAngleErrKPCW, 12);
//		MDUControl.FlagAngleErrCW = 0;
//	}
//	if(MDUControl.FlagAngleErrCCW)
//	{		
//		MC.AngleErr = MDU_MULA_U16(MC.SpeedCtl.Speed, TempAngleErrKPCCW, 12);
//		MDUControl.FlagAngleErrCCW = 0;
//	}
//	if(MDUControl.FlagTheta)
//	{
//		Temp32 = TempTheta;
//		MC.Theta = MDU_DIV_U16(&Temp32, &MC.PeriodTime);			// 750000Hz/PWMFre/MC.PeriodTime*65535 
//		MDUControl.FlagTheta = 0;
//	}
	
	/*Speed Control*/
	#if (SpeedCloseLoopEnable)
	{	
		if(MDUControl.FlagSpeed)
		{
			Temp32 = TempSpeed;
			MC.SpeedCtl.Speed = MDU_DIV_U16(&Temp32, &MC.PeriodTime);//Speed=750KHz/(MC.PeriodTime*MC.Pole)*60, rpm	
			MDUControl.FlagSpeed = 0;
		}
		if(MDUControl.FlagPISpeed)
		{
			Temp32 = TempPISpeed;
			MC.SpeedCtl.PISpeed = MDU_DIV_U16(&Temp32, &MC.PeriodTime);//0~SPEEDBASE(rpm)-->0~32767
			MDUControl.FlagPISpeed = 0;
		}
		if(MDUControl.FlagTargetSpeed)
		{
			MC.SpeedCtl.TargetSpeed = MDU_MULA_U16((ADCData.VoltageSREF-TempSREFMin), TempTargetSpeed, 15) + SpeedMin;
			MDUControl.FlagTargetSpeed = 0;
		}
		if(MDUControl.FlagPITargetSpeed)
		{	
			MC.SpeedCtl.PITargetSpeed = MDU_MULA_U16(MC.SpeedCtl.TargetSpeed, TempPITargetSpeed, 8);	
			MDUControl.FlagPITargetSpeed = 0;
		}
	}
	#endif
	
	/*Current Control*/
//	#if (CurrentCloseLoopEnable)
//	{	
//		if(MDUControl.FlagCurrentBUS) 
//		{
//			MC.CurrentCtl.CurrentBUS = MDU_MULA_U16(MDU_MULA_U16(ADCData.CurrentBUS, MC.PWMDutyCtl.VS, 15), 23169, 15);// 32767/Sqr(2)=23169
//			MDUControl.FlagCurrentBUS = 0;
//		}
//		if(MDUControl.FlagPICurrentBUS)
//		{
//			MC.CurrentCtl.PICurrentBUS = MDU_MULA_U16(MC.CurrentCtl.CurrentBUS, TempCurrentBase, 8);	
//			MDUControl.FlagPICurrentBUS = 0;
//		}	
//		if(MDUControl.FlagPITargetCurrentBUS)
//		{
//			MC.CurrentCtl.PITargetCurrentBUS = MDU_MULA_U16(MC.CurrentCtl.TargetCurrentBUS, TempCurrentBase, 8);
//			MDUControl.FlagPITargetCurrentBUS = 0;
//		}
//	}
	#endif
	
	
	#if (SpeedPWMEnable)                                 
	{	
		if(MDUControl.FlagPWMINHighDuty)
		{
			Temp32 = MDU_MUL_U32(PWMINCtl.PWMINHigh, 32767);
			PWMINCtl.PWMINHighDuty = MDU_DIV_U16(&Temp32, &PWMINCtl.PWMINPeriod);	
			MDUControl.FlagPWMINHighDuty = 0;
		}
	}
	#endif	
}









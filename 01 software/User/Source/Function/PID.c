/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : PID.c
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains PI control function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>

/* Private variables ----------------------------------------------------------------------------*/
bit FlagStartPI = 0;

Ramp_TypeDef idata SpeedRamp;
PID_TypeDef idata SpeedPID;
PID_TypeDef idata CurrentPID;


/*-------------------------------------------------------------------------------------------------
	Function Name :	void SpeedPIDInit(void)
	Description   :	�ٶ�PID������ʼ��
	Input         :	��
  Output				:	��
-------------------------------------------------------------------------------------------------*/
void SpeedPIDInit(void)
{
	SpeedPID.Kp = TempSpeedKP;						
	SpeedPID.Ki = TempSpeedKI;						
	SpeedPID.Err = 0;
	SpeedPID.Err_Last1 = 0;
	SpeedPID.Err_Err = 0;
	#if (SpeedCloseLoopEnable)
	{
		SpeedPID.Out = TempMotorStartDuty;
		SpeedPID.OutMax = MotorVSMax;
		SpeedPID.OutMin = TempMotorStartDuty;	
	}
	#elif ((SpeedCloseLoopEnable)&&(CurrentCloseLoopEnable))
	{	
		SpeedPID.Out = 0;
		SpeedPID.OutMax = TempCurrentMax;
		SpeedPID.OutMin = TempCurrentMin;					
	}
	#endif		
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void SpeedRampInit(void)
	Description   :	�ٶ����²�����ʼ��
	Input         :	��
  Output				:	��
-------------------------------------------------------------------------------------------------*/
void SpeedRampInit(void)
{
	SpeedRamp.RampInc = SpeedRampInc;
	SpeedRamp.RampDec = SpeedRampDec;
	SpeedRamp.RampStep = MC.SpeedCtl.PISpeed;
}	

/*-------------------------------------------------------------------------------------------------
	Function Name :	void CurrentPIDInit(void)
	Description   :	����PI������ʼ��
	Input         :	��
  Output				:	��
-------------------------------------------------------------------------------------------------*/
void CurrentPIDInit(void)
{
	CurrentPID.Kp = TempCurrentKP;						
	CurrentPID.Ki = TempCurrentKI;						
	CurrentPID.Err = 0;
	CurrentPID.Err_Last1 = 0;
	CurrentPID.Err_Err = 0;
	CurrentPID.Out = TempMotorStartDuty;
	CurrentPID.OutMax = MotorVSMax;
	CurrentPID.OutMin = TempMotorStartDuty;
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	int16 RampControl(Ramp_TypeDef *Ramp, int16 Ref)
	Description   :	���¿��ƺ����������趨���������ѵ�ǰֵ��Ŀ��ֵ��£
	Input         :	Ramp--���¶�������
                  Ref--���²ο�Ŀ��
  Output				:	RampStep--���µ�ǰ���
-------------------------------------------------------------------------------------------------*/
int16 RampControl(Ramp_TypeDef *Ramp, int16 Ref)
{
	Ramp->RampTarget = Ref;
	if(Ramp->RampTarget > Ramp->RampStep)
	{
		if((Ramp->RampStep + Ramp->RampInc) <= Ramp->RampTarget)
		{
			Ramp->RampStep += Ramp->RampInc;
		}
		else
		{
			Ramp->RampStep = Ramp->RampTarget;
		}
	}
	else if(Ramp->RampTarget < Ramp->RampStep)
	{
		if((Ramp->RampStep - Ramp->RampDec) >= Ramp->RampTarget)
		{
			Ramp->RampStep -= Ramp->RampDec;
		}
		else
		{
			Ramp->RampStep = Ramp->RampTarget;
		}
	}
	
	return 	Ramp->RampStep;
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	int16 PIDControl(PID_TypeDef *PID, int16 Ref, int16 Cur)
	Description   :	PI���ں���
	Input         :	PID--PI���ڶ�������
                  Ref--PI���ڲο�Ŀ��
                  Cur--PI��ǰ����
  Output				:	Out--PI�������
-------------------------------------------------------------------------------------------------*/
int16 PIDControl(PID_TypeDef *PID, int16 Ref, int16 Cur)
{
	int32 Kp_Out, Ki_Out, PID_Out;

	/*PI����ʱ��67us*/
	if(!PID->Err)
	{
		PID->Err_Last1 = Ref - Cur;														 // ��ʼ��PID�ϴ�ƫ��
		PID->Err = Ref - Cur;																	 // ��ʼ��PID��ǰƫ��
		PID->Err_Err=PID->Err - PID->Err_Last1;								 // ��ʼ��PID�ϴ�ƫ������ϴ�ƫ��֮��
	}
	else
	{
		PID->Err_Last1 = PID->Err;														 // ����PID�ϴ�ƫ��
		PID->Err = Ref - Cur;																	 // ����PID��ǰƫ��
		PID->Err_Err=PID->Err - PID->Err_Last1;								 // ����PID�ϴ�ƫ������ϴ�ƫ��֮��		
	}

	Kp_Out = ((int32)PID->Kp * (int32)PID->Err_Err) >> 12;
	Ki_Out = ((int32)PID->Ki * (int32)PID->Err) >> 12;

	PID_Out = PID->Out;
	PID_Out += Kp_Out + Ki_Out;

	if(PID_Out > PID->OutMax)
	{
		PID_Out = PID->OutMax;												         // PID������
	}
	if(PID_Out < PID->OutMin)
	{
		PID_Out = PID->OutMin;												         // PID������
	}
	PID->Out = PID_Out;
 	
	return PID->Out;	
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	int16 PIDControl(PID_TypeDef *PID, int16 Ref, int16 Cur)
	Description   :	PI���ں���
	Input         :	PID--PI���ڶ�������
                  Ref--PI���ڲο�Ŀ��
                  Cur--PI��ǰ����
  Output				:	Out--PI�������
-------------------------------------------------------------------------------------------------*/
int16 PID_Control(PID_TypeDef *PID, int16 Ref, int16 Cur)
{
	int16 Kp_Out;
	int16 Ki_Out;
	int32 PID_Out;

	/*PI����ʱ��42us*/
	if(!PID->Err)
	{
		PID->Err_Last1 = Ref - Cur;														 // ��ʼ��PID�ϴ�ƫ��
		PID->Err = Ref - Cur;																	 // ��ʼ��PID��ǰƫ��
		PID->Err_Err=PID->Err - PID->Err_Last1;								 // ��ʼ��PID�ϴ�ƫ������ϴ�ƫ��֮��
	}
	else
	{
		PID->Err_Last1 = PID->Err;														 // ����PID�ϴ�ƫ��
		PID->Err = Ref - Cur;																	 // ����PID��ǰƫ��
		PID->Err_Err=PID->Err - PID->Err_Last1;								 // ����PID�ϴ�ƫ������ϴ�ƫ��֮��		
	}
	
	Kp_Out = MDU_MULA_S16(PID->Kp, PID->Err_Err, 12);
	Ki_Out = MDU_MULA_S16(PID->Ki, PID->Err, 12);
	PID_Out = PID->Out;
	PID_Out += Kp_Out + Ki_Out;
		
	if(PID_Out > PID->OutMax)
	{
		PID_Out = PID->OutMax;												         // PID������
	}
	if(PID_Out < PID->OutMin)
	{
		PID_Out = PID->OutMin;												         // PID������
	}
	PID->Out = PID_Out;
	
	return PID->Out;	
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void PI_Init(void)
	Description   :	PIģ���ʼ��
	Input         :	��
  Output				:	��
-------------------------------------------------------------------------------------------------*/
void PI_Init(void)
{	
/*-------------------------------------------------------------------------------------------------
	PI���ݸ�ʽ
	Q = PIRANGE + 8(Q8~Q23���ݸ�ʽ)
  ���Ĵ�����ֵʱҪͳһ���ݸ�ʽ
-------------------------------------------------------------------------------------------------*/		
	SetBit(SV_CR, PIRANGE3, 0);															// 0100: 12bit PI
	SetBit(SV_CR, PIRANGE2, 1);
	SetBit(SV_CR, PIRANGE1, 0);
	SetBit(SV_CR, PIRANGE0, 0);				                       // PI��λQ=PIRANGE+8����PI���ݸ�ʽ							

  PI_KP = 0;												                       // �Ĵ����������ͣ�int16 Q12��ʽ��PI_KP=Kp*2^12��
	PI_KI = 0;												                       // �Ĵ����������ͣ�int16 Q12��ʽ��PI_KI=Kp*2^12
	PI_EK = 0;												
  PI_UK = 0;												                       // �Ĵ����������ͣ�int16
	PI_UKMAX = 0;											                       // �Ĵ����������ͣ�int16
	PI_UKMIN = 0;											                       // �Ĵ����������ͣ�int16
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	int16 PIDControl(PID_TypeDef *PID, int16 Ref, int16 Cur)
	Description   :	PI���ں���
	Input         :	PID--PI���ڶ�������
                  Ref--PI���ڲο�Ŀ��
                  Cur--PI��ǰ����
  Output				:	Out--PI�������
-------------------------------------------------------------------------------------------------*/
int16 PI_Control(PID_TypeDef *PID, int16 Ref, int16 Cur)
{
	static uint8 *PIDType = 0;
	
	/*PI����ʱ��10us*/	
	FlagStartPI = 1;
	while(FlagStartPI)                                       // MDU��ʱ���ñ�־
	{
		FlagStartPI = 0;	

		/*ָ�����������ж�ִ��ʱ��2.5us*/
		if(PIDType != PID)                                     // ͨ��PID���ݵ�ַ��ȡPID���ͣ�
		{
			PIDType = PID;
			
			PI_KP = PID->Kp;									                   // �Ĵ����������ͣ�int16 Q12��ʽ��PI_KP=Kp*2^12��
			PI_KI = PID->Ki;									                   // �Ĵ����������ͣ�int16 Q12��ʽ��PI_KI=Kp*2^12
			PI_EK = PID->Err;												
			PI_UK = PID->Out;									                   // �Ĵ����������ͣ�int16
			PI_UKMAX = PID->OutMax;						                   // �Ĵ����������ͣ�int16
			PI_UKMIN = PID->OutMin;						                   // �Ĵ����������ͣ�int16		
			
			SetBit(SV_CR,PISTA,1);						                   // ����PI
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();													                   // �ȴ�PI�������,PI_LastErr��ʼ���ɹ

		}
		/*��ֵ����ʱ��3.2us*/	
		PID->Err = Ref - Cur;								                   // ����PID��ǰƫ��  

		/*PIӲ������ʱ��1.8us*/
		PI_EK = PID->Err;										                   // PI����
		PI_UK = PID->Out;									                     // �Ĵ����������ͣ�int16
		SetBit(SV_CR,PISTA,1);
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		PID->Out = PI_UK;										                    // PI���
	}
  FlagStartPI = 1;
	
  return PID->Out;
}


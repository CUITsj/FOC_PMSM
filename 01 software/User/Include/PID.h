/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : PID.h
* Author             : Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __PID_H_
#define __PID_H_

/* Exported types -------------------------------------------------------------------------------*/
typedef struct 
{
	int16 Kp;						                                     // ����PID����ϵ��
	int16 Ki;						                                     // ����PID����ϵ��
	int16 Err;						                                   // ����PID��ǰƫ��
	int16 Err_Last1;				                                 // ����PID�ϴ�ƫ��
	int16 Err_Err;					                                 // ����PID��ǰƫ����ϴ�ƫ��֮�Err_Err=Err-Err_Last1
	int16 Out;						                                   // ����PID���
	int16 OutMax;					                                   // ����PID�����������ֵ
	int16 OutMin;					                                   // ����PID�����С�����ֵ
}PID_TypeDef;

typedef struct 
{
	int16 RampInc;
	int16 RampDec;
	int16 RampTarget;
	int16 RampStep;
}Ramp_TypeDef;

/* Exported variables ---------------------------------------------------------------------------*/
extern PID_TypeDef idata SpeedPID;
extern Ramp_TypeDef idata SpeedRamp;
extern PID_TypeDef idata CurrentPID;

/* Exported functions ---------------------------------------------------------------------------*/
extern int16 RampControl(Ramp_TypeDef *Ramp, int16 Ref);
extern int16 PIDControl(PID_TypeDef *PID, int16 Ref, int16 Cur);
extern int16 PID_Control(PID_TypeDef *PID, int16 Ref, int16 Cur);

extern void PI_Init(void);
extern int16 PI_Control(PID_TypeDef *PID, int16 Ref, int16 Cur);

extern void SpeedRampInit(void);
extern void SpeedPIDInit(void);									
extern void CurrentPIDInit(void);									


#endif
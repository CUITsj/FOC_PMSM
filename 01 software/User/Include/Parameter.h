/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : Parameter.h
* Author             : Fortiortech  Appliction Team
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all FOC debug parameter.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/

#ifndef __Parameter_H_
#define __Parameter_H_

/* 1. _Q���� */
#define     _Q7(A)                              (int8 )(A * 128)
#define     _Q10(A)                             (int16)(A * 1024)
#define     _Q11(A)                             (int16)(A * 2048)
#define     _Q12(A)                             (int16)(A * 4096)
#define     _Q13(A)                             (int16)(A * 8192)
#define     _Q15(A)                             (int16)(A * 32767)
#define     _2PI                                (3.1415926 * 2)
#define     _Q16                                (65535.0)
/* Private define ------------------------------------------------------------*/

#define SystemPowerUpTime								(10000)											// �ϵ�ȴ�ʱ�䣬cpu����ʱ��		

/*оƬ����ֵ------------------------------------------------------------------*/
/*CPU and PWM Parameter*/
#define 	PWM_CYCLE									(1000.0/PWM_FREQUENCY)							// ����us
#define 	SAMP_FREQ									(PWM_FREQUENCY*1000)							// ����Ƶ��(HZ)
#define 	TPWM_VALUE									(1.0/SAMP_FREQ)									// �ز�����(S)
#define 	PWM_VALUE_LOAD								(uint16)(MCU_CLOCK*500/PWM_FREQUENCY - 1)		// PWM ��ʱ������ֵ


/*deadtime Parameter*/
#define		PWM_LOAD_DEADTIME							(PWM_DEADTIME * MCU_CLOCK)						// ��������ֵ

/*single resistor sample Parameter*/
#define		PWM_TS_LOAD									(uint16)(_Q16 / PWM_CYCLE * MIN_WIND_TIME)		// �������������ֵ


/*Ӳ�����Ӳ�������ֵ------------------------------------------------------------------*/
/*hardware current sample Parameter*/
/*������׼�ĵ�·����*/
#define 	HW_BOARD_CURR_MAX							(HW_ADC_REF/2/HW_AMPGAIN/HW_RSHUNT)				// ����������,2.702A
#define 	HW_BOARD_CURR_MIN							(-HW_BOARD_CURR_MAX)							// ��С��������,-2.702A
#define 	HW_BOARD_CURR_BASE							(HW_BOARD_CURR_MAX*2)							// ������׼//5.4A
#define 	HW_POWER_CURR_BASE							(HW_BOARD_CURR_BASE*256)						// power base value: HW_BOARD_CURR_BASE*Q8

/*hardware voltage sample Parameter*/
/*ĸ�ߵ�ѹ������ѹ��·����*/
#define 	HW_BOARD_VOLTAGE_BASE						(HW_BOARD_VOLT_MAX/1.732)				// ��ѹ��׼
#define 	HW_POWER_VOLT_BASE							(HW_BOARD_VOLTAGE_BASE*256)						// HW_BOARD_VOLT_MAX*Q8

/*��������ֵ------------------------------------------------------------------*/
/* protect value */
#define		OVER_PROTECT_VALUE  			  			_Q15(Over_Protect_Voltage/HW_BOARD_VOLT_MAX)
#define		UNDER_PROTECT_VALUE  						_Q15(Under_Protect_Voltage/HW_BOARD_VOLT_MAX)
#define		OVER_RECOVER_VALUE    						_Q15(Over_Recover_Vlotage/HW_BOARD_VOLT_MAX)
#define		UNDER_RECOVER_VALUE  						_Q15(Under_Recover_Vlotage/HW_BOARD_VOLT_MAX)


/* motor speed set value */
#define		Motor_Open_Ramp_Min							_Q15(MOTOR_OPEN_ACC_MIN/MOTOR_SPEED_BASE)

#define		Motor_Omega_Ramp_Min						_Q15(MOTOR_OMEGA_ACC_MIN/MOTOR_SPEED_BASE)
#define		Motor_Omega_Ramp_End						_Q15(MOTOR_OMEGA_ACC_END/MOTOR_SPEED_BASE)

#define 	Motor_Loop_Speed							_Q15(MOTOR_LOOP_RPM/MOTOR_SPEED_BASE)

#define 	Motor_Max_Speed								_Q15(MOTOR_SPEED_MAX_RPM/MOTOR_SPEED_BASE)
#define 	Motor_Min_Speed								_Q15(MOTOR_SPEED_MIN_RPM/MOTOR_SPEED_BASE)
#define 	Motor_Limit_Speed							_Q15(MOTOR_SPEED_LIMIT_RPM/MOTOR_SPEED_BASE)

#define 	Motor_Stall_Min_Speed						_Q15(MOTOR_SPEED_STAL_MIN_RPM/MOTOR_SPEED_BASE)
#define 	Motor_Stall_Max_Speed						_Q15(MOTOR_SPEED_STAL_MAX_RPM/MOTOR_SPEED_BASE)


/* obsever parameter set value */
#define 	MAX_BEMF_VOLTAGE  							((MOTOR_SPEED_BASE*Ke)/(1000.0))
#define 	MAX_OMEG_RAD_SEC							((float)(_2PI*BASE_FREQ))
#define 	OBS_K1T 									_Q15(LD/(LD+RS*TPWM_VALUE))
#define 	OBS_K2T										_Q15((TPWM_VALUE/(LD+RS*TPWM_VALUE))*(HW_BOARD_VOLTAGE_BASE/HW_BOARD_CURR_BASE))
#define 	OBS_K3T										_Q15((TPWM_VALUE/(LD+RS*TPWM_VALUE))*(MAX_BEMF_VOLTAGE/HW_BOARD_CURR_BASE))
#define 	OBS_K4T										_Q15(((LD-LQ)*TPWM_VALUE*MAX_OMEG_RAD_SEC)/(LD+RS*TPWM_VALUE))

#define     OBSE_PLLKP_GAIN                     		_Q11(((2*ATT_COEF*_2PI*E_BW*LD - RS)*HW_BOARD_CURR_BASE)/HW_BOARD_VOLTAGE_BASE)
#define     OBSE_PLLKI_GAIN                     		_Q11((_2PI*E_BW*_2PI*E_BW*LD*TPWM_VALUE*HW_BOARD_CURR_BASE)/HW_BOARD_VOLTAGE_BASE)

#define 	OBSW_KP_GAIN 								_Q12(2*_2PI*ATT_COEF*ATO_BW/BASE_FREQ)
#define 	OBSW_KI_GAIN 								_Q12(_2PI*ATO_BW*ATO_BW*TPWM_VALUE/BASE_FREQ)

#define 	OBSW_KP_GAIN_RUN							_Q12(2*_2PI*ATT_COEF*ATO_BW_RUN/BASE_FREQ)
#define 	OBSW_KI_GAIN_RUN							_Q12(_2PI*ATO_BW_RUN*ATO_BW_RUN*TPWM_VALUE/BASE_FREQ)

#define 	OBSW_KP_GAIN_RUN1							_Q12(2*_2PI*ATT_COEF*ATO_BW_RUN1/BASE_FREQ)
#define 	OBSW_KI_GAIN_RUN1							_Q12(_2PI*ATO_BW_RUN1*ATO_BW_RUN1*TPWM_VALUE/BASE_FREQ)

#define 	OBSW_KP_GAIN_RUN2							_Q12(2*_2PI*ATT_COEF*ATO_BW_RUN2/BASE_FREQ)
#define 	OBSW_KI_GAIN_RUN2							_Q12(_2PI*ATO_BW_RUN2*ATO_BW_RUN2*TPWM_VALUE/BASE_FREQ)

#define 	OBSW_KP_GAIN_RUN3							_Q12(2*_2PI*ATT_COEF*ATO_BW_RUN3/BASE_FREQ)
#define 	OBSW_KI_GAIN_RUN3							_Q12(_2PI*ATO_BW_RUN3*ATO_BW_RUN3*TPWM_VALUE/BASE_FREQ)

#define 	OBSW_KP_GAIN_RUN4							_Q12(2*_2PI*ATT_COEF*ATO_BW_RUN4/BASE_FREQ)
#define 	OBSW_KI_GAIN_RUN4							_Q12(_2PI*ATO_BW_RUN4*ATO_BW_RUN4*TPWM_VALUE/BASE_FREQ)

#define 	OBSW_KP_GAIN_RUN5							_Q12(2*_2PI*ATT_COEF*ATO_BW_RUN5/BASE_FREQ)
#define 	OBSW_KI_GAIN_RUN5							_Q12(_2PI*ATO_BW_RUN5*ATO_BW_RUN5*TPWM_VALUE/BASE_FREQ)

#define 	OBSW_KP_GAIN_RUN6							_Q12(2*_2PI*ATT_COEF*ATO_BW_RUN6/BASE_FREQ)
#define 	OBSW_KI_GAIN_RUN6							_Q12(_2PI*ATO_BW_RUN6*ATO_BW_RUN6*TPWM_VALUE/BASE_FREQ)

#define 	OBSW_KP_GAIN_RUN7							_Q12(2*_2PI*ATT_COEF*ATO_BW_RUN7/BASE_FREQ)
#define 	OBSW_KI_GAIN_RUN7							_Q12(_2PI*ATO_BW_RUN7*ATO_BW_RUN7*TPWM_VALUE/BASE_FREQ)


#define		OBS_FBASE									BASE_FREQ*TPWM_VALUE*32768						// Fbase*Tpwm*32768
#define 	OBS_KLPF 									_Q15(_2PI*BASE_FREQ*TPWM_VALUE)					// 2PI*Fbase*Tpwm
#define 	SPEED_KLPF 									_Q15(_2PI*SPD_BW*TPWM_VALUE)					// 2PI*SPD_BW*Tpwm
#define 	OBS_EA_KS 									_Q15((2*MOTOR_SPEED_MIN_SMO*_2PI*BASE_FREQ*TPWM_VALUE)/MOTOR_SPEED_BASE)		// SMO����С�ٶ�

/*����ж�ʱ�Ĺ����㷨����ֵ------------------------------------------------------------*/
#define 	SPEED_KLPF_WIND 							_Q15(_2PI*SPD_BW_Wind*TPWM_VALUE)				// 2PI*SPD_BW_Wind*Tpwm
#define 	OBSW_KP_GAIN_WIND 							_Q12(2*_2PI*ATT_COEF*ATO_BW_Wind/BASE_FREQ)
#define 	OBSW_KI_GAIN_WIND							_Q12(_2PI*ATO_BW_Wind*ATO_BW_Wind*TPWM_VALUE/BASE_FREQ)

/*valpha mode*/
#define 	DC_Bus_Calc									(0)												// Valpha��Vbeta��DC_bus����
#define 	Ipark_Calc									(1)												// Valpha��Vbeta��Ipark����
#define 	Valpha_Mode									(DC_Bus_Calc)

/*Estimation Algorithm:SMO or PLL*/
#define 	SMO											(0)												// SMO ,��Ĥ����
#define 	PLL											(1)												// PLL ,���໷
#define 	EstimateAlgorithm							(SMO)

/*Current Calib:enable or disable*/
#define 	Disable										(0)												// 
#define 	Enable										(1)												// 
#define 	CalibENDIS									(Enable)
/*estimate omega mode*/
#define 	Sin_Theta									(0)												// sin��PI��ʽ����Ƕ�
#define 	Arctan_Theta								(1)												// arctan����Ƕ�
#define 	Estimate_Omega_Mode							(Arctan_Theta)

/*SVPWM mode*/
#define 	SVPWM_5_Segment								(0)												// ���ʽSVPWM
#define 	SVPWM_7_Segment								(1)												// �߶�ʽSVPWM
#define		SVPMW_Mode									(SVPWM_7_Segment)

/*double resistor sample mode*/
#define 	DouRes_1_Cycle								(0)												// 1 ���ڲ����� ia, ib
#define 	DouRes_2_Cycle								(1)												// �������ia, ib, 2���ڲ������
#define		DouRes_Sample_Mode							(DouRes_1_Cycle)
/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : Customer.h
* Author             : Fortiortech  Appliction Team
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the user interface parameter used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef		__CUSTOMER_H_
#define		__CUSTOMER_H_

/*оƬ����ֵ------------------------------------------------------------------*/
/*CPU and PWM Parameter*/
/* CPU���ز� Ƶ�� */
#define 	MCU_CLOCK							(24.0)							// MCU��Ƶ 24M
#define 	PWM_FREQUENCY						(20.0)							// �ز�Ƶ��(KHZ)

/*deadtime Parameter*/
/* ����ʱ����� */
#define 	PWM_DEADTIME						(0.8)							// ����ʱ��(us)

/*single resistor sample Parameter*/
#define 	MIN_WIND_TIME						(1.6)							// ��������С��������(us)

/*Power filter Parameter*/
#define 	POWER_KLQFP							_Q15(0.25)						// ���ʼ����ͨ�˲�ϵ��

///*�������ֵ------------------------------------------------------------------*/
///*motor Parameter*/
//#define	Pole_Pairs 			    				(2.0)				// ������
//#define 	RS               						(0.0945)			// ����� , ohm 	
//#define 	LD               						(0.000067)			// D������,H
//#define 	LQ               						(0.000067)			// Q������,H
//#define	Ke										(0.22)				// ���綯�Ƴ���

/*�������ֵ------------------------------------------------------------------*/
/*motor Parameter*/
#define		Pole_Pairs 			    			(2.0)							// ������
#define 	RS               					(0.01)							// �����,ohm
#define 	LD               					(0.0003)						// D������,H
#define 	LQ               					(0.0003)						// Q������,H
#define		Ke									(1.16)							// ���綯�Ƴ���

/*�����ת����---------------------------------------------------------------------*/
#define 	CW					 				(0)
#define 	CCW									(1)
#define		MOTOR_FR							(CCW)

/*Ӳ�����Ӳ�������ֵ------------------------------------------------------------------*/
/*hardware current sample Parameter*/
/*������׼�ĵ�·����*/
#define 	HW_RSHUNT							(0.01)			 				// �������裬 ohm
#define 	HW_AMPGAIN							(10.00)							// �˷ŷŴ���
#define 	HW_ADC_REF							(4.5)							// ADC�ο���ѹ

/*hardware voltage sample Parameter*/
/*ĸ�ߵ�ѹ������ѹ��·����*/
#define 	RV1		  							(15)  		 					// ��λKŷķ
#define 	RV2 		  						(0) 	   						// ��λKŷķ
#define 	RV3		  							(2) 	        				// ��λKŷķ��ʵ��PCB�ϵ�ֵ
#define 	RV									((RV1+RV2+RV3)/RV3)				// Max Voltage 468V
#define 	HW_BOARD_VOLT_MAX					(HW_ADC_REF*RV)					// 5*(R1+R2+R3)/R3 = 5*(330+300+6.8)/6.9 = 468

/*ʱ������ֵ------------------------------------------------------------*/
#define		Calib_Time							1000							// ��λ:�Σ�У���������̶�1000��
#define		Charge_Time							15								// ��λ:ms��Ԥ���ʱ��
#define		Align_Time							1								// ��λ:ms��Ԥ��λʱ��  10

/*��������ʱ�����㷨�Ĳ�������ֵ------------------------------------------------------------*/
/* obsever parameter value */
#define 	BASE_FREQ							((MOTOR_SPEED_BASE/60)*Pole_Pairs)// speed base value UINT HZ����׼Ƶ��
#define 	OBS_KSLIDE							_Q15(0.85)						// SMO�㷨��Ļ�Ĥ����ֵ
#define     E_BW                                (300.0)                         // PLL�㷨��ķ��綯���˲�ֵ


/*����ж�ʱ�Ĺ����㷨����ֵ------------------------------------------------------------*/
#define		TailWind_Time						250								// ˳�����ʱ�䣬��λ:ms 
#define 	ATO_BW_Wind							15.0							// observer bandwidth filter value������жϹ۲���������˲�ֵ������ֵΪ8.0-100.0   30
#define 	SPD_BW_Wind							40.0							// speed bandwidth filter value������ж��ٶȴ�����˲�ֵ������ֵΪ5.0-40.0		15

/**���˳��״̬�µ�KP��KI****/
#define 	DKP_TailWind						_Q12(2.0)						// ˳��� D�� ����ϵ�� KP	1.8
#define 	DKI_TailWind						_Q12(0.4)						// ˳��� D�� ����ϵ�� KI 	0.38
#define 	QKP_TailWind						_Q12(2.0)						// ˳��� Q�� ����ϵ�� KP	1.8
#define 	QKI_TailWind						_Q12(0.4)						// ˳��� Q�� ����ϵ�� KI	0.38

/*������������ֵ------------------------------------------------------------*/
/* current set value */
#define		I_Value(Curr_Value)					_Q15(((Curr_Value*HW_RSHUNT*HW_AMPGAIN)/(HW_ADC_REF)))   //A


/*********** IPD parameter ***********/
/******* ��ʼλ�ü����� **********/
#define		PosCheckEnable						(1)								// ��ʼλ��ʹ��
#define		AlignTestMode						(0)								// Ԥ��λ����ģʽ

#define		PWM_ON_Time							(200)							// us���������200us
#define		PWM_OFF_Time						(560)							// us ���������200us
#define		PWM_Brake_Time						(200)							// us ���������200us
#define		PWM_Duty							(0.7)							// �������0.6 


/***Ԥ��λ��Kp��Ki****/
#define		DKP_Alignment						_Q12(0.5)						// Ԥ��λ��KP
#define		DKI_Alignment						_Q12(0.05)						// Ԥ��λ��KI
#define		QKP_Alignment						_Q12(0.5)						// Ԥ��λ��KP
#define		QKI_Alignment						_Q12(0.05)						// Ԥ��λ��KI
#define		ID_Align_CURRENT					I_Value(0.0)					// D �ᶨλ����
#define		IQ_Align_CURRENT					I_Value(0.15)					// Q �ᶨλ���� 
#define		Align_Angle							(0.0)							// Ԥ��λ�Ƕ�

#define		ID_Start_CURRENT					I_Value(0.0)					// D ����������
#define		IQ_Start_CURRENT					I_Value(1.5)					// Q ���������� 0.7 

#define		ID_RUN_CURRENT						I_Value(0.0)					// D �����е���
#define		IQ_RUN_CURRENT						I_Value(0.3)					// Q �����е���

/********Omega�����Ĳ���**********/
#define		ATO_BW								2.0								// observer bandwidth filter value���۲���������˲�ֵ������ֵΪ8.0-100.0
#define		ATO_BW_RUN							3.0
#define		ATO_BW_RUN1							5.0
#define		ATO_BW_RUN2							9.0
#define		ATO_BW_RUN3							15.0
#define		ATO_BW_RUN4							24.0
#define		ATO_BW_RUN5							30.0
#define		ATO_BW_RUN6							50.0
#define		ATO_BW_RUN7							80.0

#define 	SPD_BW								15.0							// speed bandwidth filter value���ٶȴ�����˲�ֵ������ֵΪ5.0-40.0
#define 	ATT_COEF							0.85							// obeserver filter value������Ķ�

/*ת�ٲ���ֵ------------------------------------------------------------*/
/* motor run speed value */
#define 	MOTOR_SPEED_BASE					(6000.0)						// �ٶȻ�׼ RPM

/* motor start speed value */
//open �㷨��������
#define 	MOTOR_OPEN_ACC						(50.0)							// ǿ������������(ÿ�ز����ڼ�һ��)
#define 	MOTOR_OPEN_ACC_MIN					(1.0)							// ǿ�������ĳ�ʼ�ٶ�
#define		MOTOR_OPEN_ACC_CNT					(40.0)							// ǿ��������ִ�д���(MOTOR_OPEN_ACC_CNT*256)
#define     MOTOR_OPEN_ACC_CYCLE				(1)								// OPEN����ѭ���϶��Ĵ���

//OMEGA��������
#define		Motor_Omega_Ramp_ACC				(1.0)							// omega ����������	  1~20		 4
#define 	MOTOR_OMEGA_ACC_MIN					(100.0)							// omega ��������С�л�ת��(RPM)  36
#define 	MOTOR_OMEGA_ACC_END					(500.0)							// omega ����������ת��(RPM)		 128

/* motor loop control speed value */
#define 	MOTOR_LOOP_RPM						(400.0)							// ��mode 0��mode 1�л�ת��  �������ٶȣ�ֻӰ�������ٶȻ����������⻷����һ˲�䡣 300

/* motor run speed value */
//�������ʱ�����Сת�١���ת����ת��
#define 	MOTOR_SPEED_MIN_SMO					(1000.0)						// ������ ���綯�Ƶ�ͨ�˲�ϵ������Сֵ	 ������  ACC_END < MIN_SMO < MIN_RPM  400

#define 	MOTOR_SPEED_MIN_RPM					(250.0)							// 
#define 	MOTOR_SPEED_MAX_RPM					(3500.0)						// �������ת��(RPM)

#define 	MOTOR_SPEED_MIN_RPM					(500.0)							// 
#define 	MOTOR_SPEED_MAX_RPM					(25000.0)						// �������ת��(RPM)

#define   	MOTOR_NOLOAD_MIN_RPM				(MOTOR_SPEED_MIN_RPM + 50)

#define 	MOTOR_SPEED_LIMIT_RPM				(1500.0)	


#define 	MOTOR_SPEED_STAL_MAX_RPM			(6500.0)						// ��ת�������ת��(RPM)
#define 	MOTOR_SPEED_STAL_MIN_RPM			(30.0)							// ��ת������Сת��(RPM)

#define 	Motor_Max_Speed						_Q15(MOTOR_SPEED_MAX_RPM/MOTOR_SPEED_BASE)
#define 	Motor_Min_Speed						_Q15(MOTOR_SPEED_MIN_RPM/MOTOR_SPEED_BASE)

#define		Motor_NoLoad_Min_Speed				_Q15(MOTOR_NOLOAD_MIN_RPM/MOTOR_SPEED_BASE)

#define		NoLoad_Judge_Speed					_Q15(NOLOAD_JUDGE_SPEED/MOTOR_SPEED_BASE)

/******����������ػ�������------------------------------------------------------*/
/* motor ON/0FF value */
#define 	OFFPWMDuty							_Q15(0.02)						// �ػ�PWMռ�ձȣ�С�ڸ�ռ�ձȹػ�//�ػ�PWMռ�ձȣ�С�ڸ�ռ�ձ�ʱ�ػ�
#define 	OFFPWMDutyHigh						_Q15(1.0)						// �ػ�PWMռ�ձȣ����ڸ�ռ�ձȹػ�
#define 	ONPWMDuty							_Q15(0.05)						// ����PWMռ�ձȣ����ڸ�ռ�ձ�ʱ����

#define		MINPWMDuty							_Q15(0.05)						// �ٶ���������СPWMռ�ձ�
#define		MAXPWMDuty							_Q15(0.95)						// �ٶ����������PWMռ�ձ�
#define		SPEED_K								((float)(Motor_Max_Speed-Motor_Min_Speed)/(float)(MAXPWMDuty-MINPWMDuty)) //2498

/*******����ʱ�Ĳ���*****************/

/**��������������ֵ------------------------------------------------------------*/
/* D axis parameter set value */
/* D�� �������� */
#define 	DKP									_Q12(1.0)						// D axis KP					D�� ����ϵ��
#define 	DKI									_Q12(0.01)						// D axis KI					D�� ����ϵ��
#define 	DOUTMAX								_Q15(0.3)//0.4						// D axis output max value		D�� ����޷�  0.05
#define 	DOUTMIN								_Q15(-0.3)						// D axis output min value		D�� ��С�޷�

/* Q axis parameter set value */
/* Q�� �������� */
#define 	QKP									_Q12(1.0)						//Q�� ����ϵ��  0.2   0.08 
#define 	QKI									_Q12(0.01)						//Q�� ����ϵ��

#define 	QOUTMAX								_Q15(0.96)//0.91				//Q�� ����޷�	 0.95
#define 	QOUTMIN								_Q15(-0.96)						//Q�� ��С�޷�

/*�ٶȻ���������ֵ------------------------------------------------------------*/
#define		SpeedloopDeley_Time					500								// �ٶȻ���Ч�ӳ�ʱ�� ��λ:ms
#define		SPEED_LOOP_TIME						50								// �ٶȻ��������ڣ���λ��ms		


#define		DetectionDelay_Time					(1500)							// ����ʶ����ʱʱ�䣬��λ��ms
#define		NOLOAD_JUDGE_SPEED					(500)							// �����ж��ٶ�


#define 	SKP								_Q12(1.5)						//�ٶȻ� ����ϵ��KP	 (0.2  0.001  �޳���) (0.9,0.002 �����ȶ� �����ܴ�) (0.9,0.001  �����ȶ�)
#define 	SKI								_Q12(0.01)						//�ٶȻ� ����ϵ��KI   (0.5  0.0015  �����ȶ����޳���)



#define 	SOUTMAX								I_Value(6.0)					// speed loop output max value   �ٶȻ� ���  ����޷�
#define 	SOUTMIN								I_Value(0.001)					// speed loop output min value   �ٶȻ� ���  ��С�޷�



/*ģʽѡ������ֵ------------------------------------------------------------*/
/*TailWind mode*/
#define 	IPMtest								(0)								// IPM���Ի���MOS���ԣ�MCU����̶�ռ�ձ�
#define 	NormalRun							(1)								// ���������״̬������
#define		IPMState							(NormalRun)

/*Driver Mode:3P3N or 6N*/
#define 	Driver_6N							(0)								// FOC 6N mode��һ�������ڸ�ѹ
#define 	Driver_3P3N							(1)								// FOC 3P3N mode��һ��ʹ���ڵ�ѹ
#define 	DriverMode							(Driver_3P3N)

/*PWM high or low level Mode*/
#define 	High_Level							(0)								// �����ߵ�ƽ��Ч
#define 	Low_Level							(1)								// �����͵�ƽ��Ч
#define 	PWM_Level_Mode						(High_Level)

/*speed loop mode*/
#define 	OUTLoop_Disable						(0)								// ʹ���⻷
#define 	OUTLoop_Enable						(1)								// �ر��⻷
#define 	OUTLoop_Mode						(OUTLoop_Enable)

/*TailWind mode*/
#define 	NoTailWind							(0)								// �����˳���ж�
#define 	TailWind							(1)								// ���˳���ж�
#define		TailWind_Mode						(NoTailWind)

/*open loop mode*/
#define 	Open_Start							(0)								// ����ǿ������
#define 	Omega_Start							(1)								// Omega����
#define 	Open_Omega_Start					(2)								// �ȿ���������Omega����
#define		Open_Start_Mode						(Omega_Start)

/*sample resister mode*/
#define 	Single_Resistor						(0)								// ������
#define 	Double_Resistor						(1)								// ˫����
#define 	Shunt_Resistor_Mode					(Single_Resistor)

/*��������ֵ------------------------------------------------------------------*/
/* protect value */
#define		Hardware_FO_Protect					(1)								// Ӳ��FO��������ʹ�ܣ�������IPM��FO�����ĳ���
#define		Hardware_CMP_Protect				(2)								// Ӳ��CMP�ȽϹ�������ʹ�ܣ�������MOS��Ӧ�ó���
#define		Hardware_FO_CMP_Protect				(3)								// Ӳ��CMP�ȽϺ�FO����������ʹ��
#define		HardwareCurrent_Protect				(Hardware_CMP_Protect)			// Ӳ����������ʵ�ַ�ʽ����ѡ��һ
#define		OverSoftCurrentValue				I_Value(20.0)					// �������ֵ
#define		CurrentRecoverEnable				(1)								// ��������ʹ��λ, 0����ʹ�ܣ�1��ʹ��
#define		OverCurrentRecoverTime				(2000)							// ���������ָ�ʱ�䣬��λ5ms �� 2000*5ms = 10S

#define 	VoltageProtectEnable				(0)								// ��ѹ������0,��ʹ�ܣ�1��ʹ��
#define 	StartProtectEnable					(0)								// ����������0,��ʹ�ܣ�1��ʹ��
#define 	StallProtectEnable					(0)								// ��ת������0,��ʹ�ܣ�1��ʹ��
#define 	PhaseLossProtectEnable				(0)								// ȱ�ౣ����0,��ʹ�ܣ�1��ʹ��


#define		Over_Protect_Voltage 				(29.0)							// ֱ����ѹ��ѹ����ֵ��V
#define		Over_Recover_Vlotage				(27.0)							// ֱ����ѹ��ѹ�����ָ�ֵ��V
#define		Under_Protect_Voltage				(12.0)							// ֱ����ѹǷѹ����ֵ��V
#define		Under_Recover_Vlotage				(15.0)							// ֱ����ѹǷѹ�����ָ�ֵ��V

#define		PhaseLossCurrentValue				I_Value(0.2)					// PhaseLossCurrentValue��ȱ�����ֵ
#define		PhaseLossRecoverTime				(600)							// ȱ�ౣ��ʱ�䣬��λ 5ms

#define		StallCurrentValue1					I_Value(3.5)					// stall over current value����ת����ֵ

#define		Usart_Speed							(0)
#define		SREF_Speed							(1)
#define		SpeedMode							(SREF_Speed)


#define		ONOFFtestMode						(0)	


#define		StartON_Time						5000
#define		StartOFF_Time						10000


#endif

/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FocControl.c
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

/* Private variables ----------------------------------------------------------------------------*/
MotStaType mcState;
MotStaTim  MotorStateTime;
CurrentOffset mcCurOffset;	
TimeCnt Time;

/*---------------------------------------------------------------------------*/
/* Name		:	void MC_Control(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	���״̬��������������ʼ����Ԥ��硢˳������жϡ�Ԥ��λ�����������С����ϵ�
/*---------------------------------------------------------------------------*/
void MC_Control(void)
{
	switch(mcState)
	{
		case mcInit:                          // ��ʼ��״̬������mcCharge״̬
		{

			VariablesPreInit();                 // �����ر�����ʼ��
			FOC_Init();							 // FOC����س�ʼ��
			PI_Init();			                  // PI��ʼ��
			mcState =  mcCharge;                // ����mcCharge״̬
			Time.Charge_Count = Charge_Time;    // ����Ԥ���ʱ��
			break;
		}
		case mcCharge:                        // Ԥ���״̬��MCU����̶�Ƶ��ռ�ձȣ�Ԥ��������
		{	                                    // ����mcAlign��mcTailWind״̬������������һ��״̬����������IPM��MOS����

			Motor_Charge();                     // Ԥ�������
			#if (IPMState == IPMtest)	          // IPM���Ի���MOS���ԣ�MCU����̶�ռ�ձ�			
			{	   
				
			}
			#elif (IPMState == NormalRun)		          // ���������״̬������
			{
				if(Time.Charge_Count == 0)			     
				{	
				#if (TailWind_Mode == NoTailWind)	  // �����˳���жϳ���ֱ��Ԥ��λ				
						{	
							#if (PosCheckEnable)
							{	
									mcState 					 = mcPosiCheck;
									MotorStateTime.PosiCheckOneTime	= 0;	
									mcFocCtrl.mcPosCheckAngle		    = 0xffff;						
							}
							#else
							{
							    mcState                          = mcAlign;
							    Time.Count                       = Align_Time;	
							}
							#endif							
						}
				#elif (TailWind_Mode == TailWind)		// �������˳���жϳ���			
						{														
//									/*ɲ������-------------------------------*/
//									//���� ����˳������ʱ  ����ĸ�ߵ�ѹ����
//									FOC_SWDUTY                      = 0.99*FOC_ARR;   
//									FOC_CR1                         = 0x06;	 					
//									FOC_CMR |= 0x3F;  
//									MOE = 1;   													
//									Time.TailWind_Count											= 200;																				
							    mcState                         = mcTailWind;
							    Time.TailWind_Count             = 0;									
						}
							#endif	
				}	
			}
			#endif		
			break;
		}
		case mcTailWind:                            // ˳���������жϣ����ݿͻ�����ɵ��Լ��룬�жϽ����󣬸��ݵ�ǰת�ٽ���ɲ��Ԥ��λ״̬��ֱ������                        
		if(Time.TailWind_Count == 0)		 // Time.TailWind_Count������ɲ����ʱ
		{				
			Motor_TailWind();                // ��ʼ��˳���������
			TailWindDealwith();               // ˳��紦��
		}
		break;
		 case mcPosiCheck:	
		{

			#if (PosCheckEnable)
			{				
				// ���ú�����ȡ��ǰ�Ƕȣ�������ֵΪ0xffff���ʾλ�ü��δ��ɣ����򷵻�ֵΪ��ǰ�Ƕ�
				mcFocCtrl.mcPosCheckAngle = PosCheckGetAngle();
				if(mcFocCtrl.mcPosCheckAngle != 0xffff)
				{		
					mcState    = mcAlign;
					Time.Count = Align_Time;
					TailWindDetect.AntiFlag	=1;				
				}
			}
			#else
			{				
				// ��ʼλ�ü�ⲻʹ��ʱ��ʼ�Ƕ�ΪԤ��λ�Ƕ�
				mcFocCtrl.mcPosCheckAngle = Align_Angle;
				mcState     = mcAlign;
				Time.Count  = Align_Time;
				TailWindDetect.AntiFlag	=1;
			}	
			#endif

			break;
		}			
		case mcAlign:                           
		{

			Motor_Align();                       // Ԥ��λ���ú���

			#if (!AlignTestMode)                 // Ԥ��λʱ�������ֱ������
			{
				if(Time.Count == 0)
				{
					mcState = mcStart;	
					Time.Stop_Count  = 10000;
				}
				
				mcSpeedRamp.IncValue         = 15;  // �ٶȻ�����
				mcSpeedRamp.DecValue         = 60;  // �ٶȻ�����
				
			}
			#else
			{
				while(1);                           // �����ڳ�ʼλ�ü�������
			}
			#endif 						
			break;
		}
		case mcStart:                           // �����������õ���������������������󣬽�������״̬��
		{

			Motor_Open();                         // �������ú���
			break;
		}
		case mcRun:                             // ����״̬��������״̬�ĸ�����Ϊ0������mcStop״̬��
		{		

				if(mcSpeedRamp.TargetValue==0)
				{
					mcState = mcStop; 
					Time.Stop_Count  = 10000;
				}	
				
				mcSpeedRamp.IncValue              = 40;//
				mcSpeedRamp.DecValue              = 40;
				
			break;
		}
		case mcStop:                             
		{	

			if((mcFocCtrl.mcSpeedFlt<Motor_Min_Speed)||(Time.Stop_Count==0))// �������ͣ����������Motor_Min_Speedʱ���ر����
			{
				FOC_CR1 = 0x00;
				FOCEN	= Disable;																	
				mcState = mcwait;					
			}
			else if(mcSpeedRamp.TargetValue > 0)  // ����ʱ�ظ��źţ�����������״̬
			{
				mcState = mcRun;	
				mcFocCtrl.CtrlMode= 0;
				FOC_IQREF = IQ_RUN_CURRENT;								
			}
			break;
		}
		case mcwait:                               // �ر�������������źŲ���ʱ���������ʼ�������С�
		{

			
			FOC_CR1 = 0x00;
			FOCEN		= Disable;																				
			MOE			= Disable;
			if(mcSpeedRamp.FlagONOFF == 1)
			{
				mcState = mcInit;
			}
		}
		break;
		
		case mcFault:
			break;

		default:
			break;
	}
}

void GetCurrentOffset(void)
{
	SetBit(ADC_STA, ADCBSY, 1);		                           // ʹ��ADC		
	while(GetBit(ADC_STA, ADCBSY)	);
	#if (Shunt_Resistor_Mode == Double_Resistor)					   // ˫����ģʽ���ϵ���֤Ӳ����·ʱ����۲�mcCurOffset.IaOffset��mcCurOffset.IbOffset�Ƿ�Ϊ4096
	{
		mcCurOffset.IaOffsetSum+=((ADC0_DR& 0x0fff) << 3);			//����ΪQ15��ʽ
		mcCurOffset.IaOffset = mcCurOffset.IaOffsetSum >> 4;		//������Iaoffset = 32767 / 8 = 4096;
		mcCurOffset.IaOffsetSum -= mcCurOffset.IaOffset;				//������IaOffsetSum = 32767 - 4096 = 28671
		
		mcCurOffset.IbOffsetSum+=((ADC1_DR& 0x0fff) << 3);
		mcCurOffset.IbOffset = mcCurOffset.IbOffsetSum >> 4;
		mcCurOffset.IbOffsetSum -= mcCurOffset.IbOffset;
	}
	#elif (Shunt_Resistor_Mode == Single_Resistor)			     // ������ģʽ���ϵ���֤Ӳ����·ʱ����۲�mcCurOffset.IbusOffset�Ƿ�Ϊ4096	
	{
		mcCurOffset.IbusOffsetSum+=((ADC4_DR& 0x0fff) << 3);
		mcCurOffset.IbusOffset = mcCurOffset.IbusOffsetSum >> 4;
		mcCurOffset.IbusOffsetSum -= mcCurOffset.IbusOffset;		//ĸ�ߵ��������ϵ�ƫ�õ�ѹ
	}
	#endif	
	mcCurOffset.OffsetCount++;
	if(mcCurOffset.OffsetCount>mcCurOffset.OffsetTimes)
	{
		mcCurOffset.OffsetFlag=1;
	}
}
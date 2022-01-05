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

/*---------------------------------------------------------------------------*/
/* Name		:	void FOC_Init(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	mcInit״̬�£���FOC����ؼĴ�����������
/*---------------------------------------------------------------------------*/
void FOC_Init(void)
{
	/*deadtime set*/	
	/* ����ʱ������ */
	TIM0_DTR 			= PWM_LOAD_DEADTIME;				// Deadtime = 0x18/24MHZ = 1us
	
	/*enable FOC register set*/
	/* FOCʹ�� */
	FOCEN					= Enable;						// enable FOC module
	
	CLR(FOC_SR,FUIF);                 // ���FOC�жϱ�־λ
	SET(FOC_IER,FUIE);                // ʹ��FOC�ж�
		
	PFOC1 = 1;                        // �ж����ȼ�����Ϊ2�����ȼ�����FOӲ������
	PFOC0 = 0;
		
	/*set FOC register*/
	/* FOC�Ĵ��� ��ʼ������ */
	FOC_CR1 		= 0;			// clear FOC_CR1
	FOC_CR2 		= 0;			// clear FOC_CR2
	FOC_IDREF 		= 0;			// clear Id						IDREF�ο�ֵ��0 
	FOC_IQREF 		= 0;			// clear Iq						IQREF�ο�ֵ��0
	
	FOC_THETA 		= 0;			// clear theta					THETA�ο�ֵ��0
	FOC_RTHEACC 	= 0;			// ���º����ĳ�ʼ���ٶ�
	FOC_RTHESTEP 	= 0;			// 0 degree acce speed			���³�ʼ�ٶ���0
	FOC_RTHECNT 	= 0;			// acce time					���´�����0
	FOC_THECOMP 	= 0x0000;		// smo estimater angle comp		�ǶȲ���ֵ��0
	FOC_THECOR 		= 0x0003;		// Theta error compensate		�Ƕ��л�����ֵ Ϊ1���üĴ�����MSB��LSB��Ϊ1���� 1xxx xxxx xxxx xxx1

	FOC_ARR 		= PWM_VALUE_LOAD;   // set 16Khz carry frequency			����������ֵ�������ز����ڣ�Ƶ��)
	FOC_POWKLPF 	= POWER_KLQFP;		// set FOC power cacl filter value		��ͨ�˲�ϵ��
	
	/*current loop parameter set*/
	/* ������ PI������ �������� */
	FOC_DKP 			= DKP;			// D�� ����ϵ��
	FOC_DKI 			= DKI;			// D�� ����ϵ��
	FOC_DMAX 			= DOUTMAX;		// D�� ��� ����޷�
	FOC_DMIN 			= DOUTMIN;		// D�� ��� ��С�޷�
	
	FOC_QKP 			= QKP;			// Q�� ����ϵ��
	FOC_QKI 			= QKI;			// Q�� ����ϵ��
	FOC_QMAX 			= QOUTMAX;		// Q�� ��� ����޷�
	FOC_QMIN 			= QOUTMIN;		// Q�� ��� ��С�޷�
		
	/*estimate parameter set*/
	/* ������ �������� */
	FOC_EK1 			= OBS_K1T;		// ��������������� ��һ��ϵ��
	FOC_EK2 			= OBS_K2T;		// ��������������� �ڶ���ϵ��
	FOC_EK3 			= OBS_K3T;		// ��������������� ������ϵ��
	FOC_EK4 			= OBS_K4T;		// ��������������� ���ĸ�ϵ��
	
	FOC_FBASE 		= OBS_FBASE;		// ������ ��OMEGA����Ƕ�����DELTA THETA��ϵ��			
	FOC_OMEKLPF 	= SPEED_KLPF;		// ������ �ٶȼ���ĵ�ͨ�˲�ϵ��			
	FOC_EBMFK 		= OBS_KLPF;			// ������ ���綯�Ƶ�ͨ�˲���ϵ��KLPF��ϵ�� 			
	FOC_EKP 			= OBSW_KP_GAIN;	// ������ PI������ KPϵ��				
	FOC_EKI 			= OBSW_KI_GAIN;	// ������ PI������ KIϵ��

	SetBit(FOC_SET1,FOCFR,motor_FR);		// ����F/R

//	FocSpi_Init();                    //Foc��SPI��ʼ��,���ڹ۲�FOC��ı���	
	
	/*driver mode and active level set*/
	/* ����ģʽ �� ��Ч��ƽ ���� */
	#if (DriverMode == Driver_3P3N)				// FOC 3P3N mode��һ��ʹ���ڵ�ѹ
	{		
		SetBit(FOC_CMR,CCPH,0);					// �Ϲܣ�high level ��Ч
		SetBit(FOC_CMR,CCPL,0);					// �¹ܣ�high level ��Ч
	}
	#elif (DriverMode == Driver_6N)				// FOC 6N mode��һ�������ڸ�ѹ
	{
		#if (PWM_Level_Mode == High_Level)										
		{
			SetBit(FOC_CMR,CCPH,0);				// �Ϲܣ�high level ��Ч
			SetBit(FOC_CMR,CCPL,0);				// �¹ܣ�high level ��Ч		                               
		}
		#elif (PWM_Level_Mode == Low_Level)										
		{
			SetBit(FOC_CMR,CCPH,1);				// �Ϲܣ�low level ��Ч
			SetBit(FOC_CMR,CCPL,1);				// �¹ܣ�low level ��Ч
		}
		#endif
	}
	#endif
	
	/*Estimate Algorithm set*/
	/* ������ ģʽ����*/
	#if (EstimateAlgorithm == SMO)			// SMO  ��ģģʽ
	{
		FOC_KSLIDE    = OBS_KSLIDE;			// ������ KSLIDEϵ��
		FOC_EKLPFMIN	= OBS_EA_KS;		// ������ ���綯�Ƶ�Ͳ�˲�ϵ������Сֵ																		                              
	}
	#elif (EstimateAlgorithm == PLL)		// PLL  PLLģʽ		
	{		
		FOC_KSLIDE   	= 	OBSE_PLLKP_GAIN;	// PLL�� PI������ ��KPϵ��
		FOC_EKLPFMIN	= 	OBSE_PLLKI_GAIN;	// PLL�� PI������ ��KIϵ��																	
	}
	#endif

	/*estimate omega mode set*/
	#if (Estimate_Omega_Mode == Sin_Theta)			// sin��PI��ʽ����Ƕ�
	{
			SetBit(FOC_CR2,ESCMS,0);															
	}
	#elif (Estimate_Omega_Mode == Arctan_Theta)		// arctan����Ƕ�
	{
			SetBit(FOC_CR2,ESCMS,1);		
	}
	#endif
	
	/*valpha mode set*/
	#if (Valpha_Mode == DC_Bus_Calc)												//  Valpha��Vbeta��DC_bus����
	{
			SetBit(FOC_CR2,EIVMS,0);		
	}
	#elif (Valpha_Mode == Ipark_Calc)												//  Valpha��Vbeta��Ipark����
	{
			SetBit(FOC_CR2,EIVMS,1);
	}
	#endif	
	
	/*sample shunt resister mode set*/
	/*˫�������������Ҫ��С�������ڣ���Ϊ0�����½��ؽ���ǰ��ʼ����Ia������8009*/
	#if (Shunt_Resistor_Mode == Double_Resistor)		// double resistor sample
	{
		SetBit(FOC_CR1,CSM,1);							// ˫����
		FOC_TSMIN = 0;									// clear min windows time   �����С����ʱ��
		FOC_TRGDLY = 0x8003;							// ADC������ʱ�̣�FOC_TRGDLY[15]=0,�������䣻=1���½����䡣 8003Ϊ�½��ؽ���ǰ41.67ns*3=0.125us ��ʼ����Ia�� 
		                                                // 0001Ϊ�����ؿ�ʼʱ��ʼ�����������½��ؽ���ǰ����	
		/*double resister SVPWM 5/7 segment*/
		#if (SVPMW_Mode == SVPWM_7_Segment)
		{
			SetBit(FOC_CR2,F5SEG,0);					// 7��ʽSVPWM
		}
		#elif (SVPMW_Mode == SVPWM_5_Segment)
		{
			SetBit(FOC_CR2,F5SEG,1);					// 5��ʽSVPWM
		}
		#endif
		
		/*double resister sample period*/
		#if (DouRes_Sample_Mode == DouRes_1_Cycle)
		{
			SetBit(FOC_CR2,DSS,0);						// ˫����ģʽ��1���ڲ���
		}
		#elif (DouRes_Sample_Mode == DouRes_2_Cycle)
		{
			SetBit(FOC_CR2,DSS,1);						// ˫����ģʽ��2���ڲ���
		}
		#endif
	}
		/*�������������Ҫ��С������,FOC_TRGDLYΪ0���߶�ʽSVPWM��ʽ*/
	#elif (Shunt_Resistor_Mode == Single_Resistor)					// signel resistor sample 
	{
		SetBit(FOC_CR1,CSM,0);										// ������
		FOC_TSMIN = PWM_TS_LOAD;									// ��С��������
		FOC_TRGDLY = 7;	//-7											// �ɼ������ӳ�  +Ϊ�ͺ������-Ϊ��ǰ����
		SetBit(FOC_CR2,F5SEG,0);									// 7��ʽ
	}
	#endif	
	#if (CalibENDIS == Enable)	                            // ʹ�ܵ�����׼У��				
	{
		if(mcCurOffset.OffsetFlag==1)
		{
			#if (Shunt_Resistor_Mode == Double_Resistor)			// ˫����У��		
			{
						/*set ia, ib current sample offset*/
						SET(FOC_CHC,CSOC0);
						CLR(FOC_CHC,CSOC1);
				
						if (motor_FR	== CW)
						{
							FOC_CSO = mcCurOffset.IaOffset;
						}
						else			
						{
							FOC_CSO = mcCurOffset.IbOffset;  
						}

																														
						SET(FOC_CHC,CSOC1);
						CLR(FOC_CHC,CSOC0);
						if (motor_FR	== CW)
						{
							FOC_CSO = mcCurOffset.IbOffset;
						}
						else			
						{
							FOC_CSO = mcCurOffset.IaOffset;  
						}

				
						
				
			}
			#elif (Shunt_Resistor_Mode == Single_Resistor)	  // ������У��						
			{
						/*set ibus current sample offset*/
						SET(FOC_CHC,CSOC1);
						SET(FOC_CHC,CSOC0);
						FOC_CSO = mcCurOffset.IbusOffset;	              // д��IC��ƫ��        
			}
			#endif	
		}
	}
	#elif (Shunt_Resistor_Mode == Single_Resistor)					
	{
	}
	#endif	

	
	#if (PWM_Level_Mode == High_Level)										
	{                               
	}
	#elif (PWM_Level_Mode == Low_Level)										
	{
	  /*enable FD6536 output*/
		SetBit(P1_OE, P10, 1);	// config P10 as output
		SetBit(P1_PU, P10, 1);  // enable P10 pull up
		GP10 = 1;
	}
	#endif	

}
/*---------------------------------------------------------------------------*/
/* Name		:	void Motor_Charge(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	Ԥ��磬��һֱ����Ԥ���״̬�£����ӵ������������֤IPM����Mos
/*---------------------------------------------------------------------------*/
void Motor_Charge(void)
{
     if(MotorStateTime.ChargeOneTime==0)				// �״�Ԥ��� ��������
		 {																					
				MotorStateTime.ChargeOneTime = 1;		// ��������֤ Ԥ���ֻ����һ��		 
				FOC_SWDUTY = 0.7 * FOC_ARR;				// IPM 30% duty
				FOC_CR1 = 0x06;							// FOC������ʹ�ܣ����дPWMռ�ձ�
				Time.Charge_Count = Charge_Time;
		 }
		 if(Time.Charge_Count == Charge_Time) 	 // 3/3 Charge_Time
		 {
			 #if (IPMState == IPMtest)	 
			 {
			   FOC_CMR |= 0x03;                         // U�����		/* ͨ��1�����ſ��� */
			 }			 
			 #elif (IPMState == NormalRun)		          // ���������״̬������
			 {			 
			   FOC_CMR |= 0x01;                         // U�����ű�ͨ   /* ����ͨ��1���� */
			 }
			 #endif	
			 MOE = 1;																								
		 }
		 if(Time.Charge_Count == (Charge_Time<<1)/3) // 2/3 Charge_Time
		 {
			 #if (IPMState == IPMtest)	 
			 {
         FOC_CMR |= 0x0F;                         // U��V�����   /* ͨ��1��2�����ſ��� */
			 }			 
			 #elif (IPMState == NormalRun)		  // ���������״̬������
			 {
         FOC_CMR |= 0x05;                         // U��V�����ű۵�ͨ 		/* ͨ��1��2�����ſ��� */
			 }  
       #endif				 
		 }
		 if(Time.Charge_Count == Charge_Time/3) // 1/3 Charge_Time
		 {
			 #if (IPMState == IPMtest)	 
			 {
         FOC_CMR |= 0x3F;                         // U��V��W����� 		/* ͨ��1��2��3�����ſ��� */
	     }			 
			 #elif (IPMState == NormalRun)		   // ���������״̬������
			 {				 
			  FOC_CMR |= 0x15;                     // U��V��W�����ű۵�ͨ  	/* ͨ��1��2��3�����ſ��� */
			 }  
       #endif	
		 }
}
/*---------------------------------------------------------------------------*/
/* Name		:	void Motor_Align(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	Ԥ��λ��������������ж�ʱ������Ԥ��λ�̶���ʼλ��;��������ж�ʱ������Ԥ��λɲ��
/*---------------------------------------------------------------------------*/
void Motor_Align(void)
{
	if(MotorStateTime.AlginOneTime==0)		// �״�Ԥ��λ ��������
	{
		 MotorStateTime.AlginOneTime=1;		// ��������֤ Ԥ��λֻ����һ��
		
			/*enable FOC register set*/	
			FOCEN		= Disable;			// disable FOC Module	�ر� FOCģ��
			FOC_Init();
			FOC_CMR |= 0x3F;                // U��V��W�����ʹ�� ���������� �͵�ƽ��Ч 
			MOE = 1;						// �����UVW ��Դ��TIMER0/TIMER1/FOCģ��������TIM0�����Դ�� TIMER0 ͨ��4���T0_OC4
		
			FOC_IDREF = ID_Align_CURRENT;
			FOC_IQREF = IQ_Align_CURRENT ;			
			
			FOC_EKP 	= OBSW_KP_GAIN;					
			FOC_EKI 	= OBSW_KI_GAIN;	
			
			FOC_DKP = DKP_Alignment;	
			FOC_DKI = DKI_Alignment;
			FOC_QKP = QKP_Alignment;	
			FOC_QKI = QKI_Alignment;		
			
			FOC_RTHEACC 	= 0x0000;																	// ���º����ĳ�ʼ���ٶ�
			FOC_RTHESTEP 	= 0x0000;																	// 0.62 degree acce speed

			#if (AlignTestMode)
			{
				FOC_THETA		= _Q15((float)Align_Angle/180.0); 
			}
			#else
			{
				FOC_THETA 	= _Q15((float)mcFocCtrl.mcPosCheckAngle/180.0);// alginment angle
			}
			#endif
			
//			SetBit(FOC_CR1,EFAE,0);															    // ��ֹ������ǿ�����
//			SetBit(FOC_CR1,RFAE,1);															    // ʹ��ǿ��
			
			/*sample shunt resister mode set*/
			#if (Shunt_Resistor_Mode == Double_Resistor)						// ˫����
			{
				FOC_CR1 	= 0x0D;																			// ����ж��Ѿ�ʹ��FOC����																		                            
			}
			#elif (Shunt_Resistor_Mode == Single_Resistor)					// ������
			{
				FOC_CR1 	= 0x05;																		
			}
			#endif	
			/*Estimate Algorithm set*/
			#if (EstimateAlgorithm == SMO)					             	  // SMO
			{
				SetBit(FOC_CR1,ESEL,0);																		                              
			}
			#elif (EstimateAlgorithm == PLL)		                    // PLL		
			{
				SetBit(FOC_CR1,ESEL,1);																				
			}
			#endif		    
			
  }	
}

/*---------------------------------------------------------------------------*/
/* Name		:	void Motor_Open(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	���������Ĳ�������
/*---------------------------------------------------------------------------*/
void Motor_Open(void)
{
		static uint8 OpenRampCycles;	
    if(MotorStateTime.OpenOneTime==0)
		{
			MotorStateTime.OpenOneTime=1;
			
			SetBit(FOC_CR1,FCE,1);
			SetBit(FOC_CR1,PWMSS,0);
			
			#if (PosCheckEnable)
			{

				FOC_THETA 	= _Q15((float)mcFocCtrl.mcPosCheckAngle/180.0);//�г�ʼλ�ü�⣬���ó�ʼλ�ý�
			}
			#else
			{
				FOC_THETA		= _Q15((float)Align_Angle/180.0); //�޳�ʼλ�ü�⣬����Ԥ��λ��
			}
			#endif			

			if(TailWindDetect.TailWindBrakeCount>=4)	
			{
				FOC_IDREF = ID_Start_CURRENT;                         // D����������
				mcFocCtrl.mcIqref= IQ_Start_CURRENT+I_Value(0.4);	  // Q����������	
				FOC_IQREF = mcFocCtrl.mcIqref;		                    // Q����������
			}
			else if(TailWindDetect.TailWindBrakeCount>=1)	
			{
				FOC_IDREF = ID_Start_CURRENT;                         // D����������
				mcFocCtrl.mcIqref= IQ_Start_CURRENT+I_Value(0.3);		  // Q����������	
				FOC_IQREF = mcFocCtrl.mcIqref;		                    // Q����������
			}
			else
			{
				FOC_IDREF = ID_Start_CURRENT;                         // D����������
				mcFocCtrl.mcIqref= IQ_Start_CURRENT;		          // Q����������	
				FOC_IQREF = mcFocCtrl.mcIqref;		                  // Q����������
      }
			
			FOC_DKP = DKP;		//D�� ����ϵ��
			FOC_DKI = DKI;		//D�� ����ϵ��	
			FOC_QKP = QKP;		//Q�� ����ϵ��	
			FOC_QKI = QKI;		//Q�� ����ϵ��	
			
			FOC_EKP 	= OBSW_KP_GAIN;		//	�� ATO_BW �����ȣ�ֻ��Ķ� ATO_BW			
			FOC_EKI 	= OBSW_KI_GAIN;		//	�� ATO_BW �����ȣ�ֻ��Ķ� ATO_BW
					         									
			/*Omega mode set*/
			#if (Open_Start_Mode == Omega_Start)            //Omega ����
			{
				FOC_EFREQACC 	= Motor_Omega_Ramp_ACC;		// omega ����������	  1~20
				FOC_EFREQMIN 	= Motor_Omega_Ramp_Min;		// omega ��������С�л�ת��(RPM)
				FOC_EFREQHOLD   = Motor_Omega_Ramp_End;		// omega ����������ת��(RPM)
				
				SetBit(FOC_CR1,EFAE,1);															// ������ǿ�����
				SetBit(FOC_CR1,RFAE,0);															// ��ֹǿ��
				SetBit(FOC_CR1,ANGM,1);															// ����ģʽ                             
			}
			
			/*open mode set*/
			#elif (Open_Start_Mode == Open_Start)
			{
				FOC_RTHEACC 	= MOTOR_OPEN_ACC;								// ���º����ĳ�ʼ���ٶ�
				FOC_RTHESTEP 	= Motor_Open_Ramp_Min;								// 0.62 degree acce speed
				FOC_RTHECNT 	= MOTOR_OPEN_ACC_CNT;									// acce time			
				
				SetBit(FOC_CR1,EFAE,0);															// ��ֹ������ǿ�����
				SetBit(FOC_CR1,RFAE,1);															// ʹ��ǿ��
				SetBit(FOC_CR1,ANGM,1);															// ����ģʽ
			}
			#elif (Open_Start_Mode == Open_Omega_Start)
			{
				FOC_RTHEACC 	= MOTOR_OPEN_ACC;								// ���º����ĳ�ʼ���ٶ�
				FOC_RTHESTEP 	= Motor_Open_Ramp_Min;								// 0.62 degree acce speed
				FOC_RTHECNT 	= MOTOR_OPEN_ACC_CNT;									// acce time			
				
				FOC_EFREQACC 	= Motor_Omega_Ramp_ACC;
				FOC_EFREQMIN 	= Motor_Omega_Ramp_Min;
				FOC_EFREQHOLD = Motor_Omega_Ramp_End;	
				
				SetBit(FOC_CR1,EFAE,1);															// ��ֹ������ǿ�����
				SetBit(FOC_CR1,RFAE,1);															// ʹ��ǿ��
				SetBit(FOC_CR1,ANGM,1);															// ����ģʽ
			}
			#endif
	  }	
			//OPEN״̬����ʱ�϶����
		  #if (Open_Start_Mode == Open_Start)
			{
				if(OpenRampCycles<(MOTOR_OPEN_ACC_CYCLE-1))
				{
					if(!GetBit(FOC_CR1,RFAE))
					{				
						SetBit(FOC_CR1,RFAE,1);			
						OpenRampCycles++;	
					}
				}		
				else
				{			
					Time.Count = 2;	
			    FOC_EKP = OBSW_KP_GAIN_RUN7;	                        // ���������PI��KP			
				  FOC_EKI	= OBSW_KI_GAIN_RUN7;				                  // ���������PI��KI						
					mcState = mcRun;
				}	
			}
			#elif (Open_Start_Mode == Open_Omega_Start)
			{		
					Time.Count = 2600;					
					mcState = mcRun;
			}
			#elif (Open_Start_Mode == Omega_Start) 
			{			
				Time.Count = 2500;				 
				mcState = mcRun;			
			}			
			#endif
}
/*---------------------------------------------------------------------------*/
/* Name		:	void Motor_Align(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	˳�����������ú���
/*---------------------------------------------------------------------------*/
void TailWindDealwith(void)
{
	
		   // ��ת��ת�ټ������ʱ�����������򵱼����������1-3��3-1��������2��˵����ǰ�����ֹ
				if(((TailWindDetect.MotorDir == CW)&&(TailWindDetect.SpeedOverFlowStatus))||(TailWindDetect.TimerClearTimes>=3))
				{
					mcState 			 = mcPosiCheck;
					MotorStateTime.PosiCheckOneTime	= 0;
				
				}
				else if(TailWindDetect.MotorDir == CCW)
				{
														
					//����ɲ��MinBrakeTimes����ת�ٵ���BrakeStartSpeedʱǿ�ƿ�������
					
						FOC_SWDUTY                      = 0.99*FOC_ARR;   
						FOC_CR1                         = 0x06;	 				// enable break function			
						//���ݲ�ͬ�ٶ�����ɲ��ʱ��
						if(TailWindDetect.TailWindSpeed<50)//rpm/min
						{
							Time.TailWind_Count											= 300;
							 CCWstate = 1;
								
						}
						else if(TailWindDetect.TailWindSpeed<300)
						{
							Time.TailWind_Count											= 500;	
							 CCWstate = 2;
							
						}
						else if(TailWindDetect.TailWindSpeed<600)
						{
							Time.TailWind_Count											= 900;
							CCWstate = 3;							
						}
						else
						{
							Time.TailWind_Count											= 4000;	
							CCWstate = 4;
						}
						
						TailWindDetect.TailWindBrakeCount ++;							//ɲ�������ۼ�	
            if((TailWindDetect.TailWindBrakeCount>=4)&&(TailWindDetect.TailWindSpeed<100))	
						{	
							mcState 												= mcPosiCheck;
							MotorStateTime.PosiCheckOneTime	= 0;	
					 						
						}
						else if(TailWindDetect.TailWindBrakeCount>6)
						{
							 mcFaultSource=FaultOverwind;
					     mcState = mcFault;					
					     FaultProcess();		
							
						}
					MotorStateTime.TailWindOneTime 	= 0;
				}
				else if(TailWindDetect.MotorDir == CW)//��ת�����ٶȴ���һ���������������ж϶�����
				{	
					//����������TailWindDetect.SpeedStoreNum��ת��TailWindStartMinSpeed����ʱֱ��˳������
					if((TailWindDetect.TailWindSpeed >TailWindStartMinSpeed)&&(TailWindDetect.SpeedStoreNum > 3))
					{	
						FOCCloseLoopStart();		
				  							
					}
				}
	
}
/*---------------------------------------------------------------------------*/
/* Name		:	void Motor_Align(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	˳�����������ú���
/*---------------------------------------------------------------------------*/
void Motor_TailWind(void)
{
	if(MotorStateTime.TailWindOneTime ==0)
	{	
		MotorStateTime.TailWindOneTime=1;
		
		TailWindParInit();																		//˳��������ʼ��
		
		FOCEN		= Disable;																	  // FOCģ���ȹرպ����³�ʼ��
		FOC_Init();
		
		FOC_CMR |= 0x3F;                                      // U��V��W�����  
		MOE = 1;			
		
		FOC_IDREF = 0;                                        // D���������
		FOC_IQREF = 0;                                        // Q���������
	
		FOC_DKP = DKP_TailWind;	                              // D��KP
		FOC_DKI = DKI_TailWind;                               // D��KI
		FOC_QKP = QKP_TailWind;	                              // Q��KP
		FOC_QKI = QKI_TailWind;	                              // Q��KI
		
    FOC_OMEKLPF = SPEED_KLPF_WIND;	                      // ����������ٶȼ���ĵ�ͨ�˲�ֵ
		
		FOC_EKP = OBSW_KP_GAIN_WIND;	                        // ���������PI��KP			
		FOC_EKI	= OBSW_KI_GAIN_WIND;				                  // ���������PI��KI	
	
		FOC_CR2 |= 0x80;							                        //  ʹ�������
	
		/*sample shunt resister mode set*/
		#if (Shunt_Resistor_Mode == Double_Resistor)					
		{
			FOC_CR1 	= 0x0D;				                            // ʹ��˫���裬FOC������������FOC															                             
		}
		#elif (Shunt_Resistor_Mode == Single_Resistor)					
		{
			FOC_CR1 	= 0x05;						                        // ʹ�ܵ����裬FOC������������FOC															
		}
		#endif	
		
		/*Estimate Algorithm set*/
		#if (EstimateAlgorithm == SMO)					             	  // SMO
		{
			SetBit(FOC_CR1,ESEL,0);																		                              
		}
		#elif (EstimateAlgorithm == PLL)		                    // PLL		
		{
			SetBit(FOC_CR1,ESEL,1);	
			FOC_KSLIDE    = OBSE_PLLKP_GAIN;
			FOC_EKLPFMIN	= OBSE_PLLKI_GAIN;						

		}
		#endif	
	}	

}
/*---------------------------------------------------------------------------*/
/* Name		:	void MC_Stop(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	inital motor control parameter
/*---------------------------------------------------------------------------*/
void MC_Stop(void)
{
  FOC_SET0 &= 0x7F;																							// disable FOC and clear all register
	FOC_CR1 = 0x00;																					// disable FOC output and initial register
	mcState = mcInit;
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void MotorControlInit(void)
	Description   :	���Ʊ�����ʼ������,�������������ĳ�ʼ�������״̬��ʼ��
	Input         :	����˵������ϸ��
  Output				:	���˵������ϸ��
-------------------------------------------------------------------------------------------------*/
void MotorcontrolInit(void)
{
	led_t    =0;     //LED��˸
  key_trg  = 0;    //����������¼
  key_cont = 0;    //��������������¼
  keydelay_tim  = 0;   //��������ʱ���¼
  key_flag = 0;    //�������±�־λ
	beep_cnt = 0;
	key_data = 0;
  key_static = 0;
  keypres_static = 0;
  keyscanfiltertime = 0;
	
/***********����******************/
/*****��ѹ����*****/
	mcFaultDect.mcOverVoltDetecFaultCount  = 0;
	mcFaultDect.mcUnderVoltDetecFaultCount = 0;
	mcFaultDect.mcVoltRecoverCount      = 0;
	
  NoLoad_Flag = 1;	
	
	
/*******��������*****************/	
	mcFaultDect.Abs_ia				          = 0;
	mcFaultDect.Abs_ib				          = 0;
	mcFaultDect.Abs_ic			            = 0;
	mcFaultDect.OverCurrentValue        = OverSoftCurrentValue;
	mcFaultDect.OverCurCnt              = 0;  
	mcFaultDect.currenttime             = 0;
	mcFaultDect.CurrentRecoverCnt       = 0;

/*******��������*****************/		
	mcFaultDect.SecondStart             = 0;
	mcFaultDect.mcStartDelay            = 0;	
	mcFaultDect.mcStartCnt              = 0;	
	mcFaultDect.mcStartFocmode          = 0;
	mcFaultDect.mcStartSpeedCnt         = 0;
	mcFaultDect.mcStartEsCnt            = 0;
	mcFaultDect.StartFlag								= 0;
		
/******��ת����*********/	
	mcFaultDect.mcStallCount            = 0;	
	mcFaultDect.mcStallRecover          = 1000;//5s
	mcFaultDect.mcStallTimes            = 0;	
	mcFaultDect.mcStallDelaDectEs       = 0;
	mcFaultDect.mcStallDeSpeed          = 0;
	mcFaultDect.mcStallDelaDectSpd      = 0;
	mcFaultDect.mcStallDeCurrent        = 0;
	mcFaultDect.StallCurrentVale        = StallCurrentValue1;
	mcFaultDect.StallFlag               = 0;
	mcFaultDect.StallRecoverCurrent     = 2090;//��ת�����ָ�ֵ
	mcFaultDect.mcEsValue               = 0;	
	
/*******ȱ�ౣ��*****************/		
	mcFaultDect.LphaseDelayCount        = 0;
	mcFaultDect.Lphasecnt               = 0;
	mcFaultDect.LowCurrent              = I_Value(0.03);//ȱ������ж�����(�͵�������)
	mcFaultDect.NomCurrent              = I_Value(0.08);;//ȱ������ж�����(�ߵ�������)
	mcFaultDect.Max_ia                  = 0;
	mcFaultDect.Max_ia                  = 0;
	mcFaultDect.Max_ia                  = 0;
	mcFaultDect.AOpencnt                = 0;
	mcFaultDect.BOpencnt                = 0;
	mcFaultDect.COpencnt                = 0;
	mcFaultDect.mcLossPHCount           = 0;	
	mcFaultDect.mcLossPHTimes           = 0;
	
/*****��״̬������ʼ��******/	
	MotorStateTime.ChargeOneTime       = 0;
	MotorStateTime.AlginOneTime        = 0;
	MotorStateTime.TailWindOneTime     = 0;
	MotorStateTime.OpenOneTime         = 0;	

/*****�ⲿ���ƻ�*******/
  mcFocCtrl.CtrlMode                 = 0;
	mcFocCtrl.SpeedLoopTime            = 0;
	mcFocCtrl.PowerLoopTime            = 0;
	mcFocCtrl.RunCurrent               = 0;
	mcFocCtrl.mcIqref                  = 0;
	mcFocCtrl.TorqueLoopTime           = 0;
	mcFocCtrl.McuRef                   = 0;
	mcFocCtrl.Powerlpf                 = 0;


/******ADC�����˲�ֵ*********/
	AdcSampleValue.ADCSpeed            = 0;  
	AdcSampleValue.ADCVref             = 0;	
	
/******ĸ�ߵ�ѹ�˲�ֵ*******/
  mcFocCtrl.mcDcbusFlt	                       = 0;
	
/******�ֶδ���ı���*******/	
	segmentstate                       = 0;

/******״̬����������******/	
	Time.Charge_Count                  = 0;
  Time.TailWind_Count                = 0;
  Time.Break_Count                   = 0;
	Time.Stop_Count                    = 0;
	Time.Count                         = 0;
	Time.MotorRun_Count								 = 0;	
	Time.MODE_Flag  = 0;
	Time.START_Flag = 0;
/******����ƫ��У׼����*****/		
	mcCurOffset.IaOffsetSum            = 16383;
	mcCurOffset.IaOffset               = 0;
	mcCurOffset.IbOffsetSum            = 16383;
	mcCurOffset.IbOffset               = 0;
	mcCurOffset.IbusOffsetSum          = 16383;
	mcCurOffset.IbusOffset             = 0;
	mcCurOffset.OffsetCount            = 0;
	mcCurOffset.OffsetFlag             = 0;
	mcCurOffset.OffsetTimes            = Calib_Time;
	
	/*****LED����Ӧ***/	
	mcLedDisplay.LedCount             = 0;
	mcLedDisplay.LedTimCot            = 0;
	mcLedDisplay.Count                = 0;
	mcLedDisplay.Counttime            = 4999;	
	
	/*****�ٶȻ�����Ӧ***/	
	mcSpeedRamp.TargetValue           = 0;
	mcSpeedRamp.ActualValue           = 0;
	mcSpeedRamp.IncValue              = 60;
	mcSpeedRamp.DecValue              = 60;
	mcSpeedRamp.DelayCount            = 0;
	mcSpeedRamp.DelayPeriod           = 2;

  /*****ͨ��***/
	/**���յ�����**/	
	Uart.R_DATA[0]                     = 0;
	Uart.R_DATA[1]                     = 0;
	Uart.R_DATA[2]                     = 0;
	Uart.R_DATA[3]                     = 0;
  Uart.R_DATA[4]                     = 0;	
  Uart.R_DATA[5]                     = 0;
	Uart.R_DATA[6]                     = 0;
  Uart.R_DATA[7]                     = 0;
  Uart.R_DATA[8]                     = 0;	
  Uart.R_DATA[9]                     = 0;
	
	/**���͵�����******/
  Uart.T_DATA[0]                     = 0;
	Uart.T_DATA[1]                     = 0;
  Uart.T_DATA[2]                     = 0;
  Uart.T_DATA[3]                     = 0;
  Uart.T_DATA[4]                     = 0;	
  Uart.T_DATA[5]                     = 0;
	Uart.T_DATA[6]                     = 0;
  Uart.T_DATA[7]                     = 0;
  Uart.T_DATA[8]                     = 0;	
  Uart.T_DATA[9]                     = 0;
	
	Uart.Uredata                       = 0;
  Uart.UARxCnt                       = 0;
  Uart.RxFSM                         = 0;
  Uart.UsaRxLen                      = 0;
  Uart.flagUsaRxComm                 = 0;
  Uart.CheckDate                     = 0;
	Uart.ResponceCount                 = 0;
	Uart.ResponceFlag                  = 0;
	Uart.UsaTxlen                      = 0;
	Uart.SendCnt                       = 0;

  /**˯��ģʽ***/
	MotorSleepFlag                     = 0; 
	MotorSleepDelayCout                = 0;
	Sleep_En                           = 1;
	
	/***PWM����****/
	mcPwmInput.PWMFlag                 = 0;
	mcPwmInput.PWMCount                = 0;
			
  /*****˳����ж�*******/		
	TailWindDetect.TailWindBrakeCount  = 0;	
	TailWindDetect.AntiFlag            = 0;			
	mcFRState.FlagFR = 0;
	mcFRState.FRStatus = 0;	
	mcFRState.FR=0;
	
	
	/*****�Զ��������ʼ��*****/
	ONOFF_Times=0;
	ON_Count=0;
	ONOFF_Flag=1;
	OFF_Count=0;	
	
	CCWstate = 0;
		
	RecEsVlue = 0; 
}

/*---------------------------------------------------------------------------*/
/* Name		:	void VariablesPreInit(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	��ʼ���������
/*---------------------------------------------------------------------------*/
void VariablesPreInit(void)
{
	/***********����******************/
	mcFaultSource = 0;
	
		Time.MotorRun_Count								 = 0;	
	
	  NoLoad_Flag = 1;
	
  /*****��ѹ����*****/
	mcFaultDect.mcOverVoltDetecFaultCount  = 0;
	mcFaultDect.mcUnderVoltDetecFaultCount = 0;
	mcFaultDect.mcVoltRecoverCount      = 0;

/*******��������*****************/	
	mcFaultDect.Abs_ia				          = 0;
	mcFaultDect.Abs_ib				          = 0;
	mcFaultDect.Abs_ic			            = 0;
	mcFaultDect.OverCurrentValue        = OverSoftCurrentValue;
	mcFaultDect.OverCurCnt              = 0;  
	mcFaultDect.currenttime             = 0;
  mcFaultDect.CurrentRecoverCnt       = 0;
	mcFaultDect.StartFlag								= 0;
	
	
/******��ת����*********/	
	mcFaultDect.mcStallCount            = 0;	
	mcFaultDect.mcStallRecover          = 1000;//5s
	mcFaultDect.mcStallDelaDectEs       = 0;
	mcFaultDect.mcStallDeSpeed          = 0;
	mcFaultDect.mcStallDelaDectSpd      = 0;
	mcFaultDect.mcStallDeCurrent        = 0;
	mcFaultDect.StallCurrentVale        = StallCurrentValue1;
	mcFaultDect.StallFlag               = 0;
	mcFaultDect.StallRecoverCurrent     = 2090;
	mcFaultDect.mcEsValue               = 0;
	
/*******��������*****************/		
	mcFaultDect.mcStartDelay            = 0;	
	mcFaultDect.mcStartCnt              = 0;	
	mcFaultDect.mcStartFocmode          = 0;
	mcFaultDect.mcStartSpeedCnt         = 0;
	mcFaultDect.mcStartEsCnt            = 0;

/******ȱ�ౣ��*********/	
	mcFaultDect.LphaseDelayCount        = 0;
	mcFaultDect.Lphasecnt               = 0;
	mcFaultDect.LowCurrent              = I_Value(0.03);//ȱ������ж�����(�͵�������)
	mcFaultDect.NomCurrent              = I_Value(0.08);;//ȱ������ж�����(�ߵ�������)
	mcFaultDect.Max_ia                  = 0;
	mcFaultDect.Max_ia                  = 0;
	mcFaultDect.Max_ia                  = 0;
	mcFaultDect.AOpencnt                = 0;
	mcFaultDect.BOpencnt                = 0;
	mcFaultDect.COpencnt                = 0;
	mcFaultDect.mcLossPHCount           = 0;	
//	mcFaultDect.mcLossPHTimes           = 0;
	

/*****�ⲿ���ƻ�*******/
    mcFocCtrl.CtrlMode                 = 0;
	mcFocCtrl.SpeedLoopTime            = 0;
	mcFocCtrl.PowerLoopTime            = 0;
	mcFocCtrl.RunCurrent               = 0;
	mcFocCtrl.mcIqref                  = 0;	
	mcFocCtrl.TorqueLoopTime           = 0;
	
/*****˳����ж�*******/
	TailWindDetect.AntiFlag            = 0;		
	TailWindDetect.TailWindBrakeCount  = 0;
	
/*****���״̬��ʱ�����***********/	
	MotorStateTime.ChargeOneTime       = 0;
	MotorStateTime.AlginOneTime        = 0;
	MotorStateTime.TailWindOneTime     = 0;
	MotorStateTime.OpenOneTime         = 0;

	mcFocCtrl.mcSpeedFlt						   = 0;
	//memset(&ShowData,0,sizeof(OLEDShowData));
	ShowData.Ibus=0;
	ShowData.Vbus=0;
	ShowData.VSP=0;
	
	#if (PosCheckEnable)//��ʼλ�ü��ʹ��
	{	
			//��ʼλ�ü�����������ʼ��
		PosCheckPWMVar.PWMONTimeUp 			 = PWM_ON_Time_Up;
		PosCheckPWMVar.PWMONTimeDown 		 = PWM_ON_Time_Down;	
		PosCheckPWMVar.PWMOFFTimeInput 		 = PWM_OFF_Time_Input;
		PosCheckPWMVar.PWMBrakeTimeInput 	 = PWM_Brake_Time_Input;
		PosCheckPWMVar.PWMLOADInput       	 = PWM_VALUE_LOAD;
		PosCheckPWMVar.PWMONDutyInput 		 = (PWM_VALUE_LOAD*(1-(PWM_Duty)));
		PosCheckPWMVar.PWMOFFDutyInput 		 = (PWM_VALUE_LOAD*(PWM_Duty));		
	
		//���ݲ�ͬ��ѹ������ʼλ�ü������ռ�ձ�
//		if(mcDcbusFlt < _Q15(183.0/HW_BOARD_VOLT_MAX_Actual))		//130*1.414
//		{	
//			PosCheckPWMVar.PWMONDutyInput 			 = (PWM_VALUE_LOAD*(1-(PWM_Duty+0.16)));
//			PosCheckPWMVar.PWMOFFDutyInput 			 = (PWM_VALUE_LOAD*(PWM_Duty+0.16));					
//		}
//		else if(mcDcbusFlt < _Q15(254.0/HW_BOARD_VOLT_MAX_Actual))	//180*1.414
//		{
//			PosCheckPWMVar.PWMONDutyInput 			 = (PWM_VALUE_LOAD*(1-(PWM_Duty+0.08)));
//			PosCheckPWMVar.PWMOFFDutyInput 			 = (PWM_VALUE_LOAD*(PWM_Duty+0.08));		
//		}
//		else
//		{
//			PosCheckPWMVar.PWMONDutyInput 			 = (PWM_VALUE_LOAD*(1-(PWM_Duty)));
//			PosCheckPWMVar.PWMOFFDutyInput 			 = (PWM_VALUE_LOAD*(PWM_Duty));					
//		}		
	}
	#endif
}

#endif
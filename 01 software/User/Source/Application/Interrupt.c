/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : Interrupt.c
* Author             : Fortiortech  Appliction Team
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the interrupt function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>

/* Private variables ----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Name		:	void EXTERN_INT(void) interrupt 2
/* Input	:	NO
/* Output	:	NO
/* Description:	˯�߻���
/*---------------------------------------------------------------------------*/
void EXTERN_INT(void) interrupt 2   
{	
	if(MotorSleepFlag==1)
	{
		MotorSleepFlag=0;
		Sleep_En = 0;
	}
	CLR(P1IF, P11);                    // ����P10��־λ	
}
/*-------------------------------------------------------------------------------------------------
	Function Name :	void FO_INT(void)
	Description   :	FO_INT interrupt��Ӳ��FO�����������ض�������ж����ȼ����
	Input         :	��
  Output				:	��
-------------------------------------------------------------------------------------------------*/
void FO_INT(void) interrupt 1   	 	 // Ӳ��FO�����жϣ��ر����
{
	FaultProcess();	                   // �ر����
	mcFaultSource=FaultHardOVCurrent;  // Ӳ����������
	mcState = mcFault;                 // ״̬ΪmcFault
	IF0	= 0;						   // clear P00 interrupt flag
}
/*-------------------------------------------------------------------------------------------------
	Function Name :	void CMP_ISR(void)
	Description   :	CMP interrupt��Ӳ���Ƚ��������������ض�������ж����ȼ����
	Input         :	��
  Output				:	��
-------------------------------------------------------------------------------------------------*/
void CMP_ISR(void) interrupt 7
{

	
	if(GetBit(CMP_SR, CMP3INTR))
	{
		
		FaultProcess();	                   // �ر����
		mcFaultSource=FaultHardOVCurrent;  // Ӳ����������
		mcState = mcFault;                 // ״̬ΪmcFault
		CLR(CMP_SR, CMP3INTR);
	}
}
/*---------------------------------------------------------------------------*/
/* Name		:	void TIM0_INT(void) interrupt 4
/* Input	:	NO
/* Output	:	NO
/* Description:	���ڳ�ʼλ�ü��
/*---------------------------------------------------------------------------*/
void TIM0_INT(void) interrupt 4
{
		if(GetBit(TIM0_SR1, CC4IF))
	  {
      PosCheckCurrentGet();
		  CLR(TIM0_SR1, CC4IF);	
	  }
}

/*---------------------------------------------------------------------------*/
/* Name		:	void TIM23_INT(void) interrupt 9
/* Input	:	NO
/* Output	:	NO
/* Description:	Capture PWM ���ж����ȼ��ڶ�������FOC�жϣ�����PWM����
/*---------------------------------------------------------------------------*/
void TIM23_INT(void) interrupt 9
{
	CLR(TIM3_CR1,T3IR);
	if(GetBit(TIM3_CR1,T3IF))
	{
		if(GetBit(TIM3_CR1,T3IF))//����ж�	,366Hz
		{
			if(1==GP11)//PWM 100%���
			{
			  mcPwmInput.PWMCompare = 3300;
				mcPwmInput.PWMARR     = 3300;
			}
			else//PWM Ϊ0%
			{
				mcPwmInput.PWMCompare = 0;
				mcPwmInput.PWMARR     = 3300;
		}
		
			mcPwmInput.PWMFlag    = 1;
			mcPwmInput.PWMCount   = 0;
			CLR(TIM3_CR1,T3IF);					
		}				

	}	
}
/*---------------------------------------------------------------------------*/
/* Name		:	void FOC_INT(void) interrupt 3
/* Input	:	NO
/* Output	:	NO
/* Description:	FOC�жϣ�ÿ���ز�����ִ��һ�Σ����ڴ�����Ӧ�ϸߵĳ����ж����ȼ��ڶ���
/*---------------------------------------------------------------------------*/
void FOC_INT(void) interrupt 3
{
	if(GetBit(FOC_SR,FUIF))                                 // �����ж�
	{
		Fault_Overcurrent(&mcFaultDect);					// �����������	
		
		FGOutput();		// FG�ź����
		
	#if (BEMF_DETECT_ENABLE!=1)
	{
		TailWindSpeedDetect();
	}
	#endif
		CLR(FOC_SR,FUIF);                                   // �����־λ
	}
	if(GetBit(FOC_SR,FCIF))                                 // �Ƚ��ж�
	{
		CLR(FOC_SR, FCIF);	
	}
}

/*---------------------------------------------------------------------------*/
/* Name		:	void TIM45_INT(void) interrupt 10
/* Input	:	NO
/* Output	:	NO
/* Description:	1ms ��ʱ���жϣ����ڴ����ӹ��ܣ�����ƻ�·��Ӧ�����ֱ����ȡ��ж����ȼ�����FO�жϺ�FOC�жϡ�
/*---------------------------------------------------------------------------*/
void TIM45_INT(void) interrupt 10
{
	static uint8 i = 0;
	static uint16 key_count = 3000;
	
	if (Time.START_Flag == 0)
	{
		key_count--;
		if (key_count == 0)
		{
			Time.START_Flag = 1;//�ȴ�����
		}
	}
	
	if(GetBit(TIM4_CR1,T4IF))								// TIM4 overflow interrupt	
	{
		SET(ADC_STA,ADCBSY);                                //ʹ��ADC��DCBUS����
		/****�����˲�*****/
		if(mcState==mcRun)	
		{	
//		 mcFocCtrl.Powerlpf=LPF(FOC_POW,mcFocCtrl.Powerlpf,400);
			mcFocCtrl.Powerlpf=FOC_POW;
		}	                                

		/****�ٶ��˲�*****/
		if((mcState!= mcInit)&&(mcState!= mcwait))
		{
			mcFocCtrl.mcSpeedFlt=LPF(FOC_EOME,mcFocCtrl.mcSpeedFlt,400); //Ӳ���˲�
		}
		else
		{
			mcFocCtrl.mcSpeedFlt=0;
    }
	mcFocCtrl.mcDcbusFlt=LPF(ADC2_DR,mcFocCtrl.mcDcbusFlt,400);
	mcFocCtrl.mcIbusFlt=LPF(ADC4_DR,mcFocCtrl.mcIbusFlt,400);
	mcFocCtrl.Vspinput=ADC3_DR;
	i++;	
	if (i == 5)
	{
		i=0;
		OLED_P6x8Str(0,0,"mcSpeedFlt:");
		OLED_Print_Num1(66,0,mcFocCtrl.mcSpeedFlt/5.4);
		OLED_P6x8Str(0,1,"Powerlpf:");
		OLED_Print_Num1(66,1,mcFocCtrl.Powerlpf>>4);
		OLED_P6x8Str(0,2,"mcDcbusFlt:");
		OLED_Print_Num1(66,2,mcFocCtrl.mcDcbusFlt/9);
		OLED_P6x8Str(0,3,"mcIbusFlt:");
		OLED_Print_Num1(66,3,(mcFocCtrl.mcIbusFlt/9-225));
		OLED_P6x8Str(0,4,"Vspinput:");
		OLED_Print_Num1(66,4,4095-mcFocCtrl.Vspinput);
		OLED_P6x8Str(0,5,"motor_FR:");
		if (motor_FR == CW)
		{
			OLED_P6x8Str(66,5,"CCW");
		}
		else if (motor_FR == CCW)
		{
			OLED_P6x8Str(66,5,"CW ");
		}
		OLED_P6x8Str(0,6,"Count down:");
		OLED_Print_Num1(66,6,key_count);
	}
	/****��·��Ӧ�����ٶȻ������ʻ���ת�ػ�****/
		Speed_response();    

		/*****���ϱ����������ܣ����Ƿѹ����������������ȱ�ࡢ��ת��********/
		Fault_Detection();
		
		/***LED����ʾ****/
		LED_Display();
		/***led��˸**/
		led_flashing();
		key_scan();
	

	/**** ����-ֹͣ���� ****/
	
	#if (ONOFFtestMode)
	{
		if(mcState != mcFault)
			ONOFF_Starttest();
		else
			;
	}
	 #endif
		
		
	 /****ģ�����****/
	 #if (SpeedMode==SREF_Speed)	                    //ѡ�񴮿���Ϊ���Է�ʽ
	 {
		 
//		 		/******���ϵ�����У����������д�******/

//		 
		#if (!ONOFFtestMode)
		 {
//				 if(ONOFFPin == 1)
//				 {
//					 PWMInputCapture();
			//				VSPSample();				 
					 mcSpeedRamp.FlagONOFF = 1;		
//						mcSpeedRamp.TargetValue = _Q15(3000/MOTOR_SPEED_BASE)	;				 
						mcSpeedRamp.TargetValue = Motor_Max_Speed;//���ٶ� 
//				 }
		 }
		 #endif
//		mcFocCtrl.McuRef=9550;//�㹦��
//		mcSpeedRamp.TargetValue=9550;//�㹦��
	 }
	 #endif

	 /*****���״̬����ʱ����*****/	
	 
	 Time.MotorRun_Count++;
	 
		if(Time.Charge_Count > 0)      Time.Charge_Count--;
		if(Time.TailWind_Count  > 0)   Time.TailWind_Count--;
		if(Time.Break_Count  > 0)      Time.Break_Count--;
		if(Time.Count > 0)						 Time.Count--;
		if(Time.Stop_Count > 0)				 Time.Stop_Count--;	
		 
					
		//˳�������ٶ������޶�
		if(mcState == mcTailWind)
		{
			//1Hz Min
			if(TailWindDetect.SpeedCountTimer<TailWind_Time)//SpeedCountTimer����ʱ����
			{
				TailWindDetect.SpeedCountTimer++;
			}
			else
			{
				//���ʱ�ٶ�
				TailWindDetect.TailWindSpeed = ThailWindSpeedBase_zhouqi/TailWind_Time;	
				TailWindDetect.SpeedCountTimer = 0;	
				TailWindDetect.SpeedOverFlowStatus = 1; 				//ת�ټ��������־				
			}
		 }
		/*****����������º�������*****/	
		if((mcState == mcRun))
		{			
			if(Time.Count == 2500)
			{
				FOC_EKP = OBSW_KP_GAIN_RUN;	                        // ���������PI��KP			
				FOC_EKI	= OBSW_KI_GAIN_RUN;				                  // ���������PI��KI	
			}
			else if(Time.Count == 2000)
			{				
				FOC_EKP = OBSW_KP_GAIN_RUN1;	                        // ���������PI��KP			
				FOC_EKI	= OBSW_KI_GAIN_RUN1;				                  // ���������PI��KI	
			}
			else if(Time.Count == 1600)
			{
				FOC_EKP = OBSW_KP_GAIN_RUN2;	                        // ���������PI��KP			
				FOC_EKI	= OBSW_KI_GAIN_RUN2;				                  // ���������PI��KI	
			}
			else if(Time.Count ==1400)
			{
				FOC_EKP = OBSW_KP_GAIN_RUN3;	                        // ���������PI��KP			
				FOC_EKI	= OBSW_KI_GAIN_RUN3;				                  // ���������PI��KI	
			}
			else if(Time.Count ==1300)
			{
				FOC_EKP = OBSW_KP_GAIN_RUN4;	                        // ���������PI��KP			
				FOC_EKI	= OBSW_KI_GAIN_RUN4;				                  // ���������PI��KI	
			}
			else if(Time.Count == 1200)
			{
				FOC_EKP = OBSW_KP_GAIN_RUN5;	                        // ���������PI��KP			
				FOC_EKI	= OBSW_KI_GAIN_RUN5;				                  // ���������PI��KI	
			}
			else if(Time.Count == 600)
			{	
				FOC_EKP = OBSW_KP_GAIN_RUN6;	                        // ���������PI��KP			
				FOC_EKI	= OBSW_KI_GAIN_RUN6;				                  // ���������PI��KI	
			}
			else if(Time.Count == 1)
			{	
				FOC_EKP = OBSW_KP_GAIN_RUN7;	                        // ���������PI��KP			
				FOC_EKI	= OBSW_KI_GAIN_RUN7;				                  // ���������PI��KI	
			}
			else;
		}
		CLR(TIM4_CR1,T4IF);                                    // �����־λ
	}
}


/*---------------------------------------------------------------------------*/
/* Name		:	void USART_INT(void) interrupt 12
/* Input	:	NO
/* Output	:	NO
/* Description:	�����жϣ��ж����ȼ���ͣ����ڽ��յ����ź�,���жϲ���ʱ8us
/*---------------------------------------------------------------------------*/
void USART_INT(void)  interrupt 12
{
	if(TI==1)//��������жϱ�־��1
	{
		TI=0;	//��������жϱ�־λ����
   if(Uart.SendCnt<Uart.UsaTxlen)
		{
			Uart.SendCnt++;
			UART_SendData(Uart.T_DATA[Uart.SendCnt]);
    }
	}
	if(RI == 1)
	{
		RI = 0;
		Uart.Uredata= UT_DR;			//����������
		
    Uart.R_DATA[Uart.UARxCnt]=Uart.Uredata;
		if(Uart.R_DATA[0] ==0xAA)
		{
			MotorSleepDelayCout=0;
			Uart.UARxCnt ++ ;
      if(Uart.R_DATA[1] ==0x55)	
			{
				Uart.R_DATA[1] =0;
				Uart.UARxCnt = 0;
      }				
		}
		else
		{
			 Uart.UARxCnt = 0;
		}
		if(Uart.UARxCnt>=10)
		{
			Uart.UARxCnt=0;
		  Uart.ResponceFlag=1;
    }
  }
}







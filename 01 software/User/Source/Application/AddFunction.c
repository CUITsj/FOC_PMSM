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

/* Private variables ---------------------------------------------------------*/
FaultStateType		mcFaultSource;
PWMINPUTCAL   xdata mcPwmInput;
FOCCTRL       xdata mcFocCtrl;
FaultVarible  xdata mcFaultDect;
ADCSample           AdcSampleValue;
MCLedDisplay        mcLedDisplay;
MCRAMP       xdata  mcSpeedRamp;
int16               segmentstate;
uint16              led_t;
MotorFRTypeDef		mcFRState;
OLEDShowData		ShowData;

int16 				OMEGA_READ;
int16 				VSP;
uint16              StartCurrentRef;
uint8               MotorSleepFlag;
uint16              MotorSleepDelayCout;
uint8               Sleep_En;
uint16              SMO_EK2;


uint16              ON_Count;
uint16              OFF_Count;
uint32              ONOFF_Times;
uint8               ONOFF_Flag;

uint8				CCWstate;
uint16				RecEsVlue; 


uint16				SKI_RUN;
uint16				SKP_RUN;

uint8				NoLoad_Flag;

uint8				motor_FR;
uint8				key_trg;   //����������¼
uint8				key_cont;  //��������������¼
uint8				keydelay_tim;   //��������ʱ���¼
bit					key_flag;    //�������±�־λ
uint16				beep_cnt;
uint8				key_data;
uint16				key_static;
uint16				keypres_static;
uint16				keyscanfiltertime;

MotorTailWindTypeDef xdata  TailWindDetect;
const int16  PowerGiven[10] ={0,500,1000,2200,2900,4300,5900,7300,7900,8550};
/*---------------------------------------------------------------------------*/
/* Name		:	int16 KLPF_VALUE(int16 INVlaue, int16 OutLastValue)
/* Input	:	INVlaue��OutLastValue
/* Output	:	int16�ı���
/* Description:	�˲�����,�ó˷�������
/*---------------------------------------------------------------------------*/
int16 KLPF_VALUE(int16 INVlaue, int16 OutLastValue)
{	
	int16 Result = 0;
	MD_MA = (INVlaue-OutLastValue);
	MD_MB = (int16)480;		           			/*д�������ͳ���*/

	Result = MD_MB;
	Result += OutLastValue;
	return(Result);
}

/*---------------------------------------------------------------------------*/
/* Name		:	void FaultProcess(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	�������������ر�FOC�����ͬʱ��״̬��ΪmcFault
/*---------------------------------------------------------------------------*/
void FaultProcess(void)
{
	FOC_SET0 &= 0x7F;																				// disable FOC and clear all register
	FOC_CR1 = 0x00;																					// disable FOC output and initial register
	mcState = mcFault;
	MOE     = 0;	
}
/*---------------------------------------------------------------------------*/
/* Name		:	void TailWindParInit(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	��������ı�����ʼ��
/*---------------------------------------------------------------------------*/
void TailWindParInit(void)
{
	TailWindDetect.TailWindSpeed =0;
	TailWindDetect.MotorDir	= 0;
	TailWindDetect.AngleState =0;
	TailWindDetect.MotorTailWindState = NormalTailWind;
	TailWindDetect.SpeedCountTimer=0;
	
	TailWindDetect.SpeedStoreNum =0;	
	TailWindDetect.SpeedOverFlowStatus =0;
	
	TailWindDetect.SpeedTimerClearStatus = 0;
	TailWindDetect.TempThailWindSpeedBase = ThailWindSpeedBase;
	TailWindDetect.TimerClearTimes = 0;
}
/*---------------------------------------------------------------------------*/
/* Name		:	void TailWindSpeedDetect(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	����ж�
/*---------------------------------------------------------------------------*/
void TailWindSpeedDetect(void)
{
	static int16 	LatestTheta;
	
	if(mcState == mcTailWind)
	{	
			//˳�����������FOC�ڲ����ݵ�������Ƕȣ���ֱ�Ӷ�FOC_ETHETA���д���
			if(TailWindDetect.MotorTailWindState == NormalTailWind)
			{
				LatestTheta = FOC_ETHETA;
				
				//��ת�����ж��ж��� <-170�� �� <10�� �� -10�� �� 10�ȣ� 10��  ��  170��   ����״̬�л���ʱ��

				// -170��ʱ
				if(LatestTheta<-30946)
				{
					//������δ������״̬3����״̬1ʱ����
					if((TailWindDetect.SpeedTimerClearStatus==0)||(TailWindDetect.AngleState==3))
					{
						TailWindDetect.SpeedCountTimer =0;	
						TailWindDetect.SpeedTimerClearStatus = 1;
						if(TailWindDetect.AngleState==3)
						{
							TailWindDetect.TimerClearTimes ++;//����1��3֮�䶶��
						}
					}							
					//<-170��  ʱ����״̬Ϊ1��������SpeedCountTimer��TIM5�м�ʱ
					TailWindDetect.AngleState = 1;			
				}
				//�� -10�� �� 10�� ֮��
				else if((LatestTheta>-1820)&&(LatestTheta<1820))
				{
					//״̬1��״̬3�л���״̬2ʱ���浱ǰת��ʱ����TailWindDetect.SpeedCount[SpeedStoreNum]
					if((TailWindDetect.AngleState==1)||(TailWindDetect.AngleState==3))
					{
						//���㵱ǰת�٣�RPM
						TailWindDetect.SpeedCountTimer += 1;//��ֹΪ0
						TailWindDetect.TailWindSpeed = MDU_DIV_U16(&TailWindDetect.TempThailWindSpeedBase, &TailWindDetect.SpeedCountTimer);//����ٶ�
						TailWindDetect.SpeedStoreNum ++;
						
						if(TailWindDetect.SpeedTimerClearStatus == 1)//����SpeedCountTimer����
						{
							TailWindDetect.SpeedTimerClearStatus = 0;
						}
							
						//��1״̬�л���2״̬˵���Ƕȵ�����ת����ΪCW��3->2��ΪCCW
						if(TailWindDetect.AngleState==1)
						{
							TailWindDetect.MotorDir = CW;			
						}
						else if(TailWindDetect.AngleState==3)
						{
							TailWindDetect.MotorDir = CCW;
						}
						TailWindDetect.TimerClearTimes =0;//������ض����ļ���
					}
					TailWindDetect.AngleState = 2;	
										
				}
				//>170��ʱ
				else if(LatestTheta>30946)
				{
					//������δ������״̬1����״̬3ʱ����
					if((TailWindDetect.SpeedTimerClearStatus==0)||(TailWindDetect.AngleState==1))
					{
						TailWindDetect.SpeedCountTimer =0;	
						TailWindDetect.SpeedTimerClearStatus = 1;
						if(TailWindDetect.AngleState==1)
						{
							TailWindDetect.TimerClearTimes ++;//����1��3֮�䶶��
						}
					}				
					TailWindDetect.AngleState = 3;
				}
			}		
	}
}
/*---------------------------------------------------------------------------*/
/* Name		:	void FOCCloseLoopStart(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	�ջ�����
/*---------------------------------------------------------------------------*/
void FOCCloseLoopStart(void)
{
	#if (BEMF_DETECT_ENABLE)
	{
		//�ر��˷�
		SetBit(CMP_CR2, CMP0EN, 0);
		SetBit(CMP_CR2, CMP1EN, 0);
		SetBit(CMP_CR2, CMP2EN, 0);						

		FOCEN		= Disable;																	// enable FOC module
		FOC_Init();
		
		//�ؼ����
		FOC_CMR |= 0x3F;                         				// U��V��W�����  
		MOE = 1;	
		
		/*sample shunt resister mode set*/
		#if (Shunt_Resistor_Mode == Double_Resistor)			// ˫����
		{
			FOC_CR1 	= 0x0D;																			// ����ж��Ѿ�ʹ��FOC����																		                            
		}
		#elif (Shunt_Resistor_Mode == Single_Resistor)			// ������
		{
			FOC_CR1 	= 0x05;																		
		}
		#endif	
		/*Estimate Algorithm set*/
		#if (EstimateAlgorithm == SMO)					      // SMO
		{
			SetBit(FOC_CR1,ESEL,0);																		                              
		}
		#elif (EstimateAlgorithm == PLL)		          // PLL		
		{
			SetBit(FOC_CR1,ESEL,1);																				
		}
		#endif		    

		SetBit(FOC_CR1,FCE,1);
		SetBit(FOC_CR1,PWMSS,0);	
	}
	#endif
	
		FOC_EFREQACC              = Motor_Omega_Ramp_ACC;
		FOC_EFREQMIN 	            = Motor_Omega_Ramp_Min;
		FOC_EFREQHOLD             = Motor_Omega_Ramp_End;	

		SetBit(FOC_CR1,EFAE,1);															// ������ǿ�����
		SetBit(FOC_CR1,RFAE,0);															// ��ֹǿ��
		SetBit(FOC_CR1,ANGM,1);															// ����ģʽ  								

		//����������³�ʼ��
		/*current loop parameter set*/
		FOC_DKP 			= DKP;	
		FOC_DKI 			= DKI;
		FOC_DMAX 			= DOUTMAX;
		FOC_DMIN 			= DOUTMIN;

		FOC_QKP 			= QKP;
		FOC_QKI 			= QKI;
		FOC_QMAX 			= QOUTMAX;
		FOC_QMIN 			= QOUTMIN;
			
		/*estimate parameter set*/
		FOC_EK1 			= OBS_K1T;
		FOC_EK2 			= OBS_K2T;
		FOC_EK3 			= OBS_K3T;
		FOC_EK4 			= OBS_K4T;

		FOC_FBASE 		= OBS_FBASE;						
		FOC_OMEKLPF 	= SPEED_KLPF;					
		FOC_EBMFK 		= OBS_KLPF;						

		FOC_IDREF                       = ID_RUN_CURRENT;     // D����������
		mcFocCtrl.mcIqref				= IQ_RUN_CURRENT;
		FOC_IQREF                       = mcFocCtrl.mcIqref	;		  // Q����������	
		PI_UK 							=	mcFocCtrl.mcIqref	;
		

		TailWindDetect.AntiFlag             = 3;

		mcState                         = mcRun;
    mcFocCtrl.CtrlMode              = 0;
		
		#if (BEMF_DETECT_ENABLE)  //  ��������Ϊ˳��������
		{
				if(BEMFDetect.BEMFSpeed >_Q15(360.0/MOTOR_SPEED_BASE))
				{
					FOC_EKP 			= OBSW_KP_GAIN_RUN6;					
					FOC_EKI 			= OBSW_KI_GAIN_RUN6;	
					
					Time.Count = 10;	
				}
				else
				{
					FOC_EKP 			= OBSW_KP_GAIN_RUN3;					
					FOC_EKI 			= OBSW_KI_GAIN_RUN3;	
					
//					mcFocCtrl.mcIqref								= IQ_RUN_CURRENT;
//					FOC_IQREF                       = mcFocCtrl.mcIqref	;		  // Q����������	
//					PI_UK 													=	mcFocCtrl.mcIqref	;
					Time.Count = 1800;			
				}
		}
		
    #else	//  FOC״̬�۲� ��Ϊ˳��������
    {
			//���ݲ�ͬת��ȷ������ATO_BWֵ
			if(TailWindDetect.TailWindSpeed > 300)//300rpm/min
			{
				FOC_EKP 			= OBSW_KP_GAIN_RUN6;					
				FOC_EKI 			= OBSW_KI_GAIN_RUN6;	
				
				Time.Count = 10;		
			}
			else
			{
				FOC_EKP 			= OBSW_KP_GAIN_RUN3;					
				FOC_EKI 			= OBSW_KI_GAIN_RUN3;	
				
				mcFocCtrl.mcIqref								= (IQ_RUN_CURRENT<<1);
				FOC_IQREF                       = mcFocCtrl.mcIqref	;		  // Q����������	
				PI_UK 													=	mcFocCtrl.mcIqref	;
				Time.Count = 1800;						
			}	
    }		
		#endif
}

/*---------------------------------------------------------------------------*/
/* Name		:	int16 Abs_F16(int16 value)
/* Input	:	value
/* Output	:	int16
/* Description:	�Ա���ȡ16λ�ľ���ֵ
/*---------------------------------------------------------------------------*/
uint16 Abs_F16(int16 value)
{
	if(value < 0)
	{
		return (- value);
	}
	else
	{
		return (value);
	}
}
/*---------------------------------------------------------------------------*/
/* Name		:	int32 Abs_F32(int32 value)
/* Input	:	value
/* Output	:	int16
/* Description:	�Ա���ȡ16λ�ľ���ֵ
/*---------------------------------------------------------------------------*/
uint32 Abs_F32(int32 value)
{
	if(value < 0)
	{
		return (- value);
	}
	else
	{
		return (value);
	}
}

/*****************************************************************************
 * Function:		 void	Fault_OverVoltage(mcFaultVarible *h_Fault)
 * Description:	 ��ѹǷѹ��������������ÿ5ms�ж�һ�Σ�ĸ�ߵ�ѹ���ڹ�ѹ����ֵʱ����������һ��������ֵ����20�Σ��ж�Ϊ��ѹ�������ر����;��֮��������������
                 ͬ��Ƿѹ������
	               �����Ƿѹ����״̬�£�ĸ�ߵ�ѹ�ָ���Ƿѹ�ָ�ֵ���ϣ���ѹ�ָ�ֵ����ʱ����������һ������200�κ󣬻ָ������ݵ�λ��Ϣ�������ָ����ĸ�״̬��
 * Parameter:		 mcFaultVarible *h_Fault
 * Return:			 no
 *****************************************************************************/
void Fault_OverUnderVoltage(FaultVarible *h_Fault)
{
	//��ѹ����
		if(mcFaultSource == FaultNoSource)//����������������
		{
				if(mcFocCtrl.mcDcbusFlt > OVER_PROTECT_VALUE)	 //ĸ�ߵ�ѹ���ڹ�ѹ����ֵʱ������������20�Σ��ж�Ϊ��ѹ�������ر����;��֮��������������
				{
					h_Fault->mcOverVoltDetecFaultCount++;
					if(h_Fault->mcOverVoltDetecFaultCount > 20)//���100ms
					{
						h_Fault->mcOverVoltDetecFaultCount = 0;
						mcFaultSource=FaultOverVoltage;						
						FaultProcess();					
					}
				}
				else
				{
					if(h_Fault->mcOverVoltDetecFaultCount>0)
					{			
						h_Fault->mcOverVoltDetecFaultCount--;
					}
				}
				
			//Ƿѹ����
		
				if(mcFocCtrl.mcDcbusFlt< UNDER_PROTECT_VALUE)
				{
					h_Fault->mcUnderVoltDetecFaultCount++;	
					
					if(h_Fault->mcUnderVoltDetecFaultCount > 20)//���100ms
					{									
						h_Fault->mcUnderVoltDetecFaultCount = 0;
						mcFaultSource=FaultUnderVoltage;
						FaultProcess();
					}
				}		
				else
				{
					if(h_Fault->mcUnderVoltDetecFaultCount>0)
					{			
						h_Fault->mcUnderVoltDetecFaultCount--;
					}			
				}
		}
		
		/*******��ѹǷѹ�����ָ�*********/
		if((mcState == mcFault) &&((mcFaultSource==FaultUnderVoltage)||(mcFaultSource==FaultOverVoltage)))
		{
			if((mcFocCtrl.mcDcbusFlt< OVER_RECOVER_VALUE)&&(mcFocCtrl.mcDcbusFlt> UNDER_RECOVER_VALUE))									
			{
				h_Fault->mcVoltRecoverCount++;
				if(h_Fault->mcVoltRecoverCount>200)//�������1s����������ָ�
				{
          mcState = mcwait;
					mcFaultSource=FaultNoSource;
					h_Fault->mcVoltRecoverCount = 0;	
				}
			}
			else
			{
				h_Fault->mcVoltRecoverCount = 0;
			}	
	 }
}
/*****************************************************************************
 * Function:		 void	Fault_Overcurrent(mcFaultVarible *h_Fault)
 * Description:	 �������������������������κ�һ��ľ���ֵ������������ֵ��OverCurCnt��������1s�ڳ���3��ʱ��
	               �����ж�Ϊ����������������ֹͣ�������ϵ���ָܻ�����1s��OverCurCntС��3��ʱ��OverCurCnt���㡣
 * Parameter:		 mcFaultVarible *h_Fault
 * Return:			 no
 *****************************************************************************/
void Fault_Overcurrent(FaultVarible *h_Fault)
{
	if((mcState == mcRun)||(mcState == mcStart))						// check over current in rum and open mode
	{
		h_Fault->Abs_ia = Abs_F16(FOC_IA);
		h_Fault->Abs_ib = Abs_F16(FOC_IB);
		h_Fault->Abs_ic = Abs_F16(-FOC_IA-FOC_IB);
		if(h_Fault->Abs_ia> h_Fault->Max_ia)
		{
			 h_Fault->Max_ia = h_Fault->Abs_ia;
		}
	
		if(h_Fault->Abs_ib > h_Fault->Max_ib)
		{
			 h_Fault->Max_ib = h_Fault->Abs_ib;
		}
	
		if(h_Fault->Abs_ic > h_Fault->Max_ic)
		{
			 h_Fault->Max_ic = h_Fault->Abs_ic;
		}	
		
		h_Fault->currenttime++;
		if(h_Fault->currenttime>16000)//1s����
		{
			h_Fault->currenttime = 0;
			h_Fault->OverCurCnt= 0;
		}
		if((h_Fault->Abs_ia>=h_Fault->OverCurrentValue)||(h_Fault->Abs_ib>=h_Fault->OverCurrentValue)||(h_Fault->Abs_ic>=h_Fault->OverCurrentValue))
		{
			h_Fault->OverCurCnt++;
			if(h_Fault->OverCurCnt>=3)
			{
				h_Fault->Abs_ia=0;
				h_Fault->Abs_ib=0;
				h_Fault->Abs_ic=0;
				h_Fault->OverCurCnt=0;
			  mcFaultSource=FaultSoftOVCurrent;
			  FaultProcess();	
			}
		}		
//		if(mcFocCtrl.Powerlpf>11045)//�޹���
//		{		
//		 if(mcFocCtrl.mcIqref>=LIMIT_MIN_CURRENT)
//		 {
//			 mcFocCtrl.mcIqref--;
//			 PI_UK = mcFocCtrl.mcIqref;
//			 FOC_IQREF = mcFocCtrl.mcIqref;
//		 }
//		}	
//		mcFocCtrl.mcIqref=FOC_POW;
	}	
}
/*****************************************************************************
 * Function:		 void	Fault_OverCurrentRecover(mcFaultVarible *h_Fault)
 * Description:	 
 * Parameter:		 mcFaultVarible *h_Fault
 * Return:			 no
 *****************************************************************************/
void Fault_OverCurrentRecover(FaultVarible *h_Fault)
{
	if((mcState == mcFault)&&((mcFaultSource==FaultSoftOVCurrent)||(mcFaultSource==FaultHardOVCurrent)))
	{
		h_Fault->CurrentRecoverCnt++;
		if(h_Fault->CurrentRecoverCnt>=OverCurrentRecoverTime)//2000*5s=10s
		{
			h_Fault->CurrentRecoverCnt=0;
				
			mcState = mcwait;		
			mcFaultSource=FaultNoSource;
    }	
  }
}

/*****************************************************************************
 * Function:		 void	Fault_Start(mcFaultVarible *h_Fault)
 * Description:	 ��������������
								����һ���������״̬�£������ǰ5s����ת�ٴﵽ��ת����ֵ����5s�󷴵綯��ֵ̫��(�˷���δ��֤)
								��������
                ��������4s�ڻ���CtrlMode״̬�����ٶȵ���MOTOR_LOOP_RPM�������ж�Ϊ����ʧ�ܣ����ͣ����

                  �������ж�Ϊ����ʧ�ܺ��������������ڻ����5�Σ�������������У׼״̬���ȴ�������
 * Parameter:		 mcFaultVarible *h_Fault               
 * Return:			 no
 *****************************************************************************/
  void Fault_Start(FaultVarible *h_Fault)
  {
		/*******���������ָ�*********/
		h_Fault->mcEsValue = FOC_ESQU;
		
		 if((mcFaultSource==FaultStart)&&(mcState == mcFault)&&(h_Fault->SecondStart<=5))		//�����ָ�����
		 {						 
			 mcFaultSource=FaultNoSource;
			 mcState = mcwait;			   
		 }
		 
		if(mcState == mcRun)	
		{	
//			//����һ			5���ڵ���ٶȳ������ת�٣��ҷ��綯��ֵ�Ƚϵͣ� �ж�Ϊ ����ʧ��
//			if(h_Fault->mcStartSpeedCnt<=1000)		//5s
//			{
//			  h_Fault->mcStartSpeedCnt++;
//				if((mcSpeedFlt > Motor_Max_Speed)&&(h_Fault->mcEsValue<10))
//				{
//					h_Fault->mcStartSpeedCnt = 0;
//					mcFaultSource=FaultStart;
//					mcState = mcFault;					
//					FaultProcess();		
//					h_Fault->SecondStart++;	
//					
//					h_Fault->StartFlag = 1;		
//				}	
//				else
//				{
//					//	h_Fault->mcStartSpeedCnt = 0;
//				}
//			}
			
			
     //������		������� ��2�� �� ��6�� �ڣ����綯��ֵ���ͣ��ж�Ϊ ����ʧ��
			if(h_Fault->mcStartEsCnt<=1200) //1200 * 5 /1000 = 6S  
			{
				h_Fault->mcStartEsCnt++;
				h_Fault->mcStartDelay++;
				if(h_Fault->mcStartDelay>=400)	//400*5/1000=2s ��	 �ж�ʱ��
				{
					 h_Fault->mcStartDelay=400;
					
					 if(h_Fault->mcEsValue <3)	//���綯��ֵ���ڷ�Χ      �����ֵӦ�� ���ñȶ�ת��ֵ�� ����
						{	
							
							h_Fault->mcStartCnt++;
							if(h_Fault->mcStartCnt>=50) //������
							{							
								mcFaultSource=FaultStart;
								mcState = mcFault;
								
								FaultProcess();		
								h_Fault->SecondStart++;
								h_Fault->mcStartDelay=0;	
								h_Fault->mcStartCnt=0;	

								h_Fault->StartFlag = 2;								
							}						 
						}			
						else
						{
							h_Fault->mcStartCnt=0;
						}						
				 }
		 }
		 		  //������			2S������ģʽ0��ǿ�ϣ����ж�Ϊ ����ʧ��
			if(mcFocCtrl.CtrlMode==0)
			{
				h_Fault->mcStartFocmode++;
				if(h_Fault->mcStartFocmode>=400)//400*5/1000=2s
				{
					h_Fault->mcStartFocmode=0;
					mcFaultSource=FaultStart;
					mcState = mcFault;	
					
					FaultProcess();		
					h_Fault->SecondStart++;
					
					
					h_Fault->StartFlag = 3;					
				}
			}
	  }


	}
 /*****************************************************************************
 * Function:		 void	Fault_Stall(mcFaultVarible *h_Fault)
 * Description:	 ��ת���������������ֱ�����ʽ��
	               ��һ�֣�
	               �ڶ��֣��������״̬�£��ӳ�4s�жϣ������ٶȾ���ֵ������ת�ٶ�����5�Σ�
	               �����֣��������״̬�£���U,V�����������ֵ���ڶ�ת��������ֵ����6�Σ�
	               ���������ֵ��κ�һ�ֱ�������ʱ�����ͣ���������ж�Ϊ��ת������
	               ����ת����״̬�£�U��ɼ�ֵ���ڶ�ת�ָ�ֵʱ������ת����С�ڻ���ڶ�ת��������8�Σ�
	               �����ӳ�mcStallRecover��������������У׼״̬��
 * Parameter:		 mcFaultVarible *h_Fault
 * Return:			 no
 *****************************************************************************/
//��ת����
void Fault_Stall(FaultVarible *h_Fault)
{
		//method 1
	h_Fault->mcEsValue = FOC_ESQU;
	if(mcState == mcRun)
	{	
//method 2				
		if(h_Fault->mcStallDelaDectSpd <=1000)//5s
		{
			h_Fault->mcStallDelaDectSpd ++;
				
		}
		else
		{		
			
			
	    //method 1		
			if((h_Fault->mcEsValue < 3)||(FOC_EOME > Motor_Stall_Max_Speed)) // EsValue Ӧ���� ������ֵ��
			{		
				h_Fault->mcStallDelaDectEs++;
				if(h_Fault->mcStallDelaDectEs >= 30)
				{
					h_Fault->mcStallDelaDectEs=0;
					mcFaultSource=FaultStall;
					h_Fault->mcStallTimes++;
					FaultProcess();
					h_Fault->StallFlag  =  1;
				}
			}
			else
			{
				if(h_Fault->mcStallDelaDectEs>0)
				{
				 h_Fault->mcStallDelaDectEs--;
				}
			}
			
//			if((mcSpeedFlt<Motor_Stall_Min_Speed)||((FOC_EOME > _Q15(600.0/MOTOR_SPEED_BASE))&&(h_Fault->mcEsValue < 180)))
			if(mcFocCtrl.mcSpeedFlt<Motor_Stall_Min_Speed)
			{				 
				h_Fault->mcStallDeSpeed++;
				if(h_Fault->mcStallDeSpeed>=8)
				{	
					h_Fault->mcStallDeSpeed=0;
					mcFaultSource=FaultStall;
			    h_Fault->mcStallTimes++;					
					FaultProcess();			
					
					
 					h_Fault->StallFlag =2;
				}						 
			}			
			else
			{
				h_Fault->mcStallDeSpeed=0;	
			}	
			
		}		
//method 3		
		if((h_Fault->Abs_ia>=h_Fault->StallCurrentVale)||
			 (h_Fault->Abs_ib>=h_Fault->StallCurrentVale)||
		   (h_Fault->Abs_ic>=h_Fault->StallCurrentVale))
		{			 
			h_Fault->mcStallDeCurrent++;
			if(h_Fault->mcStallDeCurrent>=6)
			{
				h_Fault->mcStallDeCurrent=0;
				mcFaultSource=FaultStall;	
				h_Fault->mcStallTimes++;
				FaultProcess();									
 				h_Fault->StallFlag =3;
				
				
			}						 
		}	
		else if((h_Fault->Abs_ia<=(h_Fault->StallCurrentVale-I_Value(0.05)))&&
			 (h_Fault->Abs_ia<=(h_Fault->StallCurrentVale-I_Value(0.05))))
		{
			h_Fault->mcStallDeCurrent=0;
		}		
	}		 
		 /*******��ת�����ָ�*********/
	if((mcFaultSource==FaultStall)&&(mcState == mcFault)&&(h_Fault->mcStallTimes<=5))	//�ָ�����  	2017-10-31 ȥ��(ADC4_DR<h_Fault->StallRecoverCurrent)&&
	{
		h_Fault->mcStallCount++;			 
		if(h_Fault->mcStallCount>=h_Fault->mcStallRecover)
		{
			h_Fault->mcStallRecover=h_Fault->mcStallRecover;
			h_Fault->mcStallCount=16000;
			mcFaultSource=FaultNoSource;
 			mcState = mcwait;	
		}					 				 
	}
	else
	{
		h_Fault->mcStallCount=0;	
	}
}
 /*****************************************************************************
 * Function:		 void	Fault_phaseloss(mcFaultVarible *h_Fault)
 * Description:	 ȱ�ౣ�����������������״̬�£�10msȡ������������ֵ��
	               1.5s�жϸ���������ֵ���������������ֵ����һ��ֵ�������������ֵȴ�ǳ�С�����ж�Ϊȱ�ౣ�������ͣ����	               
 * Parameter:		 mcFaultVarible *h_Fault
 * Return:			 no
 *****************************************************************************/
  void Fault_phaseloss(FaultVarible *h_Fault)
  {
     if(mcState == mcRun)	
		   {
			   h_Fault->LphaseDelayCount++;
			   if(h_Fault->LphaseDelayCount>=2)//wait 1ms*5*2=10ms
			   {		
            h_Fault->LphaseDelayCount=0;					 
				
						h_Fault->Lphasecnt++;
						if(h_Fault->Lphasecnt>50)
							 {
								 h_Fault->Lphasecnt=0;
								 if(((h_Fault->Max_ia>(h_Fault->Max_ib*3))||(h_Fault->Max_ia>(h_Fault->Max_ic*3)))&&(h_Fault->Max_ia>PhaseLossCurrentValue))
//								 if(h_Fault->Max_ia < h_Fault->LowCurrent && h_Fault->Max_ib > h_Fault->NomCurrent && h_Fault->Max_ic > h_Fault->NomCurrent)
								 {
										h_Fault->AOpencnt++;
								 }
								 else
								 {
										h_Fault->AOpencnt = 0;
								 }
                 if(((h_Fault->Max_ib>(h_Fault->Max_ia*3))||(h_Fault->Max_ib>(h_Fault->Max_ic*3)))&&(h_Fault->Max_ib>PhaseLossCurrentValue))								 
//								 if(h_Fault->Max_ib < h_Fault->LowCurrent && h_Fault->Max_ia > h_Fault->NomCurrent && h_Fault->Max_ic > h_Fault->NomCurrent)
								 {
									 h_Fault->BOpencnt++;
								 }
								else
								 {
									 h_Fault->BOpencnt = 0;
								 }
								 if(((h_Fault->Max_ic>(h_Fault->Max_ia*3))||(h_Fault->Max_ic>(h_Fault->Max_ib*3)))&&(h_Fault->Max_ic>PhaseLossCurrentValue))
//								if(h_Fault->Max_ic < h_Fault->LowCurrent && h_Fault->Max_ib > h_Fault->NomCurrent && h_Fault->Max_ia > h_Fault->NomCurrent)
								 {
									 h_Fault->COpencnt++;
								 }
								else
								 {
										h_Fault->COpencnt = 0;
								 }
							 
								h_Fault->Max_ia = 0;
								h_Fault->Max_ib = 0;
								h_Fault->Max_ic = 0;
							 if(h_Fault->AOpencnt > 1|| h_Fault->BOpencnt > 1 || h_Fault->COpencnt > 1)
								 {
										h_Fault->mcLossPHTimes++;
										mcFaultSource=FaultLossPhase;
										mcState = mcFault;	
										FaultProcess();									 
								 }
							}
				 }
	   }	
		 /*******ȱ�ౣ���ָ�*********/
      if((mcFaultSource==FaultLossPhase)&&(mcState == mcFault)&&
			(h_Fault->mcLossPHTimes<0))//������5��
			 {
					 h_Fault->mcLossPHCount++;			 
					 if(h_Fault->mcLossPHCount>=PhaseLossRecoverTime)
					 {
						 h_Fault->AOpencnt=0;
						 h_Fault->BOpencnt=0;
						 h_Fault->COpencnt=0;					
						 // restart	
             mcState = mcwait;		
						 mcFaultSource=FaultNoSource;					
           }				 
       }
    else
			 {
			 h_Fault->mcLossPHCount=0;	
			 }			 

  }

/*---------------------------------------------------------------------------*/
/* Name		:	void Fault_Detection(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	�����������򱣻���ʱ����Ӧ����ܸߣ����÷ֶδ���ÿ5����ʱ���ж�ִ��һ�ζ�Ӧ�ı���
	              ���������й�Ƿѹ�����¡���ת��������ȱ��ȱ���������ʱ���ɸ�������һ�����ĵ��Լ��롣
/*---------------------------------------------------------------------------*/
void Fault_Detection(void)
{
	segmentstate++;
	if(segmentstate>=5)
	{
		segmentstate=0;
	}
	switch(segmentstate)
	{
		case 0:
		if(CurrentRecoverEnable) //���������ָ�
		{	
			Fault_OverCurrentRecover(&mcFaultDect);	
		}
		break;
		case 1:
		if(VoltageProtectEnable==1)//��ѹ����
		{					
			Fault_OverUnderVoltage(&mcFaultDect);
		}
		break;
		case 2:
		if(StartProtectEnable==1)//��������
		{	
		 
			Fault_Start(&mcFaultDect);
		}
		break;
		case 3:
		if(StallProtectEnable==1)//��ת����
		{
			Fault_Stall(&mcFaultDect);
		}
		break;
		case 4:
		if(PhaseLossProtectEnable==1)//ȱ�ౣ��
		{
			Fault_phaseloss(&mcFaultDect);
		}
		break;
		default:
		break;
	}
}
/*---------------------------------------------------------------------------*/
/* Name		:	void Speed_response(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	�ٶ���Ӧ�������ɸ������������ƻ������ת�ؿ��ơ���ת�ٿ��ơ��㹦�ʿ���
/*---------------------------------------------------------------------------*/
void Speed_response(void)
{
	if((mcState ==mcRun)||(mcState ==mcStop))       
	{
		switch(mcFocCtrl.CtrlMode)
		{
			case 0:
			 if(mcFocCtrl.mcSpeedFlt >Motor_Loop_Speed)        //������������������
			 {
					mcFocCtrl.CtrlMode= 1;
					mcFocCtrl.mcIqref=IQ_Start_CURRENT;  //�������ٶȣ�ֻӰ�������ٶȻ����������⻷����һ˲�䡣
					FOC_IQREF = IQ_Start_CURRENT;
					//mcSpeedRamp.ActualValue = FOC_EOME;  					 
					mcFocCtrl.TorqueLoopTime=0;	
					PI_UK = mcFocCtrl.mcIqref;
			 }					
			break;
			case 1:
			mcFocCtrl.TorqueLoopTime++;
			if(mcFocCtrl.TorqueLoopTime>5)
			{
				mcFocCtrl.TorqueLoopTime=0;
				#if (OUTLoop_Mode==OUTLoop_Disable)//���⻷������ɵ��ڵ�����������֤�����������͵��������ȶ���
				{	
					FOC_IQREF = IQ_RUN_CURRENT;								
				}
				#elif (OUTLoop_Mode==OUTLoop_Enable)//���⻷������ʵ��������ٶȻ������ʻ���������·��
				{
					//mc_ramp(&mcSpeedRamp);
					mcSpeedRamp.ActualValue = 19114 - mcFocCtrl.Vspinput*4.3;
					if (mcSpeedRamp.ActualValue > Motor_Max_Speed)
					{
						mcSpeedRamp.ActualValue = Motor_Max_Speed;
					}
					else if (mcSpeedRamp.ActualValue < Motor_Min_Speed)					
					{
						mcSpeedRamp.ActualValue = Motor_Min_Speed;
					}
					if (mcFocCtrl.Vspinput == 4095)
					{
						mcSpeedRamp.ActualValue = Motor_Min_Speed;
					}
					else if (mcFocCtrl.Vspinput == 0)
					{
						mcSpeedRamp.ActualValue = Motor_Max_Speed;
					}
					
					HW_Speed_PI();					
				}
				#endif
			}	
			break;
		}
	}
}

/*---------------------------------------------------------------------------*/
/* Name		:	 uint16 SoftLPF(uint16 Xn1, uint16 Xn0, uint16 K)
/* Input	:	uint16 Xn1, uint16 Xn0, uint16 K
/* Output	:	uint16
/* Description:	�����ͨ�˲�
/*---------------------------------------------------------------------------*/
 int16 SoftLPF(int16 Xn1, int16 Xn0, int16 K)
 {
 	int16 Temp16 = 0;
 	int32 Temp32 = 0;
 	
 	Temp32 = (((int32)Xn1 - (int32)Xn0) * (int32)K) >> 15;
 	Temp16 = Xn0 + (int16)Temp32;
 	return Temp16;
 }
 
 void Sleepmode(void)
 {
	 		MotorSleepDelayCout++;
		if(MotorSleepDelayCout>=20000)//���65530����Ҫ�ٴ������������
		{
			mcSpeedRamp.TargetValue=0;
			mcFocCtrl.McuRef =0;
			SetBit(P0_OE, P02, 0);
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
		  SetBit(PH_SEL,UT1EN,0);	
			REN = 0;						//�رմ��ڽ���
	    ES0 = 0;						//�ȹ��жϴ���	
			FOC_SET0 &= 0x7F;																				// disable FOC and clear all register
			FOC_CR1 = 0x00;																					// disable FOC output and initial register
			MOE     = 0;	
			MotorSleepDelayCout=0;
			MotorSleepFlag=1;
			SetBit(P1IE, P11, 1);   // config P11 as the source of EXTI1
			SetBit(PCON, STOP, 1);
		}			
 }
 
/*---------------------------------------------------------------------------*/
/* Name		:	void ONOFF_Starttest(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	��������
/*---------------------------------------------------------------------------*/
void ONOFF_Starttest(void)
{
	if(ONOFF_Flag==1)
	{
		mcSpeedRamp.FlagONOFF = 1;
		mcSpeedRamp.TargetValue =  Motor_Min_Speed;
		 ON_Count++;
		if(ON_Count>StartON_Time)
		{
			ON_Count=0;
			ONOFF_Times++;
			ONOFF_Flag=0;

			mcSpeedRamp.TargetValue = 0;
		}
	}
	else
	{
		 OFF_Count++;
		mcSpeedRamp.FlagONOFF = 0;
		mcSpeedRamp.TargetValue = 0;
		if(OFF_Count>StartOFF_Time)
		{
			OFF_Count=0;
			ONOFF_Flag=1;
			mcSpeedRamp.TargetValue = Motor_Min_Speed;
			
		}
		
  }
} 
 
/*---------------------------------------------------------------------------*/
/* Name		:	void LED_Display(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	LED����ʾ
/*---------------------------------------------------------------------------*/
void LED_Display(void)
{
	switch(mcFaultSource)
	{
    		case FaultNoSource:					
    		    ResetLEDPin;	    	//�͵�ƽ�������ߵ�ƽ��			
    		  break;
    	  case FaultStall:
				    Led_OnOff(&mcLedDisplay,1);								
				  break;
				case FaultLossPhase:
					  Led_OnOff(&mcLedDisplay,2);					
					break;	
    		case FaultSoftOVCurrent:
            Led_OnOff(&mcLedDisplay,3);	
    			break;
    		case FaultHardOVCurrent:
            Led_OnOff(&mcLedDisplay,4);				
    			break;
    		case FaultOverVoltage:
					  Led_OnOff(&mcLedDisplay,5);		
					break;
				case FaultUnderVoltage:
					  Led_OnOff(&mcLedDisplay,6);	
				  break;
    		case  FaultStart:
            Led_OnOff(&mcLedDisplay,7);	
    			break;
				case  FaultOverwind:
					  Led_OnOff(&mcLedDisplay,8);					
					break;					
    		default:
    			break;
  }

}

  //LED�Ƶ���˸
  void Led_OnOff(MCLedDisplay *hLedDisplay,uint8 htime)
  {	
		hLedDisplay->LedCount++;
	  if(hLedDisplay->LedCount<hLedDisplay->Counttime)
	   {  
		    if(hLedDisplay->Count<200)
	      {
				 hLedDisplay->Count++;
		 
	      }
				else if((hLedDisplay->Count>=200)&&(hLedDisplay->Count<201))
				{
					hLedDisplay->Count=0;
				  LEDPinONOFF;
					hLedDisplay->LedTimCot++;
				}
				else
					;
				if(hLedDisplay->LedTimCot>=2*htime)
		  	{
					hLedDisplay->Count=202;
				  SetLEDPin;
				}
      }
	    else if(hLedDisplay->LedCount>=hLedDisplay->Counttime)
	    {	
	     hLedDisplay->LedCount=0;
	     hLedDisplay->LedTimCot=0;
	     hLedDisplay->Count=0;
	    }
 }
/*---------------------------------------------------------------------------*/
/* Name		:	void mc_ramp(void)
/* Input	:	hTarget,MC_RAMP *hSpeedramp
/* Output	:	NO
/* Description:	
/*---------------------------------------------------------------------------*/		

void mc_ramp(MCRAMP *hSpeedramp)
{
	  if( --hSpeedramp->DelayCount < 0)
		{
				hSpeedramp->DelayCount = hSpeedramp->DelayPeriod;
			
				if (hSpeedramp->ActualValue < hSpeedramp->TargetValue)
				{
						if(hSpeedramp->ActualValue + hSpeedramp->IncValue < hSpeedramp->TargetValue)
						{
								hSpeedramp->ActualValue += hSpeedramp->IncValue;
						}
						else
						{
								hSpeedramp->ActualValue = hSpeedramp->TargetValue;
						}
				}
				else
				{
						if(hSpeedramp->ActualValue - hSpeedramp->DecValue > hSpeedramp->TargetValue)
						{

								hSpeedramp->ActualValue -= hSpeedramp->DecValue;
						}
						else
						{
								hSpeedramp->ActualValue = hSpeedramp->TargetValue;
						}	
				}
		}
}
void HW_Speed_PI(void)
{
	PI_EK =  mcSpeedRamp.ActualValue - mcFocCtrl.mcSpeedFlt;	//����ת����ʵ��ת��֮��
	PL_CR |= 0x02;									// Start PI
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_();
	_nop_(); 
	_nop_();
	mcFocCtrl.mcIqref= PI_UK;//�������޹����벻�޹���
	FOC_IQREF= PI_UK;
}
/*-------------------------------------------------------------------------------------------------
	Function Name :	uint16 LPF(uint16 Xn1, uint16 Xn0, uint16 K)
	Description   :	LFP����
	Input         :	Xn1--��ǰ����ֵ
	                Xn0--��һ���˲����ֵ
									K--LPF�˲�ϵ��
  Output				:	LPF_Y--��ǰ�˲����ֵ
-------------------------------------------------------------------------------------------------*/
uint16 LPF(uint16 Xn1, uint16 Xn0, uint16 K)
{	
	LPF_K = K;
	LPF_X = Xn1;
	LPF_Y = Xn0;	
	SetBit(PL_CR, LPFSTA, 1);
	_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();
	return LPF_Y;
}

/*---------------------------------------------------------------------------*/
/* Name		:	void PWMInputCapture(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	PWM�����źŲ�����,�����ٶȸ���
/*---------------------------------------------------------------------------*/
void PWMInputCapture(void)
{
	  uint32 Temp32;
	  uint16 MotorControlVSP;
	
		if(mcPwmInput.PWMFlag==0)//����ֵ�ͱȽ�ֵ
		{
			mcPwmInput.PWMCompare = TIM3_DR;
			mcPwmInput.PWMARR     = TIM3_ARR;
		}	
		else 
		{
			mcPwmInput.PWMCount++;
			if(mcPwmInput.PWMCount>=1600)//100ms��2Hz��һ��PWMFlag
			{
				mcPwmInput.PWMCount=0;
				mcPwmInput.PWMFlag=0;
			}		
		}
		
		if(Abs_F32(mcPwmInput.PWMCompare-mcPwmInput.PWMCompareOld)<1000)//���αȽ�ֵ���ʱ�����ٶ�����
		{	
				//�����˷���
				MDU_MUL_U32(mcPwmInput.PWMCompare, 32767, &Temp32);
				//����������
				mcPwmInput.PWMDuty = MDU_DIV_U16(&Temp32, &mcPwmInput.PWMARR);
			
			  MotorControlVSP=mcPwmInput.PWMDuty;
			
			 if((MotorControlVSP > ONPWMDuty)&&(MotorControlVSP <=OFFPWMDutyHigh))
				{
					mcSpeedRamp.FlagONOFF = 1;
				}
			 else if(MotorControlVSP < OFFPWMDuty)//||(MotorControlVSP >= OFFPWMDutyHigh))
				{
					mcSpeedRamp.FlagONOFF = 0;
				}	
			
				//ת�����߼���
				if(mcSpeedRamp.FlagONOFF==1)
				{
					
					if(NoLoad_Flag == 1)
					{
							if(MotorControlVSP <= MINPWMDuty)
							{
								mcSpeedRamp.TargetValue = Motor_Min_Speed;
							}
							else if(MotorControlVSP < MAXPWMDuty)
							{							
								mcSpeedRamp.TargetValue = Motor_Min_Speed + SPEED_K*(MotorControlVSP-MINPWMDuty);						
							}	
							else		
							{
								mcSpeedRamp.TargetValue	=	Motor_Max_Speed;
							}
					}
					else
					{
						
							if(MotorControlVSP <= MINPWMDuty)
							{
								mcSpeedRamp.TargetValue = Motor_NoLoad_Min_Speed;
							}
							else if(MotorControlVSP < MAXPWMDuty)
							{							
								mcSpeedRamp.TargetValue = Motor_NoLoad_Min_Speed + SPEED_K*(MotorControlVSP-MINPWMDuty);						
							}	
							else		
							{
								mcSpeedRamp.TargetValue	=	Motor_Max_Speed;
							}												
					}
			  }	
        else
				{
					mcSpeedRamp.TargetValue =0;
        }					
    }
		mcPwmInput.PWMCompareOld=mcPwmInput.PWMCompare;//���˴αȽ�ֵ��ֵ���ϴαȽ�ֵ
		mcPwmInput.PWMARROld=mcPwmInput.PWMARR;//���˴�����ֵ��ֵ���ϴ�����ֵ
	}
	
	/*---------------------------------------------------------------------------*/
/* Name		:	void FGOutput(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	FG�ź����
/*---------------------------------------------------------------------------*/
void FGOutput(void)
{
	if(mcState==mcRun)
	{
		if((FOC_THETA>=0)&&(FOC_THETA<32768))//0-180
		{
			ResetFGPin; 
		}
		else //180-360
		{
			SetFGPin;
		}
	
//		if((FOC_THETA>=0)&&(FOC_THETA<10922))//0-60
//		{
//			ResetFGPin; 
//		}
//		else if((FOC_THETA>=10922)&&(FOC_THETA<21845))//60-120
//		{
//			SetFGPin;
//		}
//		else if((FOC_THETA>=21845)&&(FOC_THETA<32768))//120-180
//		{
//			ResetFGPin; 
//		}
//		else if((FOC_THETA>=32768)&&(FOC_THETA<43690))//180-240
//		{
//			SetFGPin;
//		}
//		else if((FOC_THETA>=43690)&&(FOC_THETA<54613))//240-300
//		{
//			ResetFGPin; 
//		}
//		else if((FOC_THETA>=54613)&&(FOC_THETA<65536))//300-360
//		{
//		  SetFGPin;
//		}
  }
	else if(mcState == mcFault)
	{
			SetFGPin;
	}
	else
	{
			ResetFGPin; 
	}
}

/*---------------------------------------------------------------------------*/
/* Name		:	void VSPSample(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	VSP����
/*---------------------------------------------------------------------------*/
void VSPSample(void)
{
	/*****VREF�Ĳ�����ȡֵ���˲�******/
	AdcSampleValue.ADCVref = (ADC3_DR<<3);//�ɼ���Ӧ�˿�
	VSP=LPF(AdcSampleValue.ADCVref,VSP,1480);//��ͨ�˲�
			
	 if((VSP > ONPWMDuty)&&(VSP <= OFFPWMDutyHigh))//��ONPWMDuty-OFFPWMDutyHigh֮�䣬�����ת������
		{
			mcSpeedRamp.FlagONOFF = 1;
		}
	 else if((VSP < OFFPWMDuty)||(VSP > OFFPWMDutyHigh))//���ͣ��
		{
			mcSpeedRamp.FlagONOFF = 0;
		}		
		//ת�����߼���
		if(mcSpeedRamp.FlagONOFF==1)//  
		{
			if(VSP <= MINPWMDuty)    //��Сת������
			{
				mcSpeedRamp.TargetValue = Motor_Min_Speed;
			}
			else if(VSP < MAXPWMDuty)//����
			{							
				mcSpeedRamp.TargetValue = Motor_Min_Speed + SPEED_K*(VSP-MINPWMDuty);						
			}	
			else		     //���ת������
			{
				mcSpeedRamp.TargetValue	=	Motor_Max_Speed;
			}	
		}	
		else
		{
			mcSpeedRamp.TargetValue =0;
		}						
}
/*---------------------------------------------------------------------------*/
/* Name		:led_flashing(void
/* Input	:	NO
/* Output	:	NO
/* Description:	led��˸������
/*---------------------------------------------------------------------------*/

void led_flashing(void)//LED
{

	led_t++;
	if (led_t >= 20000)
	{
		GP37=1;
		led_t = 0;
  }
}
/*---------------------------------------------------------------------------*/
/* Name		:led_flashing(void
/* Input	:	NO
/* Output	:	NO
/* Description:	����ɨ�躯��
/*---------------------------------------------------------------------------*/
void key_scan(void)
{
////	uint8 key_data = 1;
//	key_data = GP06;//��ȡGP06
////	key_data &= 0x01;
////	key_trg = key_data&(key_data^key_cont);
////	key_cont = key_data;//key_contʼ�ո���key_data
//	
//	if(key_data == 0)
//	{
//		keydelay_tim++;
//		if(keydelay_tim > 100)
//		{
//			keydelay_tim = 0;
//			key_data = GP06;
//		  if(key_data == 0)
//			{
//		   key_flag = 1;
//      }				
//			
//    }
////		key_flag = 1;
//  }
//	else
//	{
//		key_flag = 0;
//  }
if (stop_pin)
  {
    if (key_static == 1)
		{ 
       keypres_static = 1;

    }
		key_static = 0;
			key_pro();
  }
else 
  {
	if(key_static!=1)
	  {
     keyscanfiltertime++;
			if(keyscanfiltertime > 100)
			{
				keyscanfiltertime = 0;
				key_static = 1;
      }
    }
  }
	
}
/*---------------------------------------------------------------------------*/
/* Name		:led_flashing(void
/* Input	:	NO
/* Output	:	NO
/* Description:	����������
/*---------------------------------------------------------------------------*/
void key_pro(void)
{
	if (keypres_static == 1)
	{
//		keypres_static = 0;
		beep_cnt++;
		GP10 = 1;
		GP37 = 0;
		if(beep_cnt > 500)
		{

			GP10 = 0;
			GP37 = 1;
			beep_cnt = 0;
			keypres_static = 0;
    }
	}
		
//else if (keypres_static ==0)
//		{
//		GP10 = 0;
//    GP37 = 1;			
//    }

}
/*---------------------------------------------------------------------------*/
/* Name		:led_flashing(void
/* Input	:	NO
/* Output	:	NO
/* Description:	��ʱ����
/*---------------------------------------------------------------------------*/
void beep_pro(void)
{
	;
}



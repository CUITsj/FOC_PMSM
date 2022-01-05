
#include <FTC6801.h>
#include <Myproject.h>
/* Private variables ----------------------------------------------------------------------------*/
MCUART Uart;

void UART_Init(void)
{
	SetBit(PH_SEL,UT1EN,1);		//p0[6]as UART_RXD; p0[5]as UART_TXD
	MOD1 = 0;
	MOD0 = 1;						//8bit�����ʿɱ�UARTģʽ
	SM2 = 0;						//��ֹMode2��Mode3���ͨѶ
	REN = 1;						//ʹ�ܽ���
	ES0 = 0;						//�ȹ��ж�		
	
	PUART1=0;           //�ж����ȼ�ʱ���
	PUART0=0;	
	
	UT_BAUD =0x9b;			//default baudrate:9600-0x9b,1200-0x4E1
	ES0 = 1;				    //����/�����ж�ʹ��	
}
void UartTxdate(uint16* sndBuf, int32 len)
{
	uint16 i=0;
	for(i=0;i<len;i++)
	{
	  UART_SendData(*sndBuf++);
	}
}
void UART_SendData(unsigned char T_Data)
{
	UT_DR = T_Data;
}
/***************�����ڽ��յ�������************/
void UartDealResponse(void)
{
	uint16 j=0;
	uint8 checkdate=0x00;	  
	if(Uart.ResponceFlag==1)
	{		
      if((Uart.R_DATA[0]==0xaa)&&(Uart.R_DATA[9]==0x55))				
			  for( j = 0; j < 8; j++ )
    	  {
    		 checkdate += Uart.R_DATA[j];
    	  }
        if(Uart.R_DATA[8] == checkdate)	
				{
				  if(mcFocCtrl.Vspinput>=10)
						mcFocCtrl.Vspinput=10;
					if(mcFocCtrl.Vspinput>=1)//1-9����Ӧת��
					{
						mcFocCtrl.McuRef =PowerGiven[mcFocCtrl.Vspinput];					
				    mcSpeedRamp.TargetValue=mcFocCtrl.Vspinput;//speed recived
					}
					else //ͣ����
					{
						mcFocCtrl.McuRef =0;
						mcSpeedRamp.TargetValue=0;
						if((mcFaultSource==FaultStart)||(mcFaultSource==FaultStall)||(mcFaultSource==FaultLossPhase))
						{
							mcFaultDect.mcStallTimes=0;
							mcFaultDect.mcLossPHTimes=0;
							mcFaultDect.SecondStart =0;
							mcFaultDect.AOpencnt=0;
							mcFaultDect.BOpencnt=0;
							mcFaultDect.COpencnt=0;	
							mcFaultSource=FaultNoSource;
							mcState = mcInit;																		// restart
							AdcSampleValue.ADCSpeed=0;
							mcFocCtrl.mcSpeedFlt=0;	
						}							
          }

				}
			}
} 
void UartDealComm(void)
{
		uint16 j=0;
	  uint16 checkdate=0x00;
	  int16   PowerFdb;
	 if(Uart.ResponceFlag==1)//10ms
	 {
		 UartDealResponse();
		 Uart.T_DATA[0] 	= 0xaa;	
		 PowerFdb         = 0;//(mcFocCtrl.Powerlpf>>6);
		 Uart.T_DATA[1] 	= mcState;//(uint16)(mcFocCtrl.Powerlpf>>6);//Q12, power feedback
		 Uart.T_DATA[2] 	= (uint16)(((int32)mcFocCtrl.mcSpeedFlt*2400)>>19);// actual speed/16 ,if actual speed=1000rpm/min,then TxBuf[2]=63
		 Uart.T_DATA[3] 	= (uint16)(((int32)mcFocCtrl.mcDcbusFlt*626)>>16);// DC bus feedback, 1 stand for 2V 
		 Uart.T_DATA[4] 	= mcFaultSource;//Fault state
		 Uart.T_DATA[5] 	= 0x00;
		 Uart.T_DATA[6] 	= 0x00;
		 Uart.T_DATA[7] 	= 0x00;			
			for( j = 0; j < 7; j++ )
			{
			 checkdate += Uart.T_DATA[j];
			}
		 Uart.T_DATA[8] 	= checkdate;
		 Uart.T_DATA[9] 	= 0x55;		 
			
	   Uart.UsaTxlen =9;
	   Uart.SendCnt=0;
	   UART_SendData(Uart.T_DATA[0]);	 

		 Uart.ResponceFlag=0;
	 }
}
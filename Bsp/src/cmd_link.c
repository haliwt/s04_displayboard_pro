#include "cmd_link.h"
#include "usart.h"
#include "gpio.h"
#include "run.h"
#include "display.h"
#include "led.h"

volatile static uint8_t transOngoingFlag; //interrupt Transmit flag bit , 1---stop,0--run
uint8_t outputBuf[8];
static uint8_t transferSize;
uint8_t inputBuf[MAX_BUFFER_SIZE];



/****************************************************************************************************
**
*Function Name:static void selectLight(uint8_t index)
*Function: UART2 transmit interrupt process ---4D 58 4C 53 32 30 32 
*Input Ref: LED number 
*Return Ref:NO
*
****************************************************************************************************/
void SendData_PowerOff(uint8_t index)
{
	
   //crc=0x55;
	outputBuf[0]='T'; //4D //Touch Key
	outputBuf[1]='K'; //58
	outputBuf[2]='P'; //4C	// 'P' power
	outputBuf[3]=index; //53	// 'S' select light command, 'C' close all light command
	
    transferSize=4;
	if(transferSize)
	{
		while(transOngoingFlag);
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
	
}
void SendData_Buzzer(void)
{
	
   //crc=0x55;
	outputBuf[0]='T'; //4D
	outputBuf[1]='K'; //58
	outputBuf[2]='Z'; //4C	// buzzer
	outputBuf[3]='Z'; //53	// buzzer
	
    transferSize=4;
	if(transferSize)
	{
		while(transOngoingFlag);
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
	
}
/*********************************************************
 * 
 * Function Name:void SendData_Temp_Data(uint8_t tdata)
 * Function:send 
 * 
*********************************************************/
void SendData_Set_Wifi(uint8_t hdata)
{
	    outputBuf[0]='T'; //
		outputBuf[1]='K'; //58
		outputBuf[2]='W'; //W ->WIFI
		outputBuf[3]=hdata; 
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}
	
}
/*********************************************************
 * 
 * Function Name:void SendData_Temp_Data(uint8_t tdata)
 * Function:send temperature value 
 * 
*********************************************************/
void SendData_Set_Command(uint8_t hdata)

{
	    outputBuf[0]='T'; //4D
		outputBuf[1]='K'; //58
		outputBuf[2]='C'; //"C" ->command
		outputBuf[3]=hdata; //53	//
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}
	
}
/*********************************************************
 * 
 * Function Name:void SendData_Temp_Data(uint8_t tdata)
 * Function:send temperature value 
 * 
*********************************************************/
void SendData_Temp_Data(uint8_t tdata)
{

        outputBuf[0]='T'; //4D
		outputBuf[1]='K'; //58
		outputBuf[2]='M'; //"T"->temperature
		outputBuf[3]=tdata; //53	//
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}
/*********************************************************
 * 
 * Function Name:void SendData_Time_Data(uint8_t tdata)
 * Function:send to set  timer timing value
 * 
*********************************************************/
void SendData_Time_Data(uint8_t tdata)
{

        outputBuf[0]='T'; //4D
		outputBuf[1]='K'; //58
		outputBuf[2]='T'; //"T"->timer timing
		outputBuf[3]=tdata; //53	//
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}
/*********************************************************
 * 
 * Function Name:void SendData_Temp_Data(uint8_t tdata)
 * Function:send temperature value 
 * 
*********************************************************/
void SendData_Works_Time(uint8_t tdata,uint8_t tdata_2)
{

        outputBuf[0]='T'; //4D
		outputBuf[1]='K'; //"T"->temperature
		outputBuf[2]=tdata; //53	//
		outputBuf[3]=tdata_2;
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}


void SendData_Remaining_Time(uint8_t tdata,uint8_t tdata_2)
{

        outputBuf[0]='T'; //4D
		outputBuf[1]='R'; //"T"->temperature
		outputBuf[2]=tdata; //53	//
		outputBuf[3]=tdata_2;
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}



/********************************************************************************
**
*Function Name:void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
*Function :UART callback function  for UART interrupt for transmit data
*Input Ref: structure UART_HandleTypeDef pointer
*Return Ref:NO
*
*******************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart==&huart1)
	{
		transOngoingFlag=0; //UART Transmit interrupt flag =0 ,RUN
	}
	
}


#include "bsp.h"

static void SendFrame(uint8_t *frame, uint8_t length) ;
static uint8_t PrepareFrame(uint8_t cmd, uint8_t *data, uint8_t dataLength,uint8_t frameType) ;
static void SendCommand(uint8_t cmd, uint8_t *data, uint8_t dataLength,uint8_t frameType) ;
static uint8_t PrepareAnswerFrame(uint8_t cmd, uint8_t *data, uint8_t dataLength, uint8_t frameType);

static void SendCmd_Answer(uint8_t cmd, uint8_t *data, uint8_t dataLength,uint8_t frameType);


volatile static uint8_t transOngoingFlag; //interrupt Transmit flag bit , 1---stop,0--run
uint8_t outputBuf[8];
//static uint8_t transferSize;
uint8_t inputBuf[MAX_BUFFER_SIZE];


/****************************************************************************************************
    * Function Name: SendFrame
    * Function: 发送数据帧
    * Input Ref: 数据帧内容和长度
****************************************************************************************************/
static void SendFrame(uint8_t *frame, uint8_t length) 
{
    if (length > 0) {
        while (transOngoingFlag); // 等待上一次传输完成
        transOngoingFlag = 1; // 设置传输标志
        HAL_UART_Transmit_IT(&huart1, frame, length); // 启动传输
    }
}
/****************************************************************************************************
    * Function Name: PrepareFrame
    * Function: 准备数据帧
    * Input Ref: 设备号、命令类型、数据、数据长度
    * Output Ref: 数据帧长度
****************************************************************************************************/
static uint8_t PrepareFrame(uint8_t cmd, uint8_t *data, uint8_t dataLength,uint8_t frameType) 
{
    outputBuf[0] = 0xA5; //display board帧头
    outputBuf[1] = 0x01;//deviceId; // 设备号
    outputBuf[2] = cmd; // 
    outputBuf[3] = frameType;//命令类型
    outputBuf[4] = dataLength;
    if(dataLength > 0){
  
        memcpy(&outputBuf[5], data, dataLength); // 
        outputBuf[5 + dataLength] = 0xFE; // 帧尾
        outputBuf[6 + dataLength] = bcc_check(outputBuf, 6 + dataLength); // BCC 校验
        return 7 + dataLength; //拷贝

    }
    else{

           outputBuf[5] = 0xFE; // 帧尾
           outputBuf[6] = bcc_check(outputBuf, 6); // BCC 校验
           return 7; // 


    }
   
}

/******************************************************************************
    * Function Name: PrepareFrame
    * Function: 准备数据帧
    * Input Ref: 命令、数据、数据长度和帧类型
    * Output Ref: 数据帧长度
******************************************************************************/
static uint8_t PrepareAnswerFrame(uint8_t cmd, uint8_t *data, uint8_t dataLength, uint8_t frameType)
{
    outputBuf[0] = 0xA5; // 帧头
    outputBuf[1] = 0x01; // 设备号
    outputBuf[2] = 0xFF; // 设备号
    outputBuf[3] = cmd;  // 命令类型
    outputBuf[4] = frameType ; //帧类型：00-open/clouse ,0x01- open/link/turn on 0x02-not-ai-mode , 0x10--多指令，0x0f:数据不是指令。 0xff：主板和显示板，通讯测试模式
    outputBuf[5] = dataLength; // 数据长度

    if (dataLength > 0) {
        memcpy(&outputBuf[6], data, dataLength); // 拷贝数据
         outputBuf[7+ dataLength] = 0xFE; // 帧尾
         outputBuf[8 + dataLength] = bcc_check(outputBuf, 8 + dataLength); // BCC 校验
          
        return 9 + dataLength; // 返回帧长度
    }
    else{

        outputBuf[6] = 0xFE;
        outputBuf[7] = bcc_check(outputBuf, 7); // BCC 校验

        return 8 ; // 返回帧长度
    }

}
/****************************************************************************************************
    * Function Name: SendCommand
    * Function: 发送通用命令
    * Input Ref: 设备号、命令类型、数据、数据长度
****************************************************************************************************/
static void SendCmd_Answer(uint8_t cmd, uint8_t *data, uint8_t dataLength,uint8_t frameType) 
{
    uint8_t frameLength = PrepareAnswerFrame(cmd, data, dataLength,frameType); // 准备数据帧
    SendFrame(outputBuf, frameLength); // 发送数据
}


/****************************************************************************************************
    * Function Name: SendCommand
    * Function: 发送通用命令
    * Input Ref: 设备号、命令类型、数据、数据长度
****************************************************************************************************/
static void SendCommand(uint8_t cmd, uint8_t *data, uint8_t dataLength,uint8_t frameType) 
{
    uint8_t frameLength = PrepareFrame(cmd, data, dataLength,frameType); // 准备数据帧
    SendFrame(outputBuf, frameLength); // 发送数据
}

/****************************************************************************************************
    * Function Name: SendData_PowerOnOff
    * Function: 发送电源开关命令
    * Input Ref: 电源状态 (index)
****************************************************************************************************/
void SendData_PowerOnOff(uint8_t index) 
{
    //uint8_t data[1] = {index}; // 电源状态
    SendCommand(0x01,0,0,index); // 发送命令
}
/****************************************************************************************************
    * Function Name: SendData_Buzzer
    * Function: 发送蜂鸣器命令
    * Input Ref: 无
****************************************************************************************************/
void SendData_Buzzer(void) {
    //uint8_t data[1] = {0x01}; // 蜂鸣器状态
    SendCommand(0x06,0,0,1); // 发送命令
}

/****************************************************************************************************
    * Function Name: SendData_Buzzer_Has_Ack
    * Function: 发送带应答的蜂鸣器命令
    * Input Ref: 无
****************************************************************************************************/
void SendData_Buzzer_Has_Ack(void) {
   // uint8_t data[1] = {0x01}; // 蜂鸣器状态
    SendCommand(0x16,0,0,1); // 发送命令
}

/****************************************************************************************************
    * Function Name: SendData_Set_Command
    * Function: 发送设置命令
    * Input Ref: 命令 (cmd) 和数据 (data)
****************************************************************************************************/
void SendData_Set_Command(uint8_t cmd, uint8_t frameType) {
   // uint8_t dataArray[1] = {data}; // 命令数据
    SendCommand(cmd,0,0,frameType); // 发送命令
}

/****************************************************************************************************
    * Function Name: SendData_Temp_Data
    * Function: 发送温度数据
    * Input Ref: 温度值 (tdata)
****************************************************************************************************/
void SendData_Temp_Data(uint8_t tdata) 
{
    uint8_t data[1] = {tdata}; // 温度数据
    SendCommand(0x1A,data,1,0x0F); // 发送命令
}

/****************************************************************************************************
    * Function Name: SendWifiData_Answer_Cmd
    * Function: 发送 WiFi 应答命令
    * Input Ref: 命令 (cmd) 和数据 (data)
****************************************************************************************************/
void SendWifiData_Answer_Cmd(uint8_t cmd, uint8_t frameType) {
   // uint8_t dataArray[1] = {data}; // 命令数据
  //  SendCommand(cmd,0,0,frameType); // 发送命令
    SendCmd_Answer(cmd,0,0,frameType);
}

/****************************************************************************************************
    * Function Name: HAL_UART_TxCpltCallback
    * Function: UART 发送完成回调函数
    * Input Ref: UART_HandleTypeDef 指针
****************************************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) 
{
    if (huart->Instance == USART1) {
        transOngoingFlag = 0; // 清除传输标志
    }
}



#if 0

/****************************************************************************************************
**
*Function Name:static void selectLight(uint8_t index)
*Function: UART2 transmit interrupt process ---4D 58 4C 53 32 30 32 
*Input Ref: LED number 
*Return Ref:NO
*
****************************************************************************************************/
void SendData_PowerOnOff(uint8_t index)
{
	
   //crc=0x55;
	outputBuf[0]=0xA5; //display board head = 0xA5
	outputBuf[1]= 0x01; //display device Number:is 0x01
	outputBuf[2]=0x01; // command type = 0x01 ->power on or power off 
	outputBuf[3]=index; // command order -> 01 - power on , 00- power off
	outputBuf[4]=0x00; // data is length: 00 ->don't data 
	outputBuf[5]=0xFE; // frame of end code -> 0xFE.
	
	outputBuf[6] = bcc_check(outputBuf,6);
	transferSize=7;
	if(transferSize)
	{
		while(transOngoingFlag);
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
	
}
/****************************************************************************************************
**
*Function Name:void SendData_Buzzer(void)
*Function: send buzzer sound cmd.
*Input Ref: NO 
*Return Ref:NO
*
****************************************************************************************************/
void SendData_Buzzer(void)
{
	
    outputBuf[0]=0xA5; //display board head = 0xA5
	outputBuf[1]= 0x01; //display device Number:is 0x01
	outputBuf[2]=0x06; // command type = 0x06 ->buzzer sound open or not
	outputBuf[3]=0x01; // command order -> 01 - buzzer sound done, 00- don't buzzer sound 
	outputBuf[4]=0x00; // data is length: 00 ->don't data 
	outputBuf[5]=0xFE; // frame of end code -> 0xFE.
	
	outputBuf[6] = bcc_check(outputBuf,6);
	transferSize=7;
	if(transferSize)
	{
		while(transOngoingFlag);
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
	
}

void SendData_Buzzer_Has_Ack(void)
{
	
    outputBuf[0]=0xA5; //display board head = 0xA5
	outputBuf[1]= 0x01; //display device Number:is 0x01
	outputBuf[2]=0x16; // command type = 0x06 ->buzzer sound open or not
	outputBuf[3]=0x01; // command order -> 01 - buzzer sound done, 00- don't buzzer sound 
	outputBuf[4]=0x00; // data is length: 00 ->don't data 
	outputBuf[5]=0xFE; // frame of end code -> 0xFE.
	
	outputBuf[6] = bcc_check(outputBuf,6);
	transferSize=7;
	if(transferSize)
	{
		while(transOngoingFlag);
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
	
}

/****************************************************************************************************
**
*Function Name:void SendData_Buzzer(void)
*Function: send buzzer sound cmd.
*Input Ref: to mainboard order , data - send to main board data. 
*Return Ref:NO
*
****************************************************************************************************/
void SendData_Set_Command(uint8_t cmd,uint8_t data)
{
    outputBuf[0]=0xA5; //display board head = 0xA5
	outputBuf[1]= 0x01; //display device Number:is 0x01
	outputBuf[2]=cmd; // command type = 0x06 ->buzzer sound open or not
	outputBuf[3]= data; // command order -> 01 - buzzer sound done, 00- don't buzzer sound 
	outputBuf[4]=0x00; // data is length: 00 ->don't data 
	outputBuf[5]=0xFE; // frame of end code -> 0xFE.
    outputBuf[6] = bcc_check(outputBuf,6);


		transferSize=7;
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

    outputBuf[0]=0xA5; //display board head = 0xA5
	outputBuf[1]= 0x01; //display device Number:is 0x01
	outputBuf[2]=0x1A; // command type = 0x1A -> temperature of value 
	outputBuf[3]=0x0f; // command order -> 0x0f -> is data , don't order.
	outputBuf[4]=0x01; // data is length: 00 ->don't data 
	outputBuf[5]=tdata; // frame of end code -> 0xFE.
	outputBuf[6]=0xFE; // frame of end code -> 0xFE.
    outputBuf[7] = bcc_check(outputBuf,7);
		
		transferSize=8;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}


/********************************************************************************
    **
    *Function Name
    *Function : commad order , data -command type
    *Input Ref: commad order , data -command type
    *Return Ref:NO
    *
*******************************************************************************/
void SendWifiData_Answer_Cmd(uint8_t cmd ,uint8_t data)
{
        outputBuf[0]=0x5A; //display board head = 0xA5
        outputBuf[1]=0x01; //display device Number:is 0x01
        outputBuf[2]=0xFF; // answer or copy command
        outputBuf[3]= cmd; // 0x0F : is data ,don't command order.
        outputBuf[4]= data; // don't data ,onlay is command order,recieve data is 1byte .
       
        outputBuf[5] = 0xFE; //frame is end of byte.
        outputBuf[6] = bcc_check(outputBuf,6);
        
        transferSize=7;
        if(transferSize)
        {
            while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
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

#endif 

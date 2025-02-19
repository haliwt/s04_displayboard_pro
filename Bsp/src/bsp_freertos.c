#include "bsp.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"


#define POWER_KEY_0	        (1 << 0)
#define MODE_KEY_1	        (1 << 1)
#define DEC_KEY_2           (1 << 2)
#define ADD_KEY_3           (1 << 3)

#define FAN_KEY_4           (1 << 4)
#define PLASMA_KEY_5        (1 << 5)
#define DRY_KEY_6           (1<< 6)
#define AI_KEY_7            ( 1<< 7)

#define MODE_LONG_KEY_8         (1 << 8)
#define DECODER_BIT_9          (1<< 9)




/***********************************************************************************************************
											函数声明
***********************************************************************************************************/
static void vTaskRunPro(void *pvParameters);
static void vTaskDecoderPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);



/* 创建任务通信机制 */
//static void AppObjCreate(void);


/***********************************************************************************************************
											变量声明
***********************************************************************************************************/
static TaskHandle_t xHandleTaskRunPro = NULL;
static TaskHandle_t xHandleTaskDecoderPro= NULL;
static TaskHandle_t xHandleTaskStart = NULL;

//static QueueHandle_t xQueue1 = NULL;
//static QueueHandle_t xQueue2 = NULL;
//static QueueHandle_t xQueue3 = NULL;



///#define QUEUE_LENGTH            1                   /* 队列支持的消息个数 */
///#define QUEUE_ITEM_SIZE         sizeof(uint32_t)    /* 队列中每条消息的大小 */
//#define SEMAPHORE_BINARY_LENGTH 1                   /* 二值信号量的有效长度 */
//#define QUEUESET_LENGTH         ((2 * QUEUE_LENGTH) + SEMAPHORE_BINARY_LENGTH)  /* 队列集支持的消息个数 */



typedef struct Msg
{
	
	uint8_t  usData[12];
    uint8_t  ucMessageID;
    uint8_t  rx_data_counter;
    uint8_t  disp_rx_cmd_done_flag;
    uint8_t  bcc_check_code;
    volatile uint8_t ulid;
 
}MSG_T;

MSG_T   gl_tMsg; /* 定义丢�个结构体用于消息队列 */

/**********************************************************************************************************
*	凄1�7 敄1�7 各1�7: vTaskTaskUserIF
*	功能说明: 接口消息处理〄1�7
*	彄1�7    叄1�7: pvParameters 是在创建该任务时传��的形参
*	迄1�7 囄1�7 倄1�7: 旄1�7
*   伄1�7 兄1�7 纄1�7: 1  (数��越小优先级越低，这个跟uCOS相反)
**********************************************************************************************************/
void freeRTOS_Handler(void)
{
     /* 创建任务 */
	  AppTaskCreate();
	  
	  /* 创建任务通信机制 */
	 //  AppObjCreate();
	  
	  /* 启动调度，开始执行任劄1�7 */
	   vTaskStartScheduler();


}
/**********************************************************************************************************
*   FunctionName: static void vTaskRunPro(void *pvParameters)
*	功能说明: 使用函数xTaskNotifyWait接收任务vTaskTaskUserIF发送的事件标志位设置
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 1  

*   priority : 1  (数值越小优先级越低，这个跟uCOS相反)
**********************************************************************************************************/
static void vTaskRunPro(void *pvParameters)
{

    uint8_t ucKeyCode;
    while(1)
    {
		
        ucKeyCode =  bsp_GetKey();

        if(ucKeyCode != KEY_NONE){

           switch(ucKeyCode){
        	case KEY_POWER_DOWN:
        	  if(run_t.gPower_On == power_off){
        		gpro_t.send_ack_cmd = ack_ptc_on;
        		gpro_t.gTimer_again_send_power_on_off =0;
        		 SendData_PowerOnOff(1);//power on
        	  }
        	  else{
        		  gpro_t.send_ack_cmd = ack_ptc_off;
        		  gpro_t.gTimer_again_send_power_on_off =0;
        		  SendData_PowerOnOff(0);//power on

        	  }

            bsp_ClearKey();

        	break;

        	case KEY_MODE_DOWN:
        	mode_key_fun();
            bsp_ClearKey();
        	break;

        	case KEY_DEC_DOWN:
        		key_dec_fun();
        		bsp_ClearKey();
        	break;

        	case KEY_ADD_DOWN:
        		 key_add_fun();
        		bsp_ClearKey();

        	break;

        	case KEY_AI_DOWN:

        		if(run_t.ai_model_flag == AI_MODE){
        		  run_t.ai_model_flag = NO_AI_MODE;
        		  LED_AI_OFF();
        		}
        		else{
        		   run_t.ai_model_flag = AI_MODE;
        		   LED_AI_ON();
        		   SendData_Set_Command(ai_cmd,0x01); //
        		}
        		//ai_key_fun(run_t.ai_model_flag);
        		bsp_ClearKey();

        	break;

        	case KEY_PLASMA_DOWN:

        		 if(run_t.gPlasma == 1){
        			 run_t.gPlasma = 0;
        			 LED_PLASMA_OFF();
        			 SendData_Set_Command(plasma_cmd,0x0); //
        		 }
        		 else{
        			 run_t.gPlasma = 1;
        			 LED_PLASMA_ON();
        			 SendData_Set_Command(plasma_cmd,0x01); //
        	     }



        		bsp_ClearKey();

           break;

           case KEY_DRY_DOWN:

        	   if(run_t.gDry ==0){
        		   run_t.gDry =1;
        		   LED_DRY_ON();
        		   gpro_t.send_ack_cmd = ack_ptc_on;
        		   gpro_t.gTimer_again_send_power_on_off =0;
        		   SendData_Set_Command(dry_cmd,0x01); //
        	   }
        	   else{
        		   run_t.gDry = 0;
        		   LED_DRY_OFF();
        		   gpro_t.send_ack_cmd = ack_ptc_off;
        		   gpro_t.gTimer_again_send_power_on_off =0;
        		   SendData_Set_Command(dry_cmd,0x00); //
        	   }


        	   gpro_t.manual_turn_off_dry_flag =1;


        	   bsp_ClearKey();

           break;
           
           case KEY_MOUSE_DOWN:
        	   if(run_t.gMouse ==0){
        		   run_t.gMouse = 1;
        		   LED_MOUSE_ON();
                   SendData_Set_Command(mouse_cmd,0x01); //
        	   }
        	   else{
        		   run_t.gMouse = 0;
        		   LED_MOUSE_OFF();
                   SendData_Set_Command(mouse_cmd,0); //
        	   }
        	  
        	   bsp_ClearKey();

           break;

           }

        }


       if(run_t.gPower_On == power_on){

      
       if(gpro_t.set_timer_timing_doing_value==1){


         ai_ico_fast_blink();
         TM1639_Write_4Bit_Time(run_t.hours_two_decade_bit,run_t.hours_two_unit_bit, run_t.minutes_one_decade_bit,run_t.minutes_one_unit_bit,0) ;

       }
       else if(gpro_t.set_timer_timing_doing_value==0){

          TM1639_Write_2bit_SetUp_TempData(run_t.set_temperature_decade_value,run_t.set_temperature_unit_value,0);

       }

       
       power_on_run_handler();
             
       Display_TimeColon_Blink_Fun();
       set_timer_fun_led_blink();

       works_run_two_hours_handler();

      }
      else if(run_t.gPower_On == power_off){

          power_off_run_handler();

       }

      send_cmd_ack_hanlder();

      vTaskDelay(20);

    }

 }

/**********************************************************************************************************
*	Function Name: vTaskStart
*	Function: 
*	Input Ref: pvParameters 是在创建该任务时传��的形参
*	Return Ref:
*   priority: 2  (数值越小优先级越低，这个跟uCOS相反)
**********************************************************************************************************/
static void vTaskDecoderPro(void *pvParameters)
{

	BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(300); /* 设置最大等待时间为30ms */
	uint32_t ulValue;
	uint8_t check_code;
	
    while(1)
    {
		xResult = xTaskNotifyWait(0x00000000,
								  0xFFFFFFFF,     /* Reset the notification value to 0 on */
								&ulValue,        /* 保存ulNotifiedValue到变量ulValue中 */
							  xMaxBlockTime);  /* 阻塞时间30ms，释放CUP控制权,给其它任务执行的权限*/

		if( xResult == pdPASS )
		{
			/* 接收到消息，检测那个位被按下 */
			if((ulValue & DECODER_BIT_9) != 0){
			gl_tMsg.disp_rx_cmd_done_flag = 0;

			check_code =  bcc_check(gl_tMsg.usData,gl_tMsg.ulid);

			if(check_code == gl_tMsg.bcc_check_code ){

			receive_data_fromm_mainboard(gl_tMsg.usData);
			}
			}
		
		}


    }
    
}
/**********************************************************************************************************
*
*	Function Name: vTaskStart
*	Function: 
*	Input Ref: pvParameters 是在创建该任务时传��的形参
*	Return Ref:
*   priority: 3  (数��越小优先级越低，这个跟uCOS相反)
*
**********************************************************************************************************/
static void vTaskStart(void *pvParameters)
{
	
   const TickType_t xMaxBlockTime = pdMS_TO_TICKS(20); /* 设置最大等待时间为30ms */
    while(1)
    {
      
      bsp_KeyScan();

     vTaskDelay(xMaxBlockTime);//
     
    }

}
/**********************************************************************************************************
*	凄1�7 敄1�7 各1�7: AppTaskCreate
*	功能说明: 创建应用任务
*	彄1�7    参：旄1�7
*	迄1�7 囄1�7 倄1�7: 旄1�7
**********************************************************************************************************/
void AppTaskCreate (void)
{

	xTaskCreate( vTaskRunPro,    		/* 任务函数  */
                 "vTaskRunPro",  		/* 任务各1�7    */
                 128,         		/* stack大小，单位word，也就是4字节 */
                 NULL,        		/* 任务参数  */
                 1,           		/* 任务优先纄1�7 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskRunPro); /* 任务句柄  */
	
	xTaskCreate( vTaskDecoderPro,     		/* 任务函数  */
                 "vTaskDecoderPro",   		/* 任务各1�7    */
                 128,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 2,               		/* 任务优先纄1�7 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskDecoderPro );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务各1�7    */
                 128,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 3,              		/* 任务优先纄1�7 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskStart );   /* 任务句柄  */
}



/*
*********************************************************************************************************
*	凄1�7 敄1�7 各1�7: AppObjCreate
*	功能说明: 创建任务通信机制
*	彄1�7    叄1�7: 旄1�7
*	迄1�7 囄1�7 倄1�7: 旄1�7
*********************************************************************************************************
*/
# if 0 
void AppObjCreate (void)
{
    #if 1

//   /* 创建10个uint8_t型消息队刄1�7 */
//	xQueue1 = xQueueCreate(4, sizeof(uint8_t));
//    if( xQueue1 == 0 )
//    {
//        /* 没有创建成功，用户可以在这里加入创建失败的处理机刄1�7 */
//    }
	
	/* 一次性可以传送4个字节的数据(一次性可以存储的最大项目数)，由于CM3/CM4内核昄1�732位机，一个指针变量占甄1�74个字芄1�7 */
	xQueue2 = xQueueCreate(4, sizeof(struct Msg *));
    if( xQueue2 == 0 )
    {
        /* 没有创建成功，用户可以在这里加入创建失败的处理机刄1�7 */
    }

	

	#endif 

    #if 0

	 /* 创建队列雄1�7 */
    xQueueSet = xQueueCreateSet(QUEUESET_LENGTH);
    /* 创建队列*/
    xQueue1 = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);
    xQueue2 = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);
	
    /* 创建二��信号量 */
    xSemaphore = xSemaphoreCreateBinary();
	
    /* 将队列和二��信号量添加到队列集丄1�7 */
    xQueueAddToSet(xQueue1, xQueueSet);
    xQueueAddToSet(xQueue2, xQueueSet);
    xQueueAddToSet(xSemaphore, xQueueSet);
    #endif 
}

#endif 
/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
     static uint8_t state,rx_end_flag ;
     BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  

    if(huart==&huart1) // Motor Board receive data (filter)
	{

    //   DISABLE_INT();
       switch(state)
		{
		case 0:  //#0
			if(inputBuf[0] == 0x5A){  // 0x5A --main board singla
               gl_tMsg.rx_data_counter=0;
               gl_tMsg.usData[gl_tMsg.rx_data_counter] = inputBuf[0];
				state=1; //=1

             }
            else
                state=0;
		break;

       
		case 1: //#1

            if(gl_tMsg.disp_rx_cmd_done_flag ==0){
              /* 初始化结构体指针 */
               gl_tMsg.rx_data_counter++;
		     
	          gl_tMsg.usData[gl_tMsg.rx_data_counter] = inputBuf[0];
              

              if(rx_end_flag == 1){

                state = 0;
            
                gl_tMsg.ulid = gl_tMsg.rx_data_counter;
                rx_end_flag=0;

                gl_tMsg.rx_data_counter =0;

                gl_tMsg.disp_rx_cmd_done_flag = 1 ;

                gl_tMsg.bcc_check_code=inputBuf[0];

              
                xTaskNotifyFromISR(xHandleTaskRunPro,  /* 目标任务 */
                                    DECODER_BIT_9,     /* 设置目标任务事件标志位bit0  */
                                    eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
                                    &xHigherPriorityTaskWoken);

                /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
                  
              }

              }

              if(gl_tMsg.usData[gl_tMsg.rx_data_counter] ==0xFE && rx_end_flag == 0 &&   gl_tMsg.rx_data_counter > 4){
                     
                     rx_end_flag = 1 ;
                          
              }

        break;


			
		}

       //   ENABLE_INT();
    __HAL_UART_CLEAR_OREFLAG(&huart1);
	HAL_UART_Receive_IT(&huart1,inputBuf,1);//UART receive data interrupt 1 byte
    
   }
}

#if 0


    if(huart==&huart1) // Motor Board receive data (filter)
	{

       DISABLE_INT();
       switch(state)
		{
		case 0:  //#0
			if(inputBuf[0]=='M' ||inputBuf[0]==0x55)  {//hex :4D - "M" -fixed mainboard
               
             
				state=1; 
		    }
			else{
			    state=0; //=1
              

			}
			break;
		case 1: //#1
			if(inputBuf[0]=='A' || inputBuf[0]==0x42) //hex : 41 -'A'  -fixed master
			{
				
               state=2; 
			}
			else{
				
				state=0;
		


				}
			break;
		case 2://#2
			if(inputBuf[0]=='D'||inputBuf[0]==0x4B || inputBuf[0]=='E'){
				if(inputBuf[0]=='D'){
                    rx_mb_data_tag=PANEL_DATA;
                    state = 3;

                }
                else if(inputBuf[0]=='E'){
                  rx_mb_data_tag=ORDER_DATA;
                  state=3;
                }
                else if(inputBuf[0]==0x4B){
                    state = 5;

                }
				
			}
            else{
				state=0;
				


				}
			break;
            
        case 3:

            if(rx_mb_data_tag==PANEL_DATA){
            
                // run_t.gReal_humtemp[0]=inputBuf[0]; //Humidity value 
                  rxcmd[0]=inputBuf[0];
                  state = 4; 
                
             }
			else if(rx_mb_data_tag==ORDER_DATA){

                rxcmd[0]=  inputBuf[0];  
                /* 初始化结构体指针 */
                ptMsg = &g_tMsg;
                ptMsg->ucMessageID=ORDER_DATA;
    		    ptMsg->ulData[0] = inputBuf[0]; //rxcmd[0];
                ptMsg ->usData[0] = 0;
               
               

                 /* 向消息队列发数据 */
            	xQueueSendFromISR(xQueue2,
            				      (void *)&ptMsg,
            				      &xHigherPriorityTaskWoken);

            	/* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
            	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

             
           

                state=0;

			}
            else {
               state = 0; 
          
            }
	    break;
        
		case 4: //

		 if(rx_mb_data_tag==PANEL_DATA){
              //run_t.gReal_humtemp[1]=inputBuf[0]; //temperature value
              
               /* 初始化结构体指针 */
                ptMsg = &g_tMsg;
                ptMsg->ucMessageID = PANEL_DATA;
    		    ptMsg->ulData[0] =  rxcmd[0];
                ptMsg ->usData[0] = inputBuf[0];
               
                /* 向消息队列发数据 */
            	xQueueSendFromISR(xQueue2,
            				      (void *)&ptMsg,
            				      &xHigherPriorityTaskWoken);

            	/* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
            	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
			
		     state=0;
		    
		
              
          }
         else {

		
			state =0;

         }
		
		 
		 break;

         case 5:

           rxcmd[0]=inputBuf[0];

           state = 6;


         break;


		 case 6:

            /* 初始化结构体指针 */
            ptMsg = &g_tMsg;
            ptMsg->ucMessageID=ANSWER_DATA;
		    ptMsg->ulData[0] = rxcmd[0];
            ptMsg ->usData[0] =  inputBuf[0];
           
           

             /* 向消息队列发数据 */
        	xQueueSendFromISR(xQueue2,
        				      (void *)&ptMsg,
        				      &xHigherPriorityTaskWoken);

        	/* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
        	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	       
		 
             state=0;
        break;
           
       
		
		
		default:
			
		break;
		}


          ENABLE_INT();
       __HAL_UART_CLEAR_OREFLAG(&huart1);
		HAL_UART_Receive_IT(&huart1,inputBuf,1);//UART receive data interrupt 1 byte
	}
}
#endif 

void USART1_Cmd_Error_Handler(void)
{
   uint32_t temp;
   if(run_t.gTimer_usart_error >4){
	  	run_t.gTimer_usart_error=0;
	    
         
        __HAL_UART_CLEAR_OREFLAG(&huart1);
        temp = USART1->RDR;
		UART_Start_Receive_IT(&huart1,inputBuf,1);
       
		}
      
    
  
 }
















#include "bsp.h"


RUN_T run_t;

uint8_t power_on_off_flag;

void Power_Off(void);


/**********************************************************************
*
*Functin Name: void Display_DHT11_Value(void)
*Function : Timer of key be pressed handle
*Input Ref:  key of value
*Return Ref: NO
*
**********************************************************************/
void Power_Off(void)
{
    Smg_AllOff();
    SMG_POWER_OFF()	;
	run_t.gPlasma=0;
	run_t.gDry=0;
	gpro_t.gmouse =0;
			
}

/**********************************************************************
*
*Functin Name: void Receive_MainBoard_Data_Handler(run_t.single_data)
*Function : run order from main command 
*Input Ref:  run_t.single_dat is command 
*Return Ref: NO
*
**********************************************************************/
void Receive_MainBoard_Data_Handler(uint8_t ulid,uint8_t data1,uint8_t data2)
{
	
	

   switch(ulid){

     case ORDER_DATA:

	   switch(data1){

         case PTC_WARNING:
		 	run_t.ptc_warning =1;
		 	run_t.gDry =0;
           //SendData_Set_Command(DRY_OFF_NO_BUZZER); 

	     break;

		 case FAN_WARNING:
		 	run_t.fan_warning =1;
            run_t.gDry = 0;
            gpro_t.gmouse = 1;
            run_t.gPlasma = 0;
           // SendData_Set_Command(DRY_OFF_NO_BUZZER); //main board shuld be shut down DRY AND PLASMA
		 	

		 break;

		
         }
    break;


    case PANEL_DATA:
    
	    run_t.gReal_humtemp[0] = data1 ;  //Humidity 
        run_t.gReal_humtemp[1] = data2;  // temperature
	   
      
       break;

	  case ANSWER_DATA:
          
//        if(data1 == 'P' && data2 == 0x01){
//
//           gpro_t.answer_signal_flag = power_on;
//
//
//        }
//        else if(data1 == 'P' && data2 == 0){
//           gpro_t.answer_signal_flag = power_off;
//
//
//        }
     

	  break;

	

   }

}
/**********************************************************************
*
*Functin Name: void Receive_ManiBoard_Cmd(uint8_t cmd)
*Function :  wifi recieve data
*Input Ref:  receive wifi send order
*Return Ref: NO
*
**********************************************************************/
void Power_On_Fun(void)
{
                
   static uint8_t hour_decade,hour_unit,minutes_one,minutes_two;

  
	
		run_t.gPlasma=1;
		run_t.gDry =1;
        gpro_t.gmouse = 1;
		run_t.ai_model_flag =AI_MODE;

  


    run_t.time_led_flag=1;
	Power_ON_Led();

	run_t.fan_warning=0;
	run_t.ptc_warning=0;

	run_t.works_dispTime_hours=0;
	run_t.works_dispTime_minutes=0;
	run_t.gTimes_time_seconds =0;
	
    run_t.gTimer_timer_timing_counter=0;
    gpro_t.set_timer_timing_value_success =0 ;
    gpro_t.set_temp_value_success = 0;
   
	run_t.timer_dispTime_hours =0;
	run_t.timer_dispTime_minutes=0;
		
        

	  hour_decade=0;
	  hour_unit=0;
    
	  minutes_one = 0;
      minutes_two = 0;
	  
	   SMG_POWER_ON(); //WT.EDIT 2023.03.02
      


	  run_t.hours_two_unit_bit = hour_unit;
	  run_t.minutes_one_decade_bit =  minutes_one;
      
	 TM1639_Write_4Bit_Time(hour_decade,run_t.hours_two_unit_bit,run_t.minutes_one_decade_bit,minutes_two,0);
     Display_DHT11_Value();
    
}


/************************************************************************
	*
	*Function Name: static void Power_Off_Fun(void)
	*
	*
	*
	*
************************************************************************/
void Power_Off_Fun(void)
{
	
 
		run_t.gPlasma=0;
		run_t.gDry =0;
		gpro_t.gmouse = 0;
		
        run_t.gPower_On =power_off;
		run_t.gTimer_set_temp_times=0; //conflict with send temperatur value 
       Power_Off_Led_Off();

  
} 


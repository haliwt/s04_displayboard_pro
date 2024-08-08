#include "bsp.h"


pro_run_t  gpro_t;


volatile uint8_t power_on_of_id ;

uint8_t hours_one,hours_two,minutes_one,minutes_two;

uint8_t  step_state;



void bsp_init(void)
{

    run_t.gRunCommand_label =RUN_NULL;
    run_t.gPower_On = power_off;


}

void power_on_handler(void)
{

    power_on_of_id = power_on_of_id ^ 0x01;
    if(power_on_of_id == 1){

        run_t.gTimer_set_temp_times=0; //conflict with send temperatur value

        run_t.gRunCommand_label =RUN_POWER_ON;
        run_t.gPower_On = power_on;
        run_t.power_off_flag = 0;
        run_t.ai_model_flag=AI_MODE;

        SendData_PowerOff(1);


    }
    else{
        if(run_t.gRunCommand_label ==SPECIAL_DISP && run_t.gPower_On == power_on){
            SendData_PowerOff(0);
            run_t.gPower_On = power_off;
            run_t.gRunCommand_label =RUN_NULL;

        }


    }
}

/******************************************************************************
	*
	*Function Name:void RunPocess_Command_Handler(void)
	*Funcion: display pannel run of process 
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
void power_on_run_handler(void)
{

 //  static uint8_t  step_state;
   switch(run_t.gRunCommand_label){

      case RUN_POWER_ON:
          
	  
           run_t.gTimer_time_colon =0;
	       run_t.set_temperature_decade_value=40;
           
            run_t.gTimer_detect_mb_receive_flag =0;
			Power_On_Fun();
			run_t.gTimer_display_dht11 = 20; //at once display temperature and humidity value.
			gpro_t.set_timer_timing_doing_value = 0;
			run_t.gRunCommand_label= SPECIAL_DISP;

            break;

            
	  break;

      case SPECIAL_DISP:


              if(gpro_t.set_timer_timing_doing_value == 1){

                   Set_TimerTiming_Number_Value();
                   
              }
              else if(gpro_t.set_timer_timing_doing_value == 0 &&  run_t.set_temperature_special_value   >0 &&  run_t.set_temperature_special_value != 0xff ){

           
                   Set_Temperature_Value();
	              
						
             }
             else{

              switch(step_state){

					case 0:
						Led_Panel_OnOff();
					    step_state=1;
					break;

					case 1:
                 
               			 RunLocal_Dht11_Data_Process();
                       
				        step_state=2;
	                    
				   break;
                    
                    case 2:
                        if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){ //read main board ptc_warning of ref.
                            Display_SmgTiming_Value();
                            }
                        else{

                            Warning_Error_Numbers_Fun();

                          }

//                       if(gpro_t.answer_signal_flag == 0 &&  ){
//
//                            SendData_PowerOff(1);  
//                       }

                     step_state=0;
                    break;

              }
            
             }    
      break;

	 

  }
}

/******************************************************************************
	*
	*Function Name:void mode_key_fun(void)
	*Funcion: exit this mode set fun ,
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
void mode_key_fun(void)
{

   if(gpro_t.set_timer_timing_doing_value  == 0){
  
       gpro_t.set_timer_timing_doing_value = 1;
       run_t.gTimer_key_timing = 0;
       run_t.gTimer_smg_blink_times =0;
       //gpro_t.gTimer_4bitsmg_blink_times=0; //4bit sumaguan blink time.
       gpro_t.set_timer_first_smg_blink_flag=0;
   }
   else{ //the send be pressed is confirm 

        gpro_t.set_timer_timing_doing_value  = 0;
        if(run_t.temporary_timer_dispTime_hours >0 || run_t.temporary_timer_dispTime_minutes >0){
          gpro_t.set_timer_timing_value_success  = TIMER_SUCCESS;
          run_t.gTimer_timer_timing_counter = 0;
           LED_AI_OFF();
         run_t.ai_model_flag = NO_AI_MODE; //set up timer timing value 
         run_t.timer_dispTime_hours = run_t.temporary_timer_dispTime_hours ;
         run_t.timer_dispTime_minutes = run_t.temporary_timer_dispTime_minutes ;

         run_t.ai_model_flag =NO_AI_MODE;

          Display_Timing(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes);
         

         }
         else{
            run_t.ai_model_flag =AI_MODE;
            gpro_t.set_timer_timing_value_success  = 0;
            LED_AI_ON();

         }



   }

 
 }
/******************************************************************************
	*
	*Function Name:void RunPocess_Command_Handler(void)
	*Funcion: display pannel run of process 
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
void power_off_run_handler(void)
{
    static uint8_t power_on_off_flag;
    switch(run_t.power_off_flag){
     case 0://2
	  
		 run_t.ptc_warning =0;
		 run_t.fan_warning =0;
			
         run_t.gFan_RunContinue =1;
	     run_t.gTimer_fan_continue=0;
         run_t.gTimer_detect_mb_receive_flag=0;
		   
		 
         //  Power_Off();
           Power_Off_Led_Off();
         run_t.power_off_flag = 1;
       break;

       case 1://4


            if(run_t.gTimer_fan_continue < 61 && run_t.gFan_RunContinue == 1 && power_on_off_flag !=0){
                   
					//LED_FAN_ON() ;
		      }
			  else if(run_t.gTimer_fan_continue > 59){
                    run_t.gTimer_fan_continue =0;
				  // LED_FAN_OFF() ;
				   run_t.gFan_RunContinue ++;
                   power_on_off_flag = 1;

			}

//           if(gpro_t.answer_signal_flag == power_on){
//
//                 SendData_PowerOff(0);
//				
//            }
//            
		  
            Breath_Led();
		 
		 break;
       }

}
/*********************************************************************************
 * 
 * Function Name:void ai_on_off_handler(void)
 * 
 * 
 **********************************************************************************/
void ai_on_off_handler(void)
{
    if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){
       switch(gpro_t.set_timer_timing_doing_value){

        case 0:  //don't set timer timing item

        if(run_t.ai_model_flag ==AI_MODE){
            run_t.ai_model_flag =NO_AI_MODE;
            LED_AI_OFF();
            SendData_Set_Command(AI_MODE_OFF);
            if(gpro_t.set_timer_timing_value_success ==TIMER_SUCCESS)
              Display_Timing(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes);
        }
        else{
            if(run_t.ai_model_flag ==NO_AI_MODE){
            run_t.ai_model_flag =AI_MODE;
            SendData_Set_Command(AI_MODE_ON);

      
            run_t.gDry= 1;
            
            run_t.gPlasma = 1;
            gpro_t.gmouse = 1;
            led_mouse_on();
            LED_PLASMA_ON();
            LED_DRY_ON();
            LED_AI_ON();
            Display_Timing(run_t.works_dispTime_hours,run_t.works_dispTime_minutes);
           
            }  

        }
        break;

        #if 0
        case 1://ai key is confirm function . set timer timing numbers 

        SendData_Buzzer();

        gpro_t.set_timer_timing_doing_value  = 0;
        if(run_t.temporary_timer_dispTime_hours >0 || run_t.temporary_timer_dispTime_minutes >0){
          gpro_t.set_timer_timing_value_success  = TIMER_SUCCESS;
          run_t.gTimer_timer_timing_counter = 0;
           LED_AI_OFF();
         run_t.timer_dispTime_hours = run_t.temporary_timer_dispTime_hours ;
         run_t.timer_dispTime_minutes = run_t.temporary_timer_dispTime_minutes ;

         run_t.ai_model_flag =NO_AI_MODE;

          Display_Timing(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes);
         

         }
         else{
            run_t.ai_model_flag =AI_MODE;
            gpro_t.set_timer_timing_value_success  = 0;
            LED_AI_ON();

         }
        
         break;
         #endif 

        }


    }


}
/*********************************************************************************
 * 
 * Function Name:void mouse_on_off_handler(void)
 * 
 * 
 **********************************************************************************/
void mouse_on_off_handler(void)
{
   if(run_t.fan_warning ==0 && run_t.ptc_warning == 0){ 

        if(run_t.ai_model_flag== NO_AI_MODE){


        if(gpro_t.gmouse ==1){

            gpro_t.gmouse =0; //tur Off
           led_mouse_off();
           
            
           SendData_Set_Command(MOUSE_STOP);

        }
        else {

          gpro_t.gmouse =1; //turn on
          led_mouse_on();
          
          SendData_Set_Command(MOUSE_RUN);
        }


        }
  }
 

}

/******************************************************
*
*Function Name:void key_add_fun(void)
*
*
******************************************************/
void key_add_fun(void)
{
    if(run_t.ptc_warning ==0){
    

    run_t.gTimer_time_colon=0;

    switch(gpro_t.set_timer_timing_doing_value){

    case 0:  //set temperature value 
         SendData_Buzzer();
        gpro_t.set_up_temperature_value ++;
        if(gpro_t.set_up_temperature_value < 20){
        gpro_t.set_up_temperature_value=20;
        }

        if(gpro_t.set_up_temperature_value > 40)gpro_t.set_up_temperature_value= 20;

        run_t.set_temperature_decade_value = gpro_t.set_up_temperature_value / 10 ;
        run_t.set_temperature_unit_value  =gpro_t.set_up_temperature_value % 10; //

       // run_t.set_temperature_flag=set_temperature_value;
        run_t.set_temperature_special_value=1;
        run_t.gTimer_key_temp_timing=0;
        run_t.gTimer_time_colon=0;
        gpro_t.manual_turn_off_dry_flag =0; //after set temperature allow shut off dry .
      
        
      //   TM1639_Write_2bit_SetUp_TempData(run_t.set_temperature_decade_value,run_t.set_temperature_unit_value,0);

    break;
    

    case 1: //set timer timing value 
        SendData_Buzzer();

       ai_ico_fast_blink();
        run_t.gTimer_key_timing =0;
    
        if(run_t.temporary_timer_dispTime_hours !=24)
            run_t.temporary_timer_dispTime_minutes =  run_t.temporary_timer_dispTime_minutes + 30;
        else if(run_t.temporary_timer_dispTime_hours ==24)
            run_t.temporary_timer_dispTime_minutes =  run_t.temporary_timer_dispTime_minutes + 60;
        
        if(run_t.temporary_timer_dispTime_minutes >59){
            run_t.temporary_timer_dispTime_hours ++;
            if(run_t.temporary_timer_dispTime_hours ==24){
                run_t.temporary_timer_dispTime_minutes=0;
            }
            else if(run_t.temporary_timer_dispTime_hours >24){

                run_t.temporary_timer_dispTime_hours=0;
                run_t.temporary_timer_dispTime_minutes=0;


            }
            else{

                run_t.temporary_timer_dispTime_minutes =0;


            }

        }

       run_t.gTimer_time_colon =0;
        
     run_t.hours_two_decade_bit = run_t.temporary_timer_dispTime_hours /10;
    run_t.hours_two_unit_bit   = run_t.temporary_timer_dispTime_hours %10;

    run_t.minutes_one_decade_bit =  run_t.temporary_timer_dispTime_minutes /10;

    run_t.minutes_one_unit_bit = run_t.temporary_timer_dispTime_minutes %10;

    ai_ico_fast_blink();
 //   TM1639_Write_4Bit_Time(run_t.hours_two_decade_bit,run_t.hours_two_unit_bit, run_t.minutes_one_decade_bit,run_t.minutes_one_unit_bit,0) ; //timer is default 12 hours "12:00" 
    break;

    }

   }

}
/******************************************************
*
*Function Name:void key_dec_fun(void)
*
*
******************************************************/
void key_dec_fun(void)
{
    
    if(run_t.ptc_warning ==0 ){
    run_t.gTimer_time_colon=0;
  
    switch(gpro_t.set_timer_timing_doing_value){

    case 0: //set temperature value

        SendData_Buzzer();

        //setup temperature of value,minimum 20,maximum 40
        gpro_t.set_up_temperature_value--;
        if(gpro_t.set_up_temperature_value<20) gpro_t.set_up_temperature_value=40;
        else if(gpro_t.set_up_temperature_value >40)gpro_t.set_up_temperature_value=40;

        run_t.set_temperature_decade_value = gpro_t.set_up_temperature_value / 10 ;
        run_t.set_temperature_unit_value  =gpro_t.set_up_temperature_value % 10; //


       //  run_t.set_temperature_flag=set_temperature_value;
        run_t.set_temperature_special_value=1;
        run_t.gTimer_key_temp_timing=0;
        run_t.gTimer_time_colon=0;
        gpro_t.manual_turn_off_dry_flag =0; //after set temperature allow shut off dry .

     //   TM1639_Write_2bit_SetUp_TempData(run_t.set_temperature_decade_value,run_t.set_temperature_unit_value,0);
        

    break;

    case 1: //set timer timing value
    SendData_Buzzer();

    ai_ico_fast_blink();
    run_t.gTimer_key_timing =0;
    
    run_t.temporary_timer_dispTime_minutes =  run_t.temporary_timer_dispTime_minutes -30;
    if(run_t.temporary_timer_dispTime_minutes < 0){
        run_t.temporary_timer_dispTime_hours--;
        if(run_t.temporary_timer_dispTime_hours <0){

        run_t.temporary_timer_dispTime_hours=24;
        run_t.temporary_timer_dispTime_minutes=0;

        }
        else{

        run_t.temporary_timer_dispTime_minutes =30;


        }

     }
     run_t.gTimer_time_colon =0;

    run_t.hours_two_decade_bit = run_t.temporary_timer_dispTime_hours /10;
    run_t.hours_two_unit_bit   = run_t.temporary_timer_dispTime_hours %10;

    run_t.minutes_one_decade_bit =  run_t.temporary_timer_dispTime_minutes /10;

    run_t.minutes_one_unit_bit = run_t.temporary_timer_dispTime_minutes %10;
    
    ai_ico_fast_blink();
    
  //  TM1639_Write_4Bit_Time(run_t.hours_two_decade_bit,run_t.hours_two_unit_bit, run_t.minutes_one_decade_bit,run_t.minutes_one_unit_bit,0) ; //timer is default 12 hours "12:00" 

  

    break;
    }

   }


}

/*******************************************************
*
*Function Name: void compare_temp_value()
*Function :by display pannel of calculate after send to main board 
*
*
*******************************************************/
void compare_temp_value(void)
{
    static uint8_t first_one_flag;

    if(gpro_t.set_temp_value_success ==1){
    

     if(gpro_t.set_up_temperature_value >run_t.gReal_humtemp[1] && gpro_t.manual_turn_off_dry_flag ==0){ //PTC TURN ON
         run_t.gDry =1;
    	
        LED_DRY_ON();
       
		SendData_Set_Command(DRY_ON_NO_BUZZER);
        
     }
     else{ //PTC turn off 
         run_t.gDry =0;
         LED_DRY_OFF();
    	 SendData_Set_Command(DRY_OFF_NO_BUZZER);


     }

   }
   else{
        if(run_t.gReal_humtemp[1] >39){

         run_t.gDry =0;
         LED_DRY_OFF();
    	 SendData_Set_Command(DRY_OFF_NO_BUZZER);
         first_one_flag =1;
        }
        else{

           if(first_one_flag==1 && (run_t.gReal_humtemp[1] <39)){

              
                 run_t.gDry =1;
            
                LED_DRY_ON();
             
        		SendData_Set_Command(DRY_ON_NO_BUZZER);

               

             }
           }

      }
}



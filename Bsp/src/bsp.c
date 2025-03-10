#include "bsp.h"


pro_run_t  gpro_t;




uint8_t hours_one,hours_two,minutes_one,minutes_two;

uint8_t  step_state;



void bsp_init(void)
{

    run_t.gRunCommand_label =RUN_NULL;
    run_t.gPower_On = power_off;


}

void power_on_handler(void)
{

	run_t.gTimer_set_temp_times=0; //conflict with send temperatur value

	run_t.gPower_On = power_on;
	run_t.gRunCommand_label =RUN_POWER_ON;
	run_t.power_off_flag = 0;
	run_t.ai_model_flag=AI_MODE;

	gpro_t.gTimer_total_works_two_hours =0;

	gpro_t.interval_works_ten_minutes_flag = 0;

 }

void power_off_handler(void)
{

    run_t.gPower_On = power_off;
	run_t.gRunCommand_label =RUN_NULL;

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
            gpro_t.key_set_dry_flag = 0; //allow open dry function .
			run_t.gRunCommand_label= SPECIAL_DISP;


            
	  break;

      case SPECIAL_DISP:


              if(gpro_t.set_timer_timing_doing_value == 1 && run_t.ptc_warning ==0 && run_t.fan_warning ==0){

                   Set_TimerTiming_Number_Value();
                   
              }
              else if(gpro_t.set_timer_timing_doing_value == 0 &&  run_t.set_temperature_special_value   >0 &&  run_t.set_temperature_special_value != 0xff ){

                   //
                   disp_smg_blink_set_tempeature_value();
	              
						
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
                    
                    case 2: //display 1:   timing times  2: timer times.
                        if(gpro_t.set_timer_timing_doing_value==0){
                        if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){ //read main board ptc_warning of ref.
                            Display_SmgTiming_Value();

                         }
                        else{

                            Warning_Error_Numbers_Fun();

                        }
                        
                        }

                     step_state=0;
                    break;

              }
            
             }    
      break;

	}
}


void detected_ptc_or_fan_warning_fun(void)
{

    Warning_Error_Numbers_Fun();

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
         gpro_t.ai_mode_set_flag=2;

          Display_Timing(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes);
         

         }
         else{
            run_t.ai_model_flag =AI_MODE;
            gpro_t.set_timer_timing_value_success  = 0;
            LED_AI_ON();
            gpro_t.ai_mode_set_flag=1;

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

         
         gpro_t.gTimer_total_works_two_hours =0;
         
         gpro_t.interval_works_ten_minutes_flag =0;
		   
		 
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
				 
				   run_t.gFan_RunContinue ++;
                 

			}


		  
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



/*********************************************************************************
 * 
 * Function Name:void mouse_on_off_handler(void)
 * 
 * 
 **********************************************************************************/

/*******************************************************
*
*Function Name: void bsp_plasma_handler(uint8_t data)
*Function :
*
*
*******************************************************/


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

       // run_t.set_temperature_flag=disp_smg_blink_set_tempeature_value;
        run_t.set_temperature_special_value=1;
        run_t.gTimer_key_temp_timing=0;
       
        gpro_t.key_set_dry_flag =0; // allow open dry function.WT.2025.02.21
      
        
      TM1639_Write_2bit_SetUp_TempData(run_t.set_temperature_decade_value,run_t.set_temperature_unit_value,0);

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

  
    switch(gpro_t.set_timer_timing_doing_value){

    case 0: //set temperature value

        SendData_Buzzer();

        //setup temperature of value,minimum 20,maximum 40
        gpro_t.set_up_temperature_value--;
        if(gpro_t.set_up_temperature_value<20) gpro_t.set_up_temperature_value=40;
        else if(gpro_t.set_up_temperature_value >40)gpro_t.set_up_temperature_value=40;

        run_t.set_temperature_decade_value = gpro_t.set_up_temperature_value / 10 ;
        run_t.set_temperature_unit_value  =gpro_t.set_up_temperature_value % 10; //


        gpro_t.key_set_dry_flag = 0 ;//  allow open dry function
        run_t.set_temperature_special_value=1;
        run_t.gTimer_key_temp_timing=0;

        //SendData_Set_Command(DRY_ON_NO_BUZZER); =0; //after set temperature allow shut off dry .

        
        TM1639_Write_2bit_SetUp_TempData(run_t.set_temperature_decade_value,run_t.set_temperature_unit_value,0);
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
    

     if(gpro_t.set_up_temperature_value >run_t.gReal_humtemp[1]){ //PTC TURN ON

      if( gpro_t.key_set_dry_flag == 0){ //allow open dry function 
         run_t.gDry =1;
    	
        LED_DRY_ON();
       if(gpro_t.interval_works_ten_minutes_flag ==0){
		   SendData_Set_Command(dry_notice_cmd,0x01);//SendData_Set_Command(DRY_ON_NO_BUZZER);
		   gpro_t.send_ack_cmd = check_ack_dry_notice_on;
		   gpro_t.gTimer_again_send_power_on_off =0;

        }
        
     }
     }
     else{ //PTC turn off 
         run_t.gDry =0;
         LED_DRY_OFF();
      
    	 SendData_Set_Command(dry_notice_cmd,0x0);//SendData_Set_Command(DRY_OFF_NO_BUZZER);
    	 gpro_t.send_ack_cmd = check_ack_dry_notice_off;
		 gpro_t.gTimer_again_send_power_on_off =0;
         }

   }
   else{
        if(run_t.gReal_humtemp[1] >39){

         run_t.gDry =0;
         LED_DRY_OFF();
       
    	 SendData_Set_Command(dry_notice_cmd,0x0);//SendData_Set_Command(DRY_OFF_NO_BUZZER);
    	 gpro_t.send_ack_cmd = check_ack_dry_notice_off;
		 gpro_t.gTimer_again_send_power_on_off =0;
        
         first_one_flag =1;
        }
        else{

           if(first_one_flag==1 && (run_t.gReal_humtemp[1] <39)){

              
                 run_t.gDry =1;
            
                LED_DRY_ON();

                if(gpro_t.interval_works_ten_minutes_flag ==0){
             
        		
        		 SendData_Set_Command(dry_notice_cmd,0x01);//SendData_Set_Command(DRY_ON_NO_BUZZER);
                  gpro_t.send_ack_cmd = check_ack_dry_notice_on;
		          gpro_t.gTimer_again_send_power_on_off =0;

                }
               }
           }

      }
}

/***********************************************************************
*
*Function Name: void works_run_two_hours_handler(void)
*Function :by display pannel of calculate after send to main board 
*
*
*************************************************************************/
void works_run_two_hours_handler(void)
{
    if(gpro_t.gTimer_total_works_two_hours > 7200){
       gpro_t.gTimer_total_works_two_hours =0;

       gpro_t.interval_works_ten_minutes_flag = 1;



    }

    if(gpro_t.interval_works_ten_minutes_flag == 1){

        if(gpro_t.gTimer_total_works_two_hours > 600){
            gpro_t.gTimer_total_works_two_hours =0;
            gpro_t.interval_works_ten_minutes_flag=0;


        }


    }

    


}



#include "bsp.h"




key_types key_t;

/***********************************************************
*
*
*
*
*
***********************************************************/
#if 0
uint8_t KEY_Scan(void)
{
  uint8_t  reval = 0;
  key_t.read = _KEY_ALL_OFF; //0xFF 


    if(AI_KEY_VALUE() ==1 ) //WIFI_KEY_ID = 0x80
	{
		key_t.read &= ~0x80; // 0x1f & 0x7F =  0x7F
	}
    else if(FAN_KEY_VALUE()   ==1 ) //FAN_KEY_ID = 0x10
	{
		  key_t.read &= ~0x10; // 0xFf & 0xEF =  0xEF
	}
	else if(PLASMA_KEY_VALUE()   ==1 ) //PLASMA_KEY_ID = 0x20
	{
		  key_t.read &= ~0x20; // 0xFf & 0xDF =  0xDF
	}
	else if(DRY_KEY_VALUE()  ==1 ) //DRY_KEY_ID = 0x40
	{
		  key_t.read &= ~0x40; // 0xFf & 0xBF =  0xBF
	}

	
	
   
    switch(key_t.state )
	{
		case start:
		{
			if(key_t.read != _KEY_ALL_OFF)
			{
				key_t.buffer   = key_t.read; //??:key.buffer = 0xFE  POWER KEY 
				key_t.state    = first;
				key_t.on_time  = 0;
				key_t.off_time = 0;
            
                
			}
			break;
		}
		case first:
		{
			if(key_t.read == key_t.buffer) //  short  key be down ->continunce be pressed key
			{
				if(++key_t.on_time>35 )//25 //10000  0.5us
				{
					//run_t.power_times++;
                    key_t.value = key_t.buffer^_KEY_ALL_OFF; // key.value = 0xFE ^ 0xFF = 0x01
					key_t.on_time = 0;                      //key .value = 0xEF ^ 0XFF = 0X10
                    key_t.state   = second;
                   
                    
				}
			  
			}
			else
			{
				key_t.state   = start;
			}
			break;
		}
		case second:
		{
			if(key_t.read == key_t.buffer) //long key key if be pressed down 
			{
				if(++key_t.on_time>70000)// 80000 long key be down
				{
				    key_t.value = key_t.value|0x80; //key.value = 0x02 | 0x80  =0x82
                    key_t.on_time = 0;
					key_t.state   = finish;
	               
				}
					
			}
			else if(key_t.read == _KEY_ALL_OFF)  // loose hand 
			{
					if(++key_t.off_time> 0) //20//30 don't holding key dithering
					{
						key_t.value = key_t.buffer^_KEY_ALL_OFF; // key.value = 0x1E ^ 0x1f = 0x01
						
						key_t.state   = finish; // loose hand
					}
			}
		   
			break;
		}
		case finish:
		{
		
			reval = key_t.value; // is short time  TIMER_KEY = 0x01  2. long times TIMER_KEY = 0X81

			key_t.state   = end;
         
			break;
		}
		case end:
		{
			if(key_t.read == _KEY_ALL_OFF)
			{
				if(++key_t.off_time>0)//5//10//50 //100
				{
					key_t.state   = start;
                  
				}
			}
			break;
		}
		default:
		{
			key_t.state   = start;
         
			break;
		}
	}
	return  reval;


}

#endif 

/****************************************************************
	*
	*Function Name :void Set_Timing_Temperature_Number_Value(void)
	*Function : set timer timing how many ?
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
*****************************************************************/
void Set_TimerTiming_Number_Value(void)
{

   if(gpro_t.set_timer_timing_doing_value==1){
   //set timer timing value 
    if(run_t.gTimer_key_timing > 4){
		run_t.gTimer_key_timing =0;		
		
      //  run_t.timer_dispTime_hours =0;
	//	run_t.timer_dispTime_minutes =0;

        gpro_t.set_timer_timing_doing_value  =0 ;
	
		
        run_t.gTimer_timer_timing_counter=0;
	
	 }

    }
}

void set_timer_fun_led_blink(void)
{

   if(gpro_t.set_timer_timing_doing_value==1){

   
    if(run_t.gTimer_smg_timing < 18){
    LED_AI_ON();
    }
    else if(run_t.gTimer_smg_timing > 17 && run_t.gTimer_smg_timing < 35){
    LED_AI_OFF();

    }
    else if(run_t.gTimer_smg_timing > 34){
    run_t.gTimer_smg_timing=0;  
    LED_AI_ON();



    }

   }

}

 
/****************************************************************
	*
	*Function Name :void Set_Temperature_Value(void)
	*Function : set timer timing how many ?
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
*****************************************************************/
void Set_Temperature_Value(void)
{
     static uint8_t counter_times;
	  //waiting for 4 s 
	  if(run_t.gTimer_key_temp_timing > 1 && run_t.set_temperature_special_value ==1){
			
			
			run_t.set_temperature_special_value =2;
			run_t.gTimer_set_temp_times =0; //couter time of smg blink timing 

	 }
	 //temperature of smg of LED blink .
	  if(run_t.set_temperature_special_value ==2){
	  	
	  	
		  if(run_t.gTimer_set_temp_times < 15 ){ // 4
		        TM1639_Write_2bit_SetUp_TempData(0,0,1);
          }
		  else if(run_t.gTimer_set_temp_times > 14 && run_t.gTimer_set_temp_times < 29){
		  	
			  TM1639_Write_2bit_SetUp_TempData(run_t.set_temperature_decade_value,run_t.set_temperature_unit_value,0);

		  }
		  else{
		  	 run_t.gTimer_set_temp_times=0;
             counter_times++ ;  

		  }


           if(counter_times > 3){
			 
           		counter_times=0;
        
			 gpro_t.set_temp_value_success=1;
			 
	         run_t.set_temperature_special_value =0xff;
			  run_t.gTimer_temp_delay = 70; //at once shut down ptc  funciton
			  run_t.gTimer_display_dht11 = 90;
		
			  TM1639_Write_2bit_SetUp_TempData(run_t.set_temperature_decade_value,run_t.set_temperature_unit_value,0);
              compare_temp_value();
              
             }
	     }

     
}









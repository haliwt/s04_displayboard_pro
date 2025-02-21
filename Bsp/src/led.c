#include "bsp.h"


volatile uint32_t led_k,led_i;


void (*panel_led_fun)(void);


static void DRY_LED_OnOff(uint8_t sel);

static void PLASMA_LED_OnOff(uint8_t sel);

static void TIME_LED_OnOff(uint8_t sel);

static void Power_Breath_Two(void);

static uint8_t  Delay(int16_t count);




/***********************************************************
*
*Function Name : static void DRY_LED_OnOff(uint8_t sel)
*
*
*
************************************************************/
static void DRY_LED_OnOff(uint8_t sel)
{
   if(sel==1){
	 LED_DRY_ON();
   }
   else 
   	LED_DRY_OFF();

}
static void PLASMA_LED_OnOff(uint8_t sel)
{
	if(sel==1){
		LED_PLASMA_ON();
   }
   else
   	 LED_PLASMA_OFF();

}

static void TIME_LED_OnOff(uint8_t sel)
{
    if(sel==1)LED_TIME_ON();
	else  LED_TIME_OFF();


}

void KeyLed_Power_On(void)
{
   LED_POWER_ON();
   LED_TIME_ON();


}



void ALL_LED_Off(void)
{
   LED_PLASMA_OFF();
   LED_AI_OFF();
  LED_DRY_OFF();
  LED_TIME_OFF();


}

/*******************************************************************************************
 	*
 	* Function Name:void Panel_Led_OnOff_Function(void)
 	* Function :panel of led turn on or off which key function 
 	* Input Ref:NO
 	* Return Ref:NO
 	* 
*******************************************************************************************/  
void Panel_Led_OnOff_Function(void)
{

  // static uint8_t ai_changed_flag;
   if(run_t.gTimer_run_ico > 0){ //30* 10ms =300ms
		run_t.gTimer_run_ico=0;
		
	  LED_POWER_ON();
	if(run_t.ai_model_flag ==AI_MODE){
         LED_AI_ON();
       

     }
	  else{

		LED_AI_OFF() ;
      }


    if(run_t.gMouse  == 0 ){

       LED_MOUSE_OFF();//mouse_led_onoff(0); //display fan of grass is one 
    } 
    else if(run_t.gMouse  == 1){ // && (run_t.gDry==0 && run_t.gPlasma==0)){ //WT.DEDIT 20223.09.15
      
       LED_MOUSE_ON();    //mouse_led_onoff(1); //display fan of grass is two .
    }
	 
	  
    if(run_t.gDry==1){
		 
	     DRY_LED_OnOff(1);
      
       //  SendData_Set_Command(DRY_ON_NO_BUZZER); //PTC turn On
                 
     }
	 else{
	    DRY_LED_OnOff(0);

	 }

	 if(run_t.gPlasma==1){
	 	
	     PLASMA_LED_OnOff(1);
     }
	 else{
	   PLASMA_LED_OnOff(0);

	 }

	
	
	 

	 if(run_t.time_led_flag ==1){
	    TIME_LED_OnOff(1);
	 }
	 else
	 	TIME_LED_OnOff(0);

    
	}
}
/***************************************************************
*
*Function Name: void LED_TempHum(void)
*Function : display temperature and humidity and times led 
*
*
*
**************************************************************/
static uint8_t  Delay(int16_t count)
{
   
    if(count ==0 || count <0){
       return 1;
    
    }
    else{
    while(count){

       count--;
   }
   }

   return 0;

}

/***************************************************************
*
*Function Name: void LED_TempHum(void)
*Function : display temperature and humidity and times led 
*
*LED_Power_Key_On();
*
**************************************************************/
static void Power_Breath_Two(void)

{
   
     static uint16_t flag,switch_flag,dec_led_flag;
        static uint16_t i,j;
        led_k++;
        
        
        if(led_k <250 && dec_led_flag==1){
            i++;
         if(switch_flag ==1 || i > 40){
            switch_flag =0;
          
            //LED_POWER_OFF();
            LED_POWER_OFF();
            flag = Delay(250-i);
           
          }
          if(flag ==1){
            flag =0;
             LED_POWER_ON();
             j=i;
             if(j< 5){
               switch_flag = Delay(j);
    
             }
             switch_flag =1;
          
    
            }
          
        }
        else if(led_k < 250 && dec_led_flag==0){
            i++;
         if(switch_flag ==1 || i < 40){
            switch_flag =0;
        #if 0
            LED_POWER_OFF();
            LED_POWER_OFF();
            LED_Power_On();
            flag = Delay(30-i);
        #endif 
            LED_POWER_ON();
            flag = Delay(250-i);
            
    
          }
          if(flag ==1){
            flag =0;
          
               if(j< 40){
               //LED_POWER_OFF();

                LED_POWER_OFF();
               switch_flag = Delay(i);
    
                }
                else{
                // LED_POWER_OFF();
                 //LED_POWER_OFF();
                 //LED_POWER_OFF();
                /// LED_POWER_OFF();

                 LED_POWER_OFF();
                 LED_POWER_OFF();
                LED_POWER_OFF();
                LED_POWER_OFF();
                 switch_flag = 1;
    
    
                }
            }
         }
        
        if(led_k > 249 && dec_led_flag==0){
    
        
    
    //          z++; 
    //      if(z<20){
    //         LED_POWER_OFF();
    //         
    //      }
    //      else{
        
            led_k=0;
             i=0;
          
             dec_led_flag=1;
            
         //  }
            
          }
          else if(led_k>249 && dec_led_flag==1){
            
         
           
            led_k=0;
             i=0;
        
             dec_led_flag=0;
            
    //        if(z<20){
    //         // LED_POWER_OFF();
    //         LED_Power_On();
    //         }
    //         else{
    //       
    //      led_k=0;
    //         i=0;
    //         z=0;
    //         dec_led_flag=0;
    //        }
        }
   

    
      

     

}

#if 0
static void Power_Breath_Two(void)
{
    static uint32_t i,j;
    led_k++;

	if(led_k<101){
        i=0;
        j=0;
		LED_POWER_ON();
		Delay(led_k);
	  // LED_POWER_OFF();
	  
       LED_POWER_OFF();
       Delay(200-led_k);

    }
    if(led_k>99 && led_k <201){
        j++;
		LED_POWER_OFF();
		Delay(j);
        LED_POWER_ON();
        Delay(100-j);
        

    }
    else if(led_k>299 && led_k <401){
		led_i++;

	  
	   LED_POWER_ON();
       LED_POWER_OFF();
	   Delay(200 -led_i);
      

	}
    else if(led_k>399){
        led_k =300;
        i++; 
      if(i<500){
          LED_POWER_OFF();
      }
      else{
        led_i=0;
		led_k=0;
        
        
      }
	}
	

}

static void Delay(int16_t count)
{
   
    if(count ==0 || count <0){
       return;
    
    }
    else{
    while(count){

       count--;
   }
   }

}
#endif 

#if 0
static void Power_BreathOnOff(void)
{
    
     static uint32_t k;
   
    k++;

	if(k<40000){

	LED_POWER_ON();

    }
          

	if(k>40000 && k <80000){

	    LED_POWER_OFF();

	}
	if(k>80000 && k< 120000){

	  LED_POWER_TOGGLE();


	}

	if(k>120000){

        k=0;
	}

}

#endif 


void Breath_Led(void)
{
    
   
   // Power_BreathOnOff();
   Power_Breath_Two();


   
}



/***************************************************************
*
*Function Name: void Panel_Led_OnOff_RunCmd(void (*panelledHandler)(void))
*Function : display temperature and humidity and times led 
*
*
*
**************************************************************/
void Panel_Led_OnOff_RunCmd(void (*panelledHandler)(void))
{

	 panel_led_fun= panelledHandler;
}

void Power_Off_Led_Off(void)
{
   LED_AI_OFF();
   LED_DRY_OFF();
   LED_TIME_OFF();
   LED_PLASMA_OFF();
   LED_MOUSE_OFF();
   LED_POWER_OFF();
   SMG_POWER_OFF();

}

void Power_ON_Led(void)
{
   LED_AI_ON();
   LED_DRY_ON();
   LED_TIME_ON();
   LED_PLASMA_ON();
   LED_MOUSE_ON();
   LED_POWER_ON();
   SMG_POWER_ON();

}

/*
 * bsp_message.c
 *
 *  Created on: 2025年2月19日
 *      Author: Administrator
 */
#include "bsp.h"

/******************************************************************************
*
*Function Name:void send_cmd_ack_hanlder(void)
*Funcion: handle of tall process
*Input Ref:
*Return Ref:
*
******************************************************************************/
void send_cmd_ack_hanlder(void)
{

   switch(gpro_t.send_ack_cmd){

    case ack_null:


    break;

    case ack_power_on:

      if(gpro_t.receive_copy_cmd == ack_power_on){
         gpro_t.receive_copy_cmd =0;
         gpro_t.send_ack_cmd = 0;

      }
      else if(gpro_t.receive_copy_cmd != 0 && gpro_t.gTimer_again_send_power_on_off >1){
         gpro_t.gTimer_again_send_power_on_off =0;
         SendData_PowerOnOff(1);
      }


    break;

    case ack_power_off :

     if(gpro_t.receive_copy_cmd == ack_power_off){
        gpro_t.receive_copy_cmd =0;
         gpro_t.send_ack_cmd = 0;

      }
      else if(gpro_t.receive_copy_cmd != 0 && gpro_t.gTimer_again_send_power_on_off >1){
          gpro_t.gTimer_again_send_power_on_off =0;
          SendData_PowerOnOff(0);
      }


    break;

    case ack_ptc_on:

    if(gpro_t.receive_copy_cmd == ack_ptc_on){
			gpro_t.receive_copy_cmd =0;
			 gpro_t.send_ack_cmd = 0;

    	}
    	else if(gpro_t.receive_copy_cmd != 0 && gpro_t.gTimer_again_send_power_on_off >1){
			  gpro_t.gTimer_again_send_power_on_off =0;
			  SendData_Set_Command(dry_cmd,0x01); // link wifi of command .
    	}


    break;

    case ack_ptc_off:

   if(gpro_t.receive_copy_cmd == ack_ptc_off){
	   gpro_t.receive_copy_cmd =0;
	   gpro_t.send_ack_cmd = 0;

	}
	else if(gpro_t.receive_copy_cmd != 0 && gpro_t.gTimer_again_send_power_on_off >1){
		gpro_t.gTimer_again_send_power_on_off =0;
		SendData_Set_Command(dry_cmd,0x0); // link wifi of command .
	}


    break;

    case ack_plasma_on:

    if(gpro_t.receive_copy_cmd == ack_plasma_on){
			gpro_t.receive_copy_cmd =0;
			 gpro_t.send_ack_cmd = 0;
    }
    else if(gpro_t.receive_copy_cmd != 0 && gpro_t.gTimer_again_send_power_on_off >1){
			  gpro_t.gTimer_again_send_power_on_off =0;
			  SendData_Set_Command(plasma_cmd,0x01); // link wifi of command .
    }
    break;

    case ack_plasma_off:

    if(gpro_t.receive_copy_cmd == ack_plasma_off){
			gpro_t.receive_copy_cmd =0;
			 gpro_t.send_ack_cmd = 0;
    }
    else if(gpro_t.receive_copy_cmd != 0 && gpro_t.gTimer_again_send_power_on_off >1){
			  gpro_t.gTimer_again_send_power_on_off =0;
			  SendData_Set_Command(plasma_cmd,0x0); // link wifi of command .
    }
    break;

    case ack_ultra_on:
    if(gpro_t.receive_copy_cmd == ack_ultra_on){
			gpro_t.receive_copy_cmd =0;
			 gpro_t.send_ack_cmd = 0;
    }
    else if(gpro_t.receive_copy_cmd != 0 && gpro_t.gTimer_again_send_power_on_off >1){
			  gpro_t.gTimer_again_send_power_on_off =0;
			  SendData_Set_Command(mouse_cmd,0x01); // link wifi of command .
    }
    break;

    case ack_ultra_off:
    if(gpro_t.receive_copy_cmd == ack_ultra_off){
			gpro_t.receive_copy_cmd =0;
			 gpro_t.send_ack_cmd = 0;
    }
    else if(gpro_t.receive_copy_cmd != 0 && gpro_t.gTimer_again_send_power_on_off >1){
			  gpro_t.gTimer_again_send_power_on_off =0;
			  SendData_Set_Command(mouse_cmd,0x0); // link wifi of command .        
    }
    break;

    case ack_ai_on:   

    if(gpro_t.receive_copy_cmd == ack_ai_on){
			gpro_t.receive_copy_cmd =0;
			 gpro_t.send_ack_cmd = 0;        
    }
    else if(gpro_t.receive_copy_cmd != 0 && gpro_t.gTimer_again_send_power_on_off >1){
			  gpro_t.gTimer_again_send_power_on_off =0;
			  SendData_Set_Command(ai_cmd,0x01); // link wifi of command .
    }
    break;

    case ack_ai_off:
    if(gpro_t.receive_copy_cmd == ack_ai_off){
			gpro_t.receive_copy_cmd =0;
			 gpro_t.send_ack_cmd = 0;   
    }
    else if(gpro_t.receive_copy_cmd != 0 && gpro_t.gTimer_again_send_power_on_off >1){
			  gpro_t.gTimer_again_send_power_on_off =0;
			  SendData_Set_Command(ai_cmd,0x0); // link wifi of command .
    }
    break;


    case ack_buzzer_sound:

        if(gpro_t.receive_copy_cmd == ack_buzzer_sound){
            gpro_t.receive_copy_cmd =0;
            gpro_t.send_ack_cmd = 0;

        }
        else if(gpro_t.receive_copy_cmd != 0 && gpro_t.gTimer_again_send_power_on_off >1){
            gpro_t.gTimer_again_send_power_on_off =0;
            SendData_Set_Command(0x06,0x01); //buzzer sound command .

        }

    break;

  }

}
/******************************************************************************
*
*Function Name:void receive_data_fromm_mainboard(uint8_t *pdata,uint8_t len)
*Funcion: handle of tall process
*Input Ref:
*Return Ref:
*
******************************************************************************/
void receive_data_fromm_mainboard(uint8_t *pdata)
{
    

    switch(pdata[2]){

     case 0:


     break;

     case dry_cmd: //PTC打开关闭指令

     if(pdata[3] == 0x01){

            run_t.gDry =1 ;//&& run_t.gPlasma ==1  && run_t.gUltransonic==1
           
        }
        else if(pdata[3] == 0x0){

          
            run_t.gDry =0;
          

        }

     break;

     case plasma_cmd: //PLASMA 打开关闭指令

        if(pdata[3] == 0x01){
          
        run_t.gPlasma =1;


        }
        else if(pdata[3] == 0x0){
          
         run_t.gPlasma =0;

        }


     break;


      case mouse_cmd: //ultrasonic  打开关闭指令

        if(pdata[3] == 0x01){  //open
           
           run_t.gMouse = 1;

        }
        else if(pdata[3] == 0x0){ //close
         run_t.gMouse = 0;
        }


     break;




      case temp_warning: //temperature of high warning.

        if(pdata[3] == 0x01){  //warning

            run_t.ptc_warning = 1;
            //run_t.setup_timer_timing_item =  PTC_WARNING; //ptc warning
           

            run_t.gDry =0;
            SendData_Set_Command(0x22,0x0); //close ptc ,but don't buzzer sound .

        }
        else if(pdata[3] == 0x0){ //close

           run_t.ptc_warning = 0;


        }



      break;

      case 0x09: //fan of default of warning.

         if(pdata[3] == 0x01){  //warning

            run_t.fan_warning = 1;

           run_t.gDry =0;
           SendData_Set_Command(0x22,0x0); //close ptc ,but don't buzzer sound .

        }
        else if(pdata[3] == 0x0){ //close

           run_t.fan_warning = 0;


        }


      break;


     //接收的是数据

      case temp_hum_data: //温度,湿度数据

        if(pdata[4] == 0x02){ //数据
            
            run_t.gReal_humtemp[0] = pdata[5] ;//gpro_t.humidity_real_value = pdata[5];
           
            
             //gpro_t.temp_real_value = pdata[6];
            
             run_t.gReal_humtemp[1] = pdata[6] ;// temperature value 
           

            

        }
        else if(pdata[4] == 0x01){ //数据)



        }
      break;

      case 0x1B: //湿度数据

        if(pdata[2] == 0x0F){ //数据


        }
      break;

      case beijing_times_data: //表示时间：小时，分，秒

        if(pdata[4] == 0x03){ //数据

            if(pdata[5] < 24){ //WT.EDIT 2024.11.23

//            lcd_t.display_beijing_time_flag= 1;

//            run_t.dispTime_hours  =  pdata[5];
//            run_t.dispTime_minutes = pdata[6];
//            run_t.gTimer_disp_time_seconds =  pdata[7];
           }


        }
      break;

        case 0x1D: //表示日期： 年，月，日

        if(pdata[2] == 0x0F){ //数据



        }
      break;

     case copy_cmd: // copy send cmd acknowlege
          //power on or power off
          if(pdata[3]==0x01){ //power on or power off cmd.
              if(pdata[4]==1){ //power on

                 gpro_t.receive_copy_cmd = ack_power_on;
                 do{

                     power_on_handler();
                     run_t.gPower_On = power_on;
                 }while(0);



              }
              else if(pdata[4]==2){ //power offf

                 gpro_t.receive_copy_cmd = ack_power_off;

                run_t.gPower_On = power_off;
               	run_t.gRunCommand_label =RUN_NULL;


              }

          }
          else if(pdata[3] == 0x05){ //link wifi command .

              if(pdata[4]==1){

                 gpro_t.receive_copy_cmd = ack_wifi_on;
                 

              }


          }
          else if(pdata[3] == 0x16){  //buzzer answer command

                gpro_t.receive_copy_cmd = ack_buzzer_sound;

           }


     break;

     }

 }



/******************************************************************************
*
*Function Name:uint8_t bcc_check(const unsigned char *data, int len)
*Funcion: BCC check code
*Input Ref: data:input data, len: data of length
*Return Ref:
*
******************************************************************************/
uint8_t bcc_check(const unsigned char *data, int len)
{
    unsigned char bcc = 0;
    for (int i = 0; i < len; i++) {
        bcc ^= data[i];
    }
    return bcc;
}




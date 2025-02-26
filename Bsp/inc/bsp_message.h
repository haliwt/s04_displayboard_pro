/*
 * bsp_message.h
 *
 *  Created on: 2025年2月19日
 *      Author: Administrator
 */

#ifndef BSP_INC_BSP_MESSAGE_H_
#define BSP_INC_BSP_MESSAGE_H_
#include "main.h"

typedef enum ack_sig{

  ack_null,
  ack_power,
  ack_ptc,
  ack_plasma,
  ack_mouse,
  ack_wifi=0x05,
 // ack_buzzer,
  ack_ai=0x07,
  ack_temp_warning = 0x08,
  ack_fan_warning ,
  
  ack_with_buzzer=0x16,
  ack_dry_notice=0x22,



}ack_sit_e;


typedef enum{

    check_ack_null,
    check_ack_power_on=0x01,
    check_ack_power_off = 0x03,
    check_ack_ptc_on,
    check_ack_ptc_off,
    check_ack_plasma_on,
    check_ack_plasma_off,
    check_ack_mouse_on,
    check_ack_mouse_off,
    check_ack_ai_on,
    check_ack_ai_off,
    check_ack_ptc_warning,
    check_ack_fan_warning,
    check_ack_buzzer,
    check_ack_dry_notice_on,
    check_ack_dry_notice_off
        
}message_info;


typedef enum{

   ack_ok_on=0x01,
   ack_ok_off,


}message_ack_info;

typedef enum{

	none_cmd,
	power_cmd=0x01,
	dry_cmd=0x02,
	plasma_cmd,
	mouse_cmd,
	wifi_cmd,
	buzzer_cmd =0x06,
	ai_cmd = 0x07,
	temp_warning,
	fan_warning,
	temp_hum_data = 0x1A,
	beijing_times_data =0x1C,
	dry_notice_cmd= 0x22, //don't sound
	copy_cmd = 0xff


}message_cmd;



void receive_data_from_mainboard(uint8_t *pdata);


void send_cmd_ack_hanlder(void);

uint8_t bcc_check(const unsigned char *data, int len);




#endif /* BSP_INC_BSP_MESSAGE_H_ */

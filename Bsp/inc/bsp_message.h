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
  ack_power_on ,
  ack_power_off,
  ack_wifi_on,
  ack_ptc_on,
  ack_ptc_off,
  ack_plasma_on,
  ack_plasma_off,
  ack_mouse_on,
  ack_mouse_off=0x09,
  ack_ai_on,
  ack_ai_off,
  ack_dry_notice_on,
  ack_dry_notice_off,
  ack_buzzer_sound


}ack_sit_e;


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


}message_info;


void receive_data_fromm_mainboard(uint8_t *pdata);


void send_cmd_ack_hanlder(void);

uint8_t bcc_check(const unsigned char *data, int len);




#endif /* BSP_INC_BSP_MESSAGE_H_ */

#ifndef __BSB_FAN_H
#define  __BSP_FAN_H
#include "main.h"



#define LED_FAN_ON()  				HAL_GPIO_WritePin(FAN_LED_EN_GPIO_Port, FAN_LED_EN_Pin,GPIO_PIN_SET)
#define LED_FAN_OFF()				HAL_GPIO_WritePin(FAN_LED_EN_GPIO_Port, FAN_LED_EN_Pin,GPIO_PIN_RESET)


void led_mouse_on(void);
void led_mouse_off(void);

void mouse_led_onoff(uint8_t sel);



#endif 


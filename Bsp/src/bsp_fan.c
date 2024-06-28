#include "bsp.h"


void led_mouse_on(void)
{
   LED_FAN_ON() ;


}
void led_mouse_off(void)
{

 LED_FAN_OFF() ;

}


 void mouse_led_onoff(uint8_t sel)
{

	if(sel==1)LED_FAN_ON();
	else  LED_FAN_OFF();


}






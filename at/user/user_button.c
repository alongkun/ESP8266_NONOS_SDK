#include "user_interface.h"

#include "user_button.h"

void userWifiStatucLedInit(void)
{
	wifi_status_led_install(USER_WIFI_LED_IO_NUM, USER_WIFI_LED_IO_MUX, USER_WIFI_LED_IO_FUNC);
}


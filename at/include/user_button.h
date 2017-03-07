#ifndef __USER_BUTTON_H__
#define __USER_BUTTON_H__

#include "eagle_soc.h"

#define USER_WIFI_LED_IO_MUX	PERIPHS_IO_MUX_GPIO0_U
#define USER_WIFI_LED_IO_NUM	0
#define USER_WIFI_LED_IO_FUNC	FUNC_GPIO0

void userWifiStatusLedInit(void);

#endif


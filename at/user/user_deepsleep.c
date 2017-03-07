#include "osapi.h"
#include "user_interface.h"

#include "user_deepsleep.h"

void userModemSleepWakeUpCB(void)
{
	os_printf("*****Wake up from Modem Sleep\r\n");

	wifi_fpm_do_wakeup();
	wifi_fpm_close();
	wifi_set_opmode(STATION_MODE);
	wifi_station_connect();
}

void userLightSleepWakeUpCB(void)
{
	os_printf("*****Wake up from Light Sleep\r\n");

	wifi_fpm_close();
	wifi_set_opmode(STATION_MODE);
	wifi_station_connect();
}


void userEnterModemSleep(uint32 sec)
{
	uint32 sleepTime;

	os_printf("*****Enter Modem Sleep\r\n");

	wifi_station_disconnect();
	wifi_set_opmode(NULL_MODE);
	wifi_fpm_set_sleep_type(MODEM_SLEEP_T);
	wifi_fpm_open();


#if USER_SELLP_WAKEUP_BY_PGIO	//wake up by wifi_fpm_do_wakeup()
	wifi_fpm_do_sleep(0xFFFFFFF);

#else											//until time up
	if(sec == 0xFFFFFFF)
		sleepTime = 0xFFFFFFF;
	else if(sec > 268)
		sleepTime = (0xFFFFFFF - 1);
	else	//0 - 268
		sleepTime = FPM_SELLP_TIME(sec);

	wifi_fpm_set_wakeup_cb(userModemSleepWakeUpCB);
	wifi_fpm_do_sleep(sleepTime);
#endif
}

void userEnterLightSleep(uint32 sec)
{
	uint32 sleepTime;

	os_printf("*****Enter Light Sleep\r\n");

	wifi_station_disconnect();
	wifi_set_opmode(NULL_MODE);
	wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
	wifi_fpm_open();

#if USER_SELLP_WAKEUP_BY_PGIO
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);
	wifi_enable_gpio_wakeup(12, GPIO_PIN_INTR_LOLEVEL);

	wifi_fpm_set_wakeup_cb(userLightSleepWakeUpCB);
	wifi_fpm_do_sleep(0xFFFFFFF);

#else	//until time up
	if(sec == 0xFFFFFFF)
		sleepTime = 0xFFFFFFF;
	else if(sec > 268)
		sleepTime = (0xFFFFFFF - 1);
	else	//0 - 268
		sleepTime = FPM_SELLP_TIME(sec);

	wifi_fpm_set_wakeup_cb(userLightSleepWakeUpCB);
	wifi_fpm_do_sleep(sleepTime);

#endif
}

void userEnterDeepSleep(uint32 sec)
{
	uint32 sleepTime;

	os_printf("*****Enter Deep Sleep\r\n");

	system_deep_sleep_set_option(1);

	if(sec == 0xFFFFFFF)
		sleepTime = 0xFFFFFFF;
	else if(sec > 268)
		sleepTime = (0xFFFFFFF - 1);
	else	//0 - 268
		sleepTime = FPM_SELLP_TIME(sec);

	system_deep_sleep(sleepTime);
}



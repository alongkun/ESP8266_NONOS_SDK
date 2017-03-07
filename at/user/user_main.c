/*
 * ESPRSSIF MIT License
 *
 * Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#include "osapi.h"
#include "at_custom.h"
#include "user_interface.h"

#include "user_connection.h"
#include "user_smartconfig.h"

/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABBBCDDD
 *                A : rf cal
 *                B : at parameters
 *                C : rf init data
 *                D : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 ICACHE_FLASH_ATTR
user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 8;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

void ICACHE_FLASH_ATTR
user_rf_pre_init(void)
{

}

void ICACHE_FLASH_ATTR
userSystemInitDone(void)
{
	os_printf("*****running in userSystemInitDone\r\n");

	userConnectionInit();
}

void ICACHE_FLASH_ATTR
user_init(void)
{
	os_printf("*****running in user_init\r\n");

	wifi_station_set_auto_connect(1);
	wifi_station_set_reconnect_policy(true);
	wifi_set_event_handler_cb(userWifiEventHandlerCB);

	system_init_done_cb(userSystemInitDone);

	os_printf("*****user init done\r\n");
//	wifi_station_disconnect();
//	wifi_set_opmode_current(NULL_MODE);
//	wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
//	wifi_fpm_open();
//
//	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U,3);
//	gpio_pin_wakeup_enable(13,GPIO_PIN_INTR_LOLEVEL);
//
//	wifi_fpm_set_wakeup_cb(fpm_wakeup_cn);
//	wifi_fpm_do_sleep(5*1000*1000);

//	user_esp_platform_init();
//	os_printf("user_esp_platform_init\r\n");
}

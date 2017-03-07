#include "user_interface.h"
#include "osapi.h"
#include "string.h"
#include "mem.h"

#include "user_connection.h"
#include "user_smartconfig.h"
#include "user_at.h"
#include "user_tcpclient.h"

bool ICACHE_FLASH_ATTR
userConnConfigExist(void)
{
	struct station_config *pConfig = (struct station_config *) os_zalloc(sizeof(struct station_config));
	bool existFlag = false;

	wifi_station_get_config_default(pConfig);
	if(strlen(pConfig->ssid))
		existFlag = true;

	os_free(pConfig);

	return existFlag;
}

void ICACHE_FLASH_ATTR
userCheckWifiStatus(void)
{
	os_printf("*****check wifi status : ");

	switch(wifi_station_get_connect_status())
	{
	case STATION_IDLE:
		os_printf("STATION_IDLE. \r\n");
		break;
	case STATION_CONNECTING:
		os_printf("STATION_CONNECTING. \r\n");
		break;
	case STATION_WRONG_PASSWORD:
		os_printf("STATION_WRONG_PASSWORD. \r\n");
		break;
	case STATION_NO_AP_FOUND:
		os_printf("STATION_NO_AP_FOUND. \r\n");
		break;
	case STATION_CONNECT_FAIL:
		os_printf("STATION_CONNECT_FAIL. \r\n");
		break;
	case STATION_GOT_IP:
		os_printf("STATION_GOT_IP. \r\n");
		break;
	default:
		break;
	}

}

void ICACHE_FLASH_ATTR
userWifiEventHandlerCB(System_Event_t *evt)
{
	os_printf("*****wifi event : ");

	switch(evt->event)
	{
	case EVENT_STAMODE_CONNECTED:
		os_printf("EVENT_STAMODE_CONNECTED. \r\n");
		break;
	case EVENT_STAMODE_DISCONNECTED:
		os_printf("EVENT_STAMODE_DISCONNECTED. \r\n");
		break;
	case EVENT_STAMODE_GOT_IP:
		os_printf("EVENT_STAMODE_GOT_IP. \r\n");
		userTcpClientInit("192.168.2.66",2333);
		userAtInit();
		break;
	case EVENT_STAMODE_AUTHMODE_CHANGE:
		os_printf("EVENT_STAMODE_AUTHMODE_CHANGE. \r\n");
		break;
	case EVENT_STAMODE_DHCP_TIMEOUT:
		os_printf("EVENT_STAMODE_DHCP_TIMEOUT. \r\n");
		break;
	default:
		break;
	}

}

void ICACHE_FLASH_ATTR
userConnectionInit(void)
{
	os_printf("*****running in userConnectionInit\r\n");

	if(wifi_get_opmode_default() != STATION_MODE)		//是否节点模式
		wifi_set_opmode(STATION_MODE);

	if(!userConnConfigExist())							//是否存在wifi连接记录,不存在则smartConfig
	{
		os_printf("*****No Conn Info,enter smart config\r\n");
		userSmartConfigInit();
	}
	else
	{
		os_printf("*****Exist Conn Info\r\n");
	}

}


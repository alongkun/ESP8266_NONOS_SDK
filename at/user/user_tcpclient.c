#include "osapi.h"
#include "user_interface.h"
#include "espconn.h"
#include "mem.h"
#include "at_custom.h"

#include "user_tcpclient.h"

static struct espconn *espconnPtr = NULL;

static uint8 userAtTcpBuffer[USER_AT_TCP_BUFFER_SIZE];
static uint32 userAtTcpDataLen = 0;
static bool userAtTcpSendFlag = FALSE;

void ICACHE_FLASH_ATTR
userTcpClientConnectCB(void *arg)
{
	os_printf("Tcp Clinet connected\r\n");
	espconn_set_opt((struct espconn*)arg,ESPCONN_COPY);
	userAtTcpSendFlag = TRUE;
	userAtTcpDataLen = 0;

	at_fake_uart_enable(true, userAtFakeUartTxFunc);
}

void ICACHE_FLASH_ATTR
userTcpClientReconCB(void *arg, sint8 errType)
{
	struct espconn *espconn_ptr = (struct espconn *)arg;

	os_printf("Tcp Clinet Reconnect\r\n");
	userAtTcpSendFlag = FALSE;
    espconn_connect(espconn_ptr);
}

void ICACHE_FLASH_ATTR
userTcpClientDisconCB(void *arg)
{
	struct espconn *espconn_ptr = (struct espconn *)arg;

	os_printf("Tcp Clinet Disconnected\r\n");
	userAtTcpSendFlag = FALSE;
	espconn_connect(espconn_ptr);

	at_fake_uart_enable(false, userAtFakeUartTxFunc);
}

void ICACHE_FLASH_ATTR
userTcpClientRecvCB(void *arg, char *pusrdata, unsigned short len)
{
	at_fake_uart_rx(pusrdata, len);
}

void ICACHE_FLASH_ATTR
userTcpClientSendCB(void *arg)
{
	userAtTcpSendFlag = TRUE;
	if(userAtTcpDataLen) {
		espconn_send(espconnPtr,userAtTcpBuffer,userAtTcpDataLen);
		userAtTcpDataLen = 0;
	}
}

void ICACHE_FLASH_ATTR
userAtFakeUartTxFunc(const uint8 * data,uint32 length)
{
	if((data == NULL) || (length == 0)) {
		return;
	}

	if(userAtTcpSendFlag) {
		espconn_send(espconnPtr,(uint8*)data,length);
		userAtTcpSendFlag = FALSE;
	} else {
		if(length <= (USER_AT_TCP_BUFFER_SIZE - userAtTcpDataLen)) {
			os_memcpy(userAtTcpBuffer + userAtTcpDataLen, data, length);
			userAtTcpDataLen += length;
		} else {
			os_printf("at espconn buffer full\r\n");
		}
	}
}

void ICACHE_FLASH_ATTR
userTcpClientInit(char * ipAddr, int32_t port)
{
	uint32 ip = 0;

	os_printf("*****running in userTcpClientInit\r\n");

	espconnPtr = (struct espconn *)os_zalloc(sizeof(struct espconn));
	espconnPtr->type = ESPCONN_TCP;
	espconnPtr->state = ESPCONN_NONE;
	espconnPtr->proto.tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp));
	espconnPtr->proto.tcp->local_port = espconn_port();
	espconnPtr->proto.tcp->remote_port = 2333;

	ip = ipaddr_addr(ipAddr);
	os_memcpy(espconnPtr->proto.tcp->remote_ip, &ip, sizeof(ip));

	espconn_regist_connectcb(espconnPtr, userTcpClientConnectCB);
	espconn_regist_reconcb(espconnPtr, userTcpClientReconCB);
	espconn_regist_disconcb(espconnPtr, userTcpClientDisconCB);
	espconn_regist_recvcb(espconnPtr, userTcpClientRecvCB);
	espconn_regist_sentcb(espconnPtr, userTcpClientSendCB);

	espconn_connect(espconnPtr);

}

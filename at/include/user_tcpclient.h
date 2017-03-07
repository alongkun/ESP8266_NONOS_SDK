#ifndef __USER_TCPCLINET_H__
#define __USER_TCPCLIENT_H__

#define USER_AT_TCP_BUFFER_SIZE 	(2920)

void userTcpClientInit(char * ipAddr, int32_t port);
void userAtFakeUartTxFunc(const uint8 * data,uint32 length);

#endif


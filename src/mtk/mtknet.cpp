// Copyright (c) Arduino. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "mtknet.h"

// #define DEBUG_STREAM Serial.print
// #define INFO_STREAM Serial.print

extern const char* CONNECT_IP_ADDRESS;
extern VMINT CONNECT_PORT;

#define C_ADDRESS CONNECT_IP_ADDRESS
#define C_PORT CONNECT_PORT


int mtk_net_tcp(int* mtk_server_fd)
{

	SOCKADDR_IN       t_addr_in = { 0 };
	int               t_ai_socktype;   /* Socket type. */
	int               t_ai_protocol;   /* Protocol of socket. */
	uint32_t          t_ai_addrlen;    /* Length of socket address. */


	int ret_connect = -1;
	int ret = -1;

//	static int server_fd;






	Serial.println("ok");
	char portBuffer[6];
	//sprintf(portBuffer, "%d", params.DestinationPort); DEBUG("  . Connecting to %s/%s...", params.pDestinationURL, portBuffer);*/

	Serial.print("mtk net cpp in Connecting to server ");
	Serial.print(C_ADDRESS);
	Serial.print("/");
	Serial.print(C_PORT);
	Serial.println("...");
	//Serial.print("going to connect in mbedtls");
	t_addr_in.sin_family = PF_INET;  //IPv4
	t_addr_in.sin_addr.S_un.s_addr = inet_addr(C_ADDRESS);
	t_addr_in.sin_port = htons(C_PORT);

	t_ai_socktype = SOCK_STREAM;
	//t_ai_protocol = proto == MBEDTLS_NET_PROTO_UDP ? VM_UDP : VM_TCP;
	t_ai_protocol = 0;
	t_ai_addrlen = sizeof(SOCKADDR);
	*mtk_server_fd = (int)vm_socket(t_addr_in.sin_family, t_ai_socktype, t_ai_protocol);
	if (*mtk_server_fd < 0)
	{
//		ret_connect = MBEDTLS_ERR_NET_SOCKET_FAILED;
		Serial.print("vm_socket error and ret value is ");
		Serial.println(*mtk_server_fd);
		return ret;
	}

	ret = vm_connect(*mtk_server_fd, (SOCKADDR*)&t_addr_in, sizeof(SOCKADDR));

	if (ret != 0){
//		ret_connect = MBEDTLS_ERR_NET_CONNECT_FAILED;
		Serial.print("vm_connect error and ret value is ");
		Serial.println(ret);
		return ret;
	}

	Serial.println("ok");

	return 0;
}


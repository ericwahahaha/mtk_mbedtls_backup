// Copyright (c) Arduino. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "mtknet.h"
#include "dns.h"

#include <LTask.h>

// #define DEBUG_STREAM Serial.print
// #define INFO_STREAM Serial.print

extern const char* CONNECT_IP_ADDRESS;
extern VMINT CONNECT_PORT;
const char* MTK_CONNECT_IP_ADDRESS;
boolean dns_resolve_status = false;


#define C_ADDRESS CONNECT_IP_ADDRESS
#define C_PORT CONNECT_PORT


int mtk_net_tcp(int* mtk_server_fd, const char *mtk_host, const char *mtk_port, int mtk_proto)
{

	SOCKADDR_IN       t_addr_in = { 0 };
	int               t_ai_socktype;   /* Socket type. */
	int               t_ai_protocol;   /* Protocol of socket. */
	uint32_t          t_ai_addrlen;    /* Length of socket address. */

	int ret_connect = -1;
	int ret = -1;
	char mtk_hostname[255];
	char *target_ipaddr;
	IN_ADDR mtk_addr;


	Serial.println("ok");
	char portBuffer[6];
	//sprintf(portBuffer, "%d", params.DestinationPort); DEBUG("  . Connecting to %s/%s...", params.pDestinationURL, portBuffer);*/


	strcpy(mtk_hostname, (char *)mtk_host);
	Serial.print("String copy : ");
	Serial.println(mtk_hostname);
	ret = ngethostbyname((unsigned char*)mtk_hostname, 1, &mtk_addr);
	Serial.print("Result Addr : ");
	Serial.println(inet_ntoa(mtk_addr));
	//mtk_host = (const char *)mtk_host;

	Serial.print("mtk net cpp in Connecting to server ");
	Serial.print(inet_ntoa(mtk_addr));
	Serial.print("/");
	Serial.print(mtk_port);
	Serial.println("...");
	//Serial.print("going to connect in mbedtls");
	t_addr_in.sin_family = PF_INET;  //IPv4
	t_addr_in.sin_addr.S_un.s_addr = inet_addr(inet_ntoa(mtk_addr));
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
	Serial.print("mtk vm_socket fd value is ");
	Serial.println(*mtk_server_fd);

	ret = vm_connect(*mtk_server_fd, (SOCKADDR*)&t_addr_in, sizeof(SOCKADDR));

	if (ret != 0){
//		ret_connect = MBEDTLS_ERR_NET_CONNECT_FAILED;
		Serial.print("vm_connect error and ret value is ");
		Serial.println(ret);
		return ret;
	}

	Serial.println("ok");

	return ret;
}


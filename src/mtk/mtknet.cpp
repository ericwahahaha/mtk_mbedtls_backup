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

VMINT mtk_Resolve_Callback(vm_soc_dns_result *pDNS)
{
	//C_ADDRESS = (const char*)&pDNS->address[0];
	IN_ADDR addr;
	addr.S_un.s_addr = pDNS->address[0];
	MTK_CONNECT_IP_ADDRESS = inet_ntoa(addr);
	Serial.println("wifiResolveCallback");
	Serial.print("ip address is ");
	Serial.println(MTK_CONNECT_IP_ADDRESS);
	dns_resolve_status = true;
//	LTask.mutexUnlock();
	LTask.post_signal();
	return 0;
}


boolean mtk_Resolve_DomainName(void *userData)
{
	VMCHAR *domainName = (VMCHAR *)userData;
	vm_soc_dns_result dns;
	IN_ADDR addr;

	Serial.print("in wifiResolveDomainName, host name is ");
	Serial.println(domainName);

	

	VMINT resolveState;
	if (vm_wifi_is_connected()){
		resolveState = vm_soc_get_host_by_name(VM_TCP_APN_WIFI,
			"www.google.com",
			&dns,
			&mtk_Resolve_Callback);
		Serial.flush();
	}
	else{
		Serial.flush();
		resolveState = vm_soc_get_host_by_name(6,
			domainName,
			&dns,
			&mtk_Resolve_Callback);
		Serial.flush();
	}

//	LTask.mutexLock();
/*	while (MTK_CONNECT_IP_ADDRESS == NULL){
	}*/

	if(resolveState > 0)
	{
		Serial.print("in wifiResolveDomainName, Waiting for response");
		Serial.println(resolveState);
		return false;
	}

	switch (resolveState)
	{
	case VM_E_SOC_SUCCESS:  // Get IP address successfully, result is filled.
		addr.S_un.s_addr = dns.address[0];
		CONNECT_IP_ADDRESS = inet_ntoa(addr);
		Serial.print("ip address is ");
		Serial.println(CONNECT_IP_ADDRESS);

		return true;
	case VM_E_SOC_WOULDBLOCK:  // wait response from network, result could be gotten from callback.
		// need to wait, return directly
		// so MMI message loop may continue.
		return false;
	case VM_E_SOC_INVAL:  // invalid arguments: null domain_name, etc.
	case VM_E_SOC_ERROR:  // unspecified error
	case VM_E_SOC_LIMIT_RESOURCE:  // socket resources not available
	case VM_E_SOC_INVALID_ACCOUNT:  // invalid data account id
		return true;
	}
}


int mtk_net_tcp(int* mtk_server_fd, const char *mtk_host, const char *mtk_port, int mtk_proto)
{

	SOCKADDR_IN       t_addr_in = { 0 };
	int               t_ai_socktype;   /* Socket type. */
	int               t_ai_protocol;   /* Protocol of socket. */
	uint32_t          t_ai_addrlen;    /* Length of socket address. */

	int ret_connect = -1;
	int ret = -1;


	Serial.println("ok");
	char portBuffer[6];
	//sprintf(portBuffer, "%d", params.DestinationPort); DEBUG("  . Connecting to %s/%s...", params.pDestinationURL, portBuffer);*/

	ret = ngethostbyname((unsigned char*)"www.google.com", 1);

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

	return ret;
}


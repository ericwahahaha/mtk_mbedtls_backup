/*
 * Copyright 2010-2015 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

/**
 * @file network_interface.h
 * @brief Network interface definition for MQTT client.
 *
 * Defines an interface to the TLS layer to be used by the MQTT client.
 * Starting point for porting the SDK to the networking layer of a new platform.
 */

#ifndef __TLSCONNECTION_H_
#define __TLSCONNECTION_H_


#include "mbedtls\config.h"
#include "mbedtls\net.h"
#include "mbedtls\ssl.h"
#include "mbedtls\entropy.h"
#include "mbedtls\ctr_drbg.h"
#include "mbedtls\certs.h"
#include "mbedtls\x509.h"
#include "mbedtls\error.h"
#include "mbedtls\debug.h"
#include "mbedtls\timing.h"

#include <Stream.h>
/**
 * @brief TLS Connection Parameters
 *
 * Defines a type containing TLS specific parameters to be passed down to the
 * TLS networking layer to create a TLS secured socket.
 */
typedef struct{
	char* pRootCALocation;				///< Pointer to string containing the filename (including path) of the root CA file.
	char* pDeviceCertLocation;			///< Pointer to string containing the filename (including path) of the device certificate.
	char* pDevicePrivateKeyLocation;	///< Pointer to string containing the filename (including path) of the device private key file.
	char* pDestinationURL;				///< Pointer to string containing the endpoint of the MQTT service.
	int DestinationPort;				///< Integer defining the connection port of the MQTT service.
	unsigned int timeout_ms;			///< Unsigned integer defining the TLS handshake timeout value in milliseconds.
	unsigned char ServerVerificationFlag;	///< Boolean.  True = perform server certificate hostname validation.  False = skip validation \b NOT recommended.
}TLSConnectionInfoParams;

class TLSConnection
{
public:
	TLSConnectionInfoParams net_key_info;
	TLSConnection();
	
/**
 * @brief Initialize the TLS implementation
 *
 * Perform any initialization required by the TLS layer.
 * Connects the interface to implementation by setting up
 * the network layer function pointers to platform implementations.
 *
 * @param pNetwork - Pointer to a Network struct defining the network interface.
 * @return integer defining successful initialization or TLS error
 */
	int tls_init(void);

/**
 * @brief Create a TLS socket and open the connection
 *
 * Creates an open socket connection including TLS handshake.
 *
 * @param pNetwork - Pointer to a Network struct defining the network interface.
 * @param TLSParams - TLSConnectParams defines the properties of the TLS connection.
 * @return integer - successful connection or TLS error
 */
	int tls_connect(TLSConnectionInfoParams params);

/**
 * @brief Write bytes to the network socket
 *
 * @param Network - Pointer to a Network struct defining the network interface.
 * @param unsigned char pointer - buffer to write to socket
 * @param integer - number of bytes to write
 * @param integer - write timeout value in milliseconds
 * @return integer - number of bytes written or TLS error
 */
	int tls_write(unsigned char *pMsg, int len, int timeout_ms);

/**
 * @brief Read bytes from the network socket
 *
 * @param Network - Pointer to a Network struct defining the network interface.
 * @param unsigned char pointer - pointer to buffer where read bytes should be copied
 * @param integer - number of bytes to read
 * @param integer - read timeout value in milliseconds
 * @return integer - number of bytes read or TLS error
 */
	int tls_read(unsigned char *pMsg, int len, int timeout_ms);

/**
 * @brief Disconnect from network socket
 *
 * @param Network - Pointer to a Network struct defining the network interface.
 */
	void tls_disconnect();

/**
 * @brief Perform any tear-down or cleanup of TLS layer
 *
 * Called to cleanup any resources required for the TLS layer.
 *
 * @param Network - Pointer to a Network struct defining the network interface.
 * @return integer - successful cleanup or TLS error
 */
	int tls_destroy();

	int available();


private:


	int ret = 0, i;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ssl_context ssl;
	mbedtls_ssl_config conf;
	uint32_t flags;
	mbedtls_x509_crt cacert;
	mbedtls_x509_crt clicert;
	mbedtls_pk_context pkey;
	mbedtls_net_context server_fd;




//	int myCertVerify(void *data, mbedtls_x509_crt *crt, int depth, uint32_t *flags);

};

#endif 

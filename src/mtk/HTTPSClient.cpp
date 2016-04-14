// Copyright (c) Arduino. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "HTTPSClient.h"

// #define DEBUG_STREAM Serial

HTTPSClient::HTTPSClient() :
    _contentLength(0)
{
}

int HTTPSClient::begin(const char* host, int port)
{
	tls_init();
	net_key_info.pDestinationURL = (char *)host;
	net_key_info.DestinationPort = port;
	net_key_info.timeout_ms = 5000;
	net_key_info.ServerVerificationFlag = true;

	net_key_info.pRootCALocation = "msazure.pem";;
	net_key_info.pDeviceCertLocation = "8cbd725746-certificate.pem.crt";
	net_key_info.pDevicePrivateKeyLocation = "8cbd725746-private.pem.key";

	return tls_connect(net_key_info);
}

int HTTPSClient::sendRequest(const char* method, const char* path)
{
    _contentLength = 0;
	memset(https_buff, 0, sizeof(unsigned char)*HTTPS_BUFFER_SIZE);
	data_input_index = (char*)https_buff;

#ifdef DEBUG_STREAM
    DEBUG_STREAM.print(method);
    DEBUG_STREAM.print(F(" "));
    DEBUG_STREAM.print(path);
    DEBUG_STREAM.print(F(" "));
    DEBUG_STREAM.println(F("HTTP/1.1"));
#endif
	
	strcpy(data_input_index, method);
	data_input_index = data_input_index + strlen(method);

	strcpy(data_input_index, " ");
	data_input_index = data_input_index + 1;

	strcpy(data_input_index, path);
	data_input_index = data_input_index + strlen(path);

	strcpy(data_input_index, " ");
	data_input_index = data_input_index + 1;

	strcpy(data_input_index, "HTTP/1.1");
	data_input_index = data_input_index + strlen("HTTP/1.1");

	strcpy(data_input_index, "\r\n");
	data_input_index = data_input_index + strlen("\r\n");

    /*print(method);
    print(F(" "));
    print(path);
    print(F(" "));
    println(F("HTTP/1.1"));*/

    return 1;
}

int HTTPSClient::sendHeader(const String& header)
{
#ifdef DEBUG_STREAM
    DEBUG_STREAM.println(header);
#endif

//    println(header);
	strcpy(data_input_index, header.c_str());
	data_input_index = data_input_index + strlen(header.c_str());

	strcpy(data_input_index, "\r\n");
	data_input_index = data_input_index + strlen("\r\n");

    return 1;
}

int HTTPSClient::sendBody(const unsigned char *content, int length)
{
#ifdef DEBUG_STREAM
    DEBUG_STREAM.println();
#endif
	strcpy(data_input_index, (const char*)content);
	data_input_index = data_input_index + length;

	/*strcpy(data_input_index, "\r\n");
	data_input_index = data_input_index + strlen("\r\n");*/

	//Serial.println((const char*)https_buff);
	//Serial.println(strlen((const char*)https_buff));

	


//    println();

/*    if (length) {
#ifdef DEBUG_STREAM
        DEBUG_STREAM.write(content, length);
#endif
        write(content, length);
    }*/

	return tls_write(https_buff, strlen((const char*)https_buff), net_key_info.timeout_ms);
}

int HTTPSClient::readStatus()
{
    int statusCode = -1;
	String status;

	memset(https_buff, 0, sizeof(unsigned char)*HTTPS_BUFFER_SIZE);
	Serial.println("Enter recieve");
	statusCode = tls_read(https_buff, HTTPS_BUFFER_SIZE, net_key_info.timeout_ms);
	Serial.println((const char*)https_buff);
	data_input_index = (char*)https_buff;


    status = readLine();
    status.trim();

#ifdef DEBUG_STREAM
    DEBUG_STREAM.println(status);
#endif

    int firstSpaceIndex = status.indexOf(' ');
    int lastSpaceIndex = status.lastIndexOf(' ');

    if (firstSpaceIndex == -1 || lastSpaceIndex == -1 || firstSpaceIndex == lastSpaceIndex) {
        return -1;
    }

    statusCode = status.substring(firstSpaceIndex + 1, lastSpaceIndex).toInt();

    if (statusCode == 0) {
        // bad parse
        statusCode = -1;
    }

    return statusCode;
}

int HTTPSClient::readHeader(String& name, String& value)
{
    String headerLine = readLine();
    headerLine.trim();

#ifdef DEBUG_STREAM
    DEBUG_STREAM.println(headerLine);
#endif

    if (headerLine.length() == 0) {
        return 0;
    }

    int colonIndex = headerLine.indexOf(':');

    if (colonIndex == -1) {
        return -1;
    }

    name = headerLine.substring(0, colonIndex);
    value = headerLine.substring(colonIndex + 1);

    name.trim();
    value.trim();

    if (name.equals("Content-Length")) {
        _contentLength = value.toInt();
    }

    return 1;
}

void HTTPSClient::end()
{
	tls_disconnect();
	tls_destroy();
}

String HTTPSClient::readLine()
{
	String readline_temp;
	int temp_counter = 0;

	while (data_input_index[temp_counter] != '\n'){
		readline_temp += (char)data_input_index[temp_counter];
		temp_counter++;
	}
	data_input_index = &data_input_index[temp_counter+1];
	/*char *line_head;
	char temp_buffer[500];
	char test[] = "123";
	int number_of_char;
	
	line_head = strtok(data_input_index, "\n");
	number_of_char = sprintf(temp_buffer, "%s", line_head);
	//readline_temp((const char*)line_head);
	data_input_index = strtok(NULL, "\n");*/

	return readline_temp;
}

size_t HTTPSClient::contentLength()
{
    return _contentLength;
}

int HTTPSClient::readBody(unsigned char *content, int length)
{
    strncpy((char *)content, data_input_index, length);
#ifdef DEBUG_STREAM
    DEBUG_STREAM.write(content, length);
#endif
    return 1;
}

void HTTPSClient::setTimeout(int timeout_val)
{
	net_key_info.timeout_ms = timeout_val;
	return;
}

int HTTPSClient::connected()
{
	if (available()){
		return 1;
	}
	return 0;
}
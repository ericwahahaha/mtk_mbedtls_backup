// Copyright (c) Arduino. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "tlsconnection.h"

#ifndef HTTPSCLIENT_H
#define HTTPSCLIENT_H

#define HTTPS_PORT          443
#define HTTPS_BUFFER_SIZE 3200

class HTTPSClient : public TLSConnection
{
    public:
        HTTPSClient();
        int begin(const char* host, int port = HTTPS_PORT);
        int sendRequest(const char* method, const char* path);
        int sendHeader(const String& header);
        int sendBody(const unsigned char *content, int length);
        int readStatus();
        int readHeader(String& name, String& value);
        size_t contentLength();
        int readBody(unsigned char *content, int length);
		void setTimeout(int timeout_val);
		int connected();
        void end();

    private:
        String readLine();
		unsigned char https_buff[ HTTPS_BUFFER_SIZE ];
		char *data_input_index;


		size_t _sendingLength;
        size_t _contentLength;
};

#endif

// Copyright (c) Arduino. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <Arduino.h>
#include <vmsock.h>


#include <signal.h>


#ifndef MTKNET_H
#define MTKNET_H

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC int mtk_net_tcp(int* mtk_server_fd, const char *mtk_host, const char *mtk_port, int mtk_proto);

#undef EXTERNC



#endif

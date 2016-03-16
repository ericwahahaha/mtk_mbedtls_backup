// Copyright (c) Arduino. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <Arduino.h>
#include <vmsock.h>


#ifndef MTKNET_H
#define MTKNET_H

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC int mtk_net_tcp(int* mtk_server_fd);

#undef EXTERNC



#endif

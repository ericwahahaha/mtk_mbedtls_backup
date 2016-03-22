// Copyright (c) Arduino. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "Arduino.h"

#include "azure_iot_hub/threadapi.h"

void ThreadAPI_Sleep(unsigned int milliseconds)
{
    delay(milliseconds);
}

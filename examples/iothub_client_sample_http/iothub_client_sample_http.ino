// Copyright (c) Arduino. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <LWiFi.h>
#include <LTask.h>

#include "iothub_client_sample_http.h"

char ssid[] = "yourNetwork"; //  your network SSID (name)
char pass[] = "secretPassword";    // your network password (use for WPA, or use as key for WEP)

/*VMINT g_bearer_hdl;

// invoked in main thread context
void bearer_callback(VMINT handle, VMINT event, VMUINT data_account_id, void *user_data)
{
	if (VM_BEARER_WOULDBLOCK == g_bearer_hdl)
	{
		g_bearer_hdl = handle;
	}

	switch (event)
	{
	case VM_BEARER_DEACTIVATED:
		break;
	case VM_BEARER_ACTIVATING:
		break;
	case VM_BEARER_ACTIVATED:
		LTask.post_signal();
		break;
	case VM_BEARER_DEACTIVATING:
		break;
	default:
		break;
	}
}

boolean bearer_open(void* ctx){
	g_bearer_hdl = vm_bearer_open(VM_BEARER_DATA_ACCOUNT_TYPE_WLAN, NULL, bearer_callback);

	if (g_bearer_hdl >= 0)
		return true;
	return false;
}*/

void setup() {
  Serial.begin(9600);

  /*// check for the presence of the shield :
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    if (status != WL_CONNECTED) {
      // wait 10 seconds for connection:
      delay(10000);
    }
  }
  Serial.println("Connected to wifi");*/

  int ret_result;
  LTask.begin();
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  Serial.println("setup()");

  // attempt to connect to Wifi network:
  LWiFi.begin();
  Serial.println("Connecting to wifi");
  while (!LWiFi.connectWPA(ssid, pass))
  {
	  delay(1000);
	  Serial.println("retry WiFi AP");
  }
  Serial.println("Connected to wifi");

  delay(1000);

//  LTask.remoteCall(&bearer_open, NULL);

//  LTask.remoteCall(&tls_start, NULL);


  Serial.println("setup() done");
}

void loop() {
  iothub_client_sample_http_run();
}


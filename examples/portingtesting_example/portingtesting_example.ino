/*

 Udp NTP Client

 Get the time from a Network Time Protocol (NTP) time server
 Demonstrates use of UDP sendPacket and ReceivePacket
 For more on NTP time servers and the messages needed to communicate with them,
 see http://en.wikipedia.org/wiki/Network_Time_Protocol

 created 4 Sep 2010
 by Michael Margolis
 modified 9 Apr 2012
 by Tom Igoe
 Modified 20 Aug 2014
 by MediaTek Inc.

 This code is in the public domain.

 */
#include <LWiFi.h>
#include <LWiFiClient.h>
#include <LWiFiUdp.h>
#include <LTask.h>

#include "linkit_aws_header.h"

char ssid[] = "I_Got_WIFI";  //  your network SSID (name)
char pass[] = "61399316";       // your network password

unsigned int localPort = 2390;      // local port to listen for UDP packets

IPAddress timeServer(129, 6, 15, 28); // time.nist.gov NTP server

//const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
NTPClient Udp;
uint32_t ntp_time_utc;

VMINT g_bearer_hdl;

//testnthuiot.azure-devices.net
char HostAddress[255] = "testnthuiot.azure-devices.net";
VMINT port = 443;
char cafileName[] = "msazure.pem";
char clientCRTName[] = "8cbd725746-certificate.pem.crt";
char clientKeyName[] = "8cbd725746-private.pem.key";

Network mtk_tls_net_test;
TLSConnectParams mtk_tls_test;

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
}

boolean  tls_start(void* ctx)
{
	int ret_result = 0;
	mtk_tls_test.pRootCALocation = cafileName;
	mtk_tls_test.pDeviceCertLocation = clientCRTName;
	mtk_tls_test.pDevicePrivateKeyLocation = clientKeyName;
	mtk_tls_test.pDestinationURL = HostAddress;
	mtk_tls_test.DestinationPort = port;
	mtk_tls_test.timeout_ms = 5000;
	mtk_tls_test.ServerVerificationFlag = true;

	Serial.println("\nStarting TLS connection ...");
	ret_result = mtk_tls_init(&mtk_tls_net_test);
	Serial.print("\n mtk_tls_init return value is ...");
	Serial.println(ret_result);

	ret_result = mtk_tls_connect(&mtk_tls_net_test, mtk_tls_test);
	Serial.print("\n mtk_tls_connect return value is ...");
	Serial.println(ret_result);


	return true;
}

void setup()
{
	int ret_result;
	LTask.begin();
	IPAddress DNS_IPResult;

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
  printWifiStatus();

  delay(1000);

/*  ret_result = LWiFi.hostByName((const char*)HostAddress, DNS_IPResult);
  Serial.print("\nDNS server resolving result is...");
  Serial.println(DNS_IPResult);
  Serial.println(ret_result);*/

  Serial.println("\nStarting connection to server...");
  ret_result = Udp.begin();

  Serial.println("setup() done");

  LTask.remoteCall(&bearer_open, NULL);

  LTask.remoteCall(&tls_start, NULL);


  Serial.println("setup() done");
}

void loop()
{

	ntp_time_utc = Udp.getEpochTime("time.stdtime.gov.tw", 123, DEFAULT_NTP_TIMEOUT);
	Serial.println(ntp_time_utc);
	// print the hour, minute and second:
	Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
	Serial.print((ntp_time_utc % 86400L) / 3600); // print the hour (86400 equals secs per day)
	Serial.print(':');
	if (((ntp_time_utc % 3600) / 60) < 10)
	{
		// In the first 10 minutes of each hour, we'll want a leading '0'
		Serial.print('0');
	}
	Serial.print((ntp_time_utc % 3600) / 60); // print the minute (3600 equals secs per minute)
	Serial.print(':');
	if ((ntp_time_utc % 60) < 10)
	{
		// In the first 10 seconds of each minute, we'll want a leading '0'
		Serial.print('0');
	}
	Serial.println(ntp_time_utc % 60); // print the second
	delay(10000);
  /*sendNTPpacket(timeServer); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);
  Serial.println( Udp.parsePacket() );
  if ( Udp.parsePacket() )
  {
    Serial.println("packet received");
    // We've received a packet, read the data from it
    memset(packetBuffer, 0xcd, NTP_PACKET_SIZE);
    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    for (int i = 0; i < NTP_PACKET_SIZE; ++i)
    {
      Serial.print(packetBuffer[i], HEX);
    }
    Serial.println();


    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = " );
    Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    Serial.println(epoch);


    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Serial.print(':');
    if ( ((epoch % 3600) / 60) < 10 )
    {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    Serial.print(':');
    if ( (epoch % 60) < 10 )
    {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.println(epoch % 60); // print the second
  }
  // wait ten seconds before asking for the time again
  delay(10000);*/
}

// send an NTP request to the time server at the given address
/*unsigned long sendNTPpacket(IPAddress& address)
{
  Serial.println("sendNTPpacket");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  //Serial.println("2");
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  //Serial.println("3");

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  //Serial.println("4");
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  //Serial.println("5");
  Udp.endPacket();
  //Serial.println("6");
}*/


void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(LWiFi.SSID());

  // print your LWiFi shield's IP address:
  IPAddress ip = LWiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = LWiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

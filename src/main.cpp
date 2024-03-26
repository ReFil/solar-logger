#include <Arduino.h>
#include <SoftwareSerial.h>

#define WEST
//#define EAST

#define ONEMPP

#include "VEDirect.h"
#include "ThingSpeak.h"
#include "secrets.h"

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

//Static data storage vars for MPPT 1
int32_t VE_powerpv, VE_voltage, VE_voltagepv, VE_current;
float powerpv, voltagepv, voltage, current;

//Static data storage vars for MPPT 2
int32_t VE_powerpv2, VE_voltage2, VE_voltagepv2, VE_current2;
float powerpv2, voltagepv2, voltage2, current2;

VEDirect myve(RX1, TX1);
#ifndef ONEMPP
VEDirect myve2(RX2, TX2);
#endif

#include <ESP8266WiFi.h>

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
WiFiClient  client;

void setup() {
  Serial.begin(115200);
  delay(100);

    Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  if(myve.begin()) {									// test connection to MPPT 1
		VE_powerpv = myve.read(VE_POWERPV);
		VE_voltagepv = myve.read(VE_VOLTAGEPV);
		VE_current = myve.read(VE_CURRENT);
    VE_voltage = myve.read(VE_VOLTAGE);
	} else {
		Serial.println("Could not open serial port to VE device 1");
		while (1);
	}
#ifndef ONEMPP

  if(myve2.begin()) {									// test connection to MPPT 2
		VE_powerpv2 = myve2.read(VE_POWERPV);
		VE_voltagepv2 = myve2.read(VE_VOLTAGEPV);
		VE_current2 = myve2.read(VE_CURRENT);
    VE_voltage2 = myve2.read(VE_VOLTAGE);
	} else {
		Serial.println("Could not open serial port to VE device 2");
		while (1);
	}
#endif

  WiFi.mode(WIFI_STA);

  ThingSpeak.begin(client);

}

void loop() {

  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
  VE_powerpv = myve.read(VE_POWERPV);
  VE_voltagepv = myve.read(VE_VOLTAGEPV);
  VE_current = myve.read(VE_CURRENT);
  VE_voltage = myve.read(VE_VOLTAGE);

#ifndef ONEMPP

  VE_powerpv2 = myve2.read(VE_POWERPV);
  VE_voltagepv2 = myve2.read(VE_VOLTAGEPV);
  VE_current2 = myve2.read(VE_CURRENT);
  VE_voltage2 = myve2.read(VE_VOLTAGE);
  #endif

  powerpv = (float)VE_powerpv/1000; 
  voltagepv = (float)VE_voltagepv/1000; 
  current = (float)VE_current/1000; 
  voltage = (float)VE_voltage/1000; 
#ifndef ONEMPP

  powerpv2 = (float)VE_powerpv2/1000; 
  voltagepv2 = (float)VE_voltagepv2/1000; 
  current2 = (float)VE_current2/1000; 
  voltage2 = (float)VE_voltage2/1000;
#endif
  /*Serial.print("Current: ");
  Serial.print(current);
  Serial.print(" Voltage: ");
  Serial.print(voltage);
  Serial.print(" PV Power: ");
  Serial.println(powerpv);
*/
 //battery management code goes here
 /* if(battvolt < 12.00)
  //some power off code
  else if    
*/

  ThingSpeak.setField(1, powerpv);
  ThingSpeak.setField(2, voltagepv);
  ThingSpeak.setField(3, current);
  ThingSpeak.setField(4, voltage);
#ifndef ONEMPP

  ThingSpeak.setField(5, powerpv2);
  ThingSpeak.setField(6, voltagepv2);
  ThingSpeak.setField(7, current2);
  ThingSpeak.setField(8, voltage2);

#endif

  int httpCode1 = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if (httpCode1 == 200) {
    Serial.println("Channel write successful.");
  }
  else {
    Serial.println("Problem writing to channel. HTTP error code " + String(httpCode1));
  }

  // Wait 20 seconds to update the channel again
  delay(20000);
}

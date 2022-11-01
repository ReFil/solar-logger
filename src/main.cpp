#include <Arduino.h>
#include <SoftwareSerial.h>

#include "VEDirect.h"
#include "ThingSpeak.h"
#include "secrets.h"

#define TX1 D1
#define RX1 D2
#define TX2 D3
#define RX2 D4

#define SCALE 100

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

int32_t VE_powerpv, VE_voltage, VE_voltagepv, VE_current;


VEDirect myve(RX1, TX1);

#include <ESP8266WiFi.h>

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key index number (needed only for WEP)
WiFiClient  client;

void setup() {
  Serial.begin(115200);
  delay(100);

  if(myve.begin()) {									// test connection
		VE_powerpv = myve.read(VE_POWERPV);
		VE_voltagepv = myve.read(VE_VOLTAGEPV);
		VE_current = myve.read(VE_CURRENT);
    VE_voltage = myve.read(VE_VOLTAGE);
	} else {
		Serial.println("Could not open serial port to VE device");
		while (1);
	}

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

  float powerpv = (float)VE_powerpv/1000; 
  float voltagepv = (float)VE_voltagepv/1000; 
  float current = (float)VE_current/1000; 
  float voltage = (float)VE_voltage/1000; 

 //battery management code goes here
 /* if(battvolt < 12.00)
  //some power off code
  else if    

*/
  ThingSpeak.setField(1, powerpv);
  ThingSpeak.setField(2, voltagepv);
  ThingSpeak.setField(3, current);
  ThingSpeak.setField(4, voltage);


  int httpCode1 = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if (httpCode1 == 200) {
    Serial.println("Channel write successful.");
  }
  else {
    Serial.println("Problem writing to channel. HTTP error code " + String(httpCode1));
  }

  // Wait 20 seconds to update the channel again
  delay(50000);
}

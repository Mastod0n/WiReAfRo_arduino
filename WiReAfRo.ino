/* References:
 * [ESP8266]      Part of the ESP8266h library.                     https://github.com/esp8266/Arduino
 * [WifiServer]   Part of the official Arduino Wifi Server class.   https://www.arduino.cc/en/Reference/WiFiServer
 * 
 * Released umder GNU Lesser General Public License v3.0
*/ 


#include <ESP8266WiFi.h>


#define ssid "DevSoftAP"                              // [ESP8266] Define your own ESP's SoftAP SSID (min. 8 characters)
#define pass "AMastod0nProduction"                    // [ESP8266] Define your own Wifi passphrase (min. 8 characters)

IPAddress ip_esp(192, 168, 3, 1);                     // [ESP8266]
IPAddress gateway(192, 168, 3, 1);                    // [ESP8266]
IPAddress subnet(255, 255, 255, 0);                   // [ESP8266]

#define port 40
#define tcpnodelay true

#define pin1a 15                                      // D8, 15
#define pin1b 4                                       // D7, 13 bzw. D2, 4
#define pin2a 12                                      // D6, 12
#define pin2b 14                                      // D5, 14
#define pinled D4                                     // D4, 2

#define pwmrange 1023                                 // [ESP8266] default: pre version 3.0: 0-1023, post version 3.0: 0-255

unsigned int pwmR = 1023;
unsigned int pwmL = 1023;

WiFiServer server(port);                              // [WifiServer]


//--------------------------------------------------------------------------------------------------------------------------


void setup() {
  pinMode(pin1a, OUTPUT);
  pinMode(pin1b, OUTPUT);
  pinMode(pin2a, OUTPUT);
  pinMode(pin2b, OUTPUT);
  pinMode(pinled, OUTPUT);
  analogWriteRange(pwmrange);

  WiFi.softAPConfig(ip_esp, gateway, subnet);         // [ESP8266]
  WiFi.softAP(ssid, pass);                            // [ESP8266]

  server.begin();                                     // [WifiServer]
  server.setNoDelay(tcpnodelay);                      // [WifiServer]
}


//--------------------------------------------------------------------------------------------------------------------------


void loop() {
  while (WiFi.softAPgetStationNum() == 0) {           // [ESP8266]
    delay(500);                                       // delay() yields; Wifi functions keep working
    digitalWrite(pinled, HIGH);
    delay(500);
    digitalWrite(pinled, LOW);
  }

  WiFiClient client = server.available();             // [WifiServer]

  if(client.connected()) {                            // [WifiServer]
    String data = client.readStringUntil('\n');       // [WifiServer]

    pwmL = atoi(strtok(&data[0u],"."));
    pwmR = atoi(strtok(NULL,"."));
  }

  if(pwmL >= 1023) {
    analogWrite(pin1b, 0);
    analogWrite(pin1a, pwmL-1023);
  }
  else if(pwmL < 1023) {
    analogWrite(pin1a, 0);
    analogWrite(pin1b, (pwmL-1023)*-1);
  }

  if(pwmR >= 1023) {
    analogWrite(pin2b, 0);
    analogWrite(pin2a, pwmR-1023);
  }
  else if(pwmR < 1023) {
    analogWrite(pin2a, 0);
    analogWrite(pin2b, (pwmR-1023)*-1);
  }
}

#include <ESP8266WiFi.h>
//gibt einmalig die Macadresse des ESP8266 Boards aus
void setup() {
   Serial.begin(115200);
  Serial.println("Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}

void loop() {
}

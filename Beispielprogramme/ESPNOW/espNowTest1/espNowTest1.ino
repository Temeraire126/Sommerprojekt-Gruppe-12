#include <ESP8266WiFi.h>
#include <espnow.h>
uint8_t broadcastAddress[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}; //Macadresse des Empfängers
//Datenpaketstruktur festlegen:
typedef struct struct_message{
  bool a;
} struct_message;
//Datenpakete erstellen:
struct_message recvData;
struct_message sendData;
//Diese Variablen werden für das wiederholte Senden verwendet
 unsigned long lastTime = 0;  
unsigned long timerDelay = 2500;
//Empfangsmethode definieren:
 void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len){
  memcpy(&recvData, incomingData,sizeof(recvData));       //speichert die empfangenen Werte in "recvData"
  Serial.print("received");
 }
 //Sendemethode:
 void onDataSent(uint8_t *mac_addr, uint8_t sendstatus){
  Serial.print("Last Packet Send Status: ");
  if(sendstatus == 0){
    Serial.println("Delivery success");
  }
  else {
    Serial.println("Delivery fail");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);        //wifi intialisieren
  //espnow initialisieren:
  if (esp_now_init() != 0) {    
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);    //Kommunikationsrolle festlegen
  //Empfangs- und Sendefunktion an das Protokoll übergeben:
  esp_now_register_recv_cb(OnDataRecv);        
  esp_now_register_send_cb(onDataSent);
  //Kommunikationspartner angeben:
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  //Als Beispiel wird hier die LED angesteuert
  pinMode(LED_BUILTIN,OUTPUT);
  //Datenpaket festlegen
  sendData.a=true;
}

void loop() {
  //sendet alle "timerDelay" Millisekunden ein alternierendes true/false welches dann die LED ein/aus schaltet
      if ((millis() - lastTime) > timerDelay) {
  esp_now_send(broadcastAddress, (uint8_t *) &sendData,sizeof(sendData));
  sendData.a=!sendData.a;
  lastTime = millis();
  }
  //LED steuerung:
  if(recvData.a){
  digitalWrite(LED_BUILTIN,LOW);
  }else{
    digitalWrite(LED_BUILTIN,HIGH);
  }
    
}

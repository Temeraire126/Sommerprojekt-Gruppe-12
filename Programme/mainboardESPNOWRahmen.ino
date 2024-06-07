#include <espnow.h>
#include <ESP8266WiFi.h>

uint8_t addressMini1[] = {0xC8,0xC9,0xA3,0x14,0x32,0x59};
uint8_t addressMini2[] = {0x60,0x01,0x94,0x10,0x09,0x3C};
typedef struct packageCodeStruct{
  uint a;
} packageCodeStruct;
typedef struct packageResponseStruct{
  bool b;
} packageResponseStruct;

packageCodeStruct sendData1;
packageCodeStruct sendData2;
packageResponseStruct recvData1;
packageResponseStruct recvData2;
unsigned long lastTime = 0;  
unsigned long timerDelay = 20000;

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len){
  memcpy(&recvData, incomingData,sizeof(recvData1));
  Serial.print("received");
 }
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
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if(esp_now_inti() != 0){
    Serial.println("Error initilizing ESPNOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(onDataSent);
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_add_peer(addressMini1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(addressMini2, ESP_NOW_ROLE_SLAVE, 2, NULL, 0);
}
void sendCodes(uint code1, uint code2){
  sendData1.a=code1;
  sendData2.a=code2;
  esp_now_send(addressMini1, (uin8_t *)&sendData1,sizeof(sendData1));
  esp_now_send(addressMini2, (uin8_t *)&sendData2,sizeof(sendData2));
}
void loop() {
  // put your main code here, to run repeatedly:

}

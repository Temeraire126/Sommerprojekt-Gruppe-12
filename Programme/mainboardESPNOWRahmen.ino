#include <espnow.h>
#include <ESP8266WiFi.h>

const int codeLength =4;
//adressen festlegen
 uint8_t addressMini1[] = {0xC8,0xC9,0xA3,0x14,0x32,0x59};
 uint8_t addressMini2[] = {0x60,0x01,0x94,0x10,0x09,0x3C};
 uint8_t recvAddress1[] = {0x4C,0x75,0x25,0x36,0xBD,0x64};
 uint8_t recvAddress2[] = {0x08,0x3A,0x8D,0xCF,0xAF,0x55};
//pakate definieren und initialisieren
 typedef struct packageCodeStruct{
  char* a;
 } packageCodeStruct;
 typedef struct packageResponseStruct{
  bool b;
 } packageResponseStruct;

 packageCodeStruct sendData;
 packageResponseStruct recvData1;
 packageResponseStruct recvData2;

bool statusTresor1=true;
bool statusTresor2=true;

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len){
  if(0==memcmp(recvAddress1, mac , sizeof(mac))){
  memcpy(&recvData1, incomingData,sizeof(recvData1));
  }
  if(0==memcmp(recvAddress2, mac, sizeof(mac))){
  memcpy(&recvData2, incomingData,sizeof(recvData2));
  }
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
  if(esp_now_init() != 0){
    Serial.println("Error initilizing ESPNOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(onDataSent);
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_add_peer(addressMini1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(addressMini2, ESP_NOW_ROLE_SLAVE, 2, NULL, 0);
}
char* generateCode(){
  char code[codeLength];
  int zahl;
  for (int i = 0; i< codeLength; i++){
    zahl = (rand()%10);
    code[i] = '0' + zahl;
  }
  char * rtr = code;
  return rtr;
}

void updateLED(){

}
void showCode(char values[]){

}

void loop() {
  // put your main code here, to run repeatedly:
  if(true){
    char * code=generateCode();
    sendData.a=code;
    free(code);
    esp_now_send(addressMini1, (uint8_t *)&sendData,sizeof(sendData));
    sendData.a=nullptr;
  }
  if(true){
    char * code = generateCode();
    sendData.a=code;
    free(code);
    esp_now_send(addressMini2, (uint8_t *)&sendData,sizeof(sendData));
    sendData.a=nullptr;
  }
}

#include <espnow.h>
#include <ESP8266WiFi.h>
#include <String>

#include <LiquidCrystal_I2C.h>       // Vorher hinzugefügte LiquidCrystal_I2C Bibliothek einbinden
LiquidCrystal_I2C lcd(0x27, 16, 2);  //Hier wird festgelegt um was für einen Display es sich handelt. In diesem Fall eines mit 16 Zeichen in 2 Zeilen und der HEX-Adresse 0x27. Für ein vierzeiliges I2C-LCD verwendet man den Code "LiquidCrystal_I2C lcd(0x27, 20, 4)"

//Pins
#define REDLED1 D5
#define REDLED2 D8
#define GREENLED1 D4
#define GREENLED2 D7
#define BUTTON1 D6
#define BUTTON2 D3

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

bool codeIsDisplayed = false;

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
 //Pin Modes:
      pinMode(REDLED1, OUTPUT);
      pinMode(REDLED2, OUTPUT);
      pinMode(GREENLED1, OUTPUT);
      pinMode(GREENLED2, OUTPUT);
      pinMode(BUTTON1, INPUT_PULLUP);
      pinMode(BUTTON2, INPUT_PULLUP);
 
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
std:string generateCode(){
  char code[codeLength];
  int zahl;
  for (int i = 0; i< codeLength; i++){
    zahl = (rand()%10);
    code[i] = '0' + zahl;
  }
  std::string rtr = code;
  rtr.erase(codeLength, std::string::npos);
  return rtr;
}

void updateLED(){
 if (statusTresor1) {
        digitalWrite(GREENLED1, HIGH);
        digitalWrite(REDLED1, LOW);
      } else {
        digitalWrite(REDLED1, HIGH);
        digitalWrite(GREENLED1, LOW);
      }
      if (statusTresor2) {
        digitalWrite(GREENLED2, HIGH);
        digitalWrite(REDLED2, LOW);
      } else {
        digitalWrite(REDLED2, HIGH);
        digitalWrite(GREENLED2, LOW);
      }
}
void showCode(char values[]){
for (int i = 0; i < sizeof(values); i++) {
        lcd.setCursor(0, i);
        lcd.print(values[i]);
      }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(BUTTON1) == HIGH){
    std::string code=generateCode();
    sendData.a=code;
    esp_now_send(addressMini1, (uint8_t *)&sendData,sizeof(sendData));
    sendData.a=nullptr;
  }
  if(digitalRead(BUTTON2)== HIGH){
    std::string code = generateCode();
    sendData.a=code;
    esp_now_send(addressMini2, (uint8_t *)&sendData,sizeof(sendData));
    sendData.a=nullptr;
  }
}

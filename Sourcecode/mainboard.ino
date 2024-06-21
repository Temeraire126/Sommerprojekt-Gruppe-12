#include <espnow.h>
#include <ESP8266WiFi.h>
#include <String>
//Pins
#define REDLED1 D5
#define REDLED2 D8
#define GREENLED1 D4
#define GREENLED2 D7
#define BUTTON1 D6
#define BUTTON2 D3

//Display
#include <LiquidCrystal_I2C.h>       
LiquidCrystal_I2C lcd(0x27, 16, 2);  //Hier wird festgelegt um was für einen Display es sich handelt
//PINs des Displays:
//- SCL: D1   grau
//- SDA: D2   weiß

const int codeLength =4;
//für weitere Tresore die Adressse im gleichen Vormat speichern:
  //Adressen der D1Mini Keypads (passcode wird hierhin gesendet)
  uint8_t addressMini1[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};//0xC8,0xC9,0xA3,0x14,0x32,0x59
  uint8_t addressMini2[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};//0x60,0x01,0x94,0x10,0x09,0x3C
 //uint8_t addressMini3[] = ....

  //Adressen der NodeMCUs von denen der Status der Tresore erhalten wird (wird zur Unterscheidung der Tresore verwendet)
  uint8_t recvAddress1[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};//0x4C,0x75,0x25,0x36,0xBD,0x64
  uint8_t recvAddress2[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};//0x08,0x3A,0x8D,0xCF,0xAF,0x55
  //uint8_t recvAddress3[]=...

//pakate definieren und initialisieren
//sendet:
 typedef struct packageCodeStruct{
  String a;
 } packageCodeStruct;
//erhält:
 typedef struct packageResponseStruct{
  bool b;
 } packageResponseStruct;

 packageCodeStruct sendData;
//eine variable für jeden Tresor, der einen Wert zurückschickt. So wird sich der Status aller Schlüssel gemerkt.
 packageResponseStruct recvData1;
 packageResponseStruct recvData2;
//packageResponseStrct recvData3;
//...

//Sind Schlüssel in den Tresoren?
bool statusTresor1=true;
bool statusTresor2=true;
//bool statusTresor3...

bool codeIsDisplayed = false; 

//Aufruf bei Datenerhalt. Abhängig der Senderadresse wird der Inhalt in verschiedene variablen gespeichert
//Bei weiteren Tresoren muss für jeden neuen eine eigene Abfrage gemacht werden
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len){
  if(0==memcmp(recvAddress1, mac , sizeof(mac))){
  memcpy(&recvData1, incomingData,sizeof(recvData1));
  }
  if(0==memcmp(recvAddress2, mac, sizeof(mac))){
  memcpy(&recvData2, incomingData,sizeof(recvData2));
  }
 /*
    if(0==memcmp(recvAddress3, mac, sizeof(mac))){
  memcpy(&recvData3, incomingData,sizeof(recvData3));
  }
  */
  Serial.print("received");
  updateLED();
}
// Debugginghilfe beim Versenden der Passcodes:
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
 //Pin Modes:
      pinMode(REDLED1, OUTPUT);
      pinMode(REDLED2, OUTPUT);
      pinMode(GREENLED1, OUTPUT);
      pinMode(GREENLED2, OUTPUT);
      pinMode(BUTTON1, INPUT_PULLUP);
      pinMode(BUTTON2, INPUT_PULLUP);
   //INIT für ESPNOW
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
//generiert einen zufälligen 4-stelligen Zahlencode
String generateCode(){
  char code[codeLength];
  int zahl;
  for (int i = 0; i< codeLength; i++){
    zahl = (rand()%10);
    code[i] = '0' + zahl;
  }
  String rtr = code;
  return rtr;
}
//stellt den Status der Tresore in den LEDs dar (grün=schlüssel ist vorhanden und kann abgeholt werden. rot= schlüssel wurde entfernt und wird verwendet)
//auch hier wäre eine Erweiterung bei neuen Tresoren notwending. Bei mehr als 2 oder 3 Tresoren wäre es sinnvoller das ganze über einen for-Schleife laufen zu lassen und die outputs vorher in einem Array zu speichern

void updateLED(){
  statusTresor1=recvData1.b;
  statusTresor2=recvData2.b;
 if (statusTresor1) {
        digitalWrite(GREENLED1, HIGH);
        digitalWrite(REDLED1, LOW); 
        Serial.println(" 1 present");
  } else {
        digitalWrite(REDLED1, HIGH);
        digitalWrite(GREENLED1, LOW);
        Serial.println(" 1 gone");
  }
  if (statusTresor2) {
        digitalWrite(GREENLED2, HIGH);
        digitalWrite(REDLED2, LOW);
        Serial.println(" 2 present");
  } else {
        digitalWrite(REDLED2, HIGH);
        digitalWrite(GREENLED2, LOW);
        Serial.println(" 2 gone");
  }
}
//Zeigt den Code auf dem LCD an
void showCode(char values[]){
for (int i = 0; i < sizeof(values); i++) {
        lcd.setCursor(0, i);
        lcd.print(values[i]);
      }
}
//Erstellt bei Knopfdruck einen Code und schickt diesen an den jeweiligen D1Mini
void loop() {
 //Tresor 1
  if(digitalRead(BUTTON1) == LOW){
    String code=generateCode();
    sendData.a=code;
    esp_now_send(addressMini1, (uint8_t *)&sendData,sizeof(sendData));
    delay(1000);
  }
 //Tresor 2
  delay(100);
  if(digitalRead(BUTTON2)== LOW){
    String code = generateCode();
    sendData.a=code; 
    esp_now_send(addressMini2, (uint8_t *)&sendData,sizeof(sendData));
    delay(1000);   
  }
 /*
 delay(100);
 if(digitalRead(BUTTON3)==LOW){
 ...
 }
 */
}

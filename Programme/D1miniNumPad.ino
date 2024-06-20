#include <espnow.h>
#include <ESP8266WiFi.h>
#include <Keypad.h>
#include <String>
//definieren der Paketstruktur die erhalten wird:
typedef struct codeStruct{
  String a;
} codeStruct;
codeStruct recvData;
//Aufruf bei Datenerhalt:
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len){
  memcpy(&recvData, incomingData,sizeof(recvData));
  Serial.println(recvData.a);
 }
//Init des Keypads:
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'D','#','0','*'},
  {'C','9','8','7'},
  {'B','6','5','4'},
  {'A','3','2','1'}
};
byte rowPins[ROWS] = { D4, D3, D2, D1 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { D8, D7, D6, D5 };  //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
//Pin für Signal bei korrekter Codeeingabe:
#define PIN_OUTPUT D0

void setup() {
  pinMode(PIN_OUTPUT, OUTPUT);
  Serial.begin(115200);
  //Initialisierung von ESPnow:
  WiFi.mode(WIFI_STA);
  if(esp_now_init() != 0){
    Serial.println("Error");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
 digitalWrite(PIN_OUTPUT,HIGH);
  char key = keypad.getKey();
  if (key) {
    Serial.println(key);
    //string zu char[] umwandeln
    String tmp = recvData.a;
    char codeArray[4];
    for( int i=0;i<4;i++ ){
    codeArray[i]=tmp[i];
    }
    //bei korrektem Code wird das Signal an D0 für einen Sekunde verändert und kann dann vom NodeMCU ausgelesen werden
    if(checkCode(codeArray, key)){
        digitalWrite(PIN_OUTPUT,LOW);
        delay(1000);
    }
  }
}
//vergleicht eingegebenen code mit überliefertem code
boolean checkCode(char code[], char first) {
  char input[4];
  input[0] = first;
  int index = 1;
  int time = 0;
  while (time < 10000) {
    char key = keypad.getKey();
    //einlesen der Zeichen
    if (key) {
      Serial.println(key);
      time = 0;
      input[index] = key;
      index += 1;
      //sind alle zeichen eingegeben worden?
      if (index == 4) {
        bool cmp = true;
        //einzelne Zeichen vergleichen
        for(int i = 0; i < 4;i++){
          if(input[i]!=code[i]){cmp=false;}
        }
        return cmp;
      }
    }
    delay(10);
    //Zeitvariable um zu überprüfen ob jemand ein Stück Code eigegeben hat und gegangen ist.
    time += 1;
  }
  return false;
}

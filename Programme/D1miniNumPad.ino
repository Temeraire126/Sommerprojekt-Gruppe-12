#include <Keypad.h>
#include <espnow.h>
#include <ESP8266WiFi.h>
#include <String>
typedef struct codeStruct{
  String a;
} codeStruct;
codeStruct recvData;
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len){
  memcpy(&recvData, incomingData,sizeof(recvData));
 }

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

#define PIN_OUTPUT D0

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_OUTPUT, OUTPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if(esp_now_init() != 0 ) {
    Serial.println("Error");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
  pinMode(PIN_OUTPUT,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(PIN_OUTPUT,LOW);
  char key = keypad.getKey();

  if (key) {
    Serial.println(key);
    //convert string to char[]
    
    String tmp = recvData.a;
    char codeArray[4];
    for( int i=0;i<4;i++ ){
    codeArray[i]=tmp[i];
    }
    //*****************************//
    // replace '4' with codelength //
    //*****************************//

    if(checkCode(codeArray, key)){
        digitalWrite(PIN_OUTPUT,HIGH);
        delay(2000);
    }
  }
}

boolean checkCode(char code[], char first) {
  char input[sizeof(code)];
  input[0] = first;
  int index = 1;
  int time = 0;
  while (time < 10000) {
    char key = keypad.getKey();

    if (key) {
      Serial.println(key);
      time = 0;
      input[index] = key;
      index += 1;
      if (index == sizeof(code)) {
        bool cmp = true;
        for(int i = 0; i < sizeof(code);i++){
          if(input[i]!=code[i]){cmp=false;}
        }
        return cmp;
      }
    }
    delay(10);
    time += 1;
  }
  return false;
}

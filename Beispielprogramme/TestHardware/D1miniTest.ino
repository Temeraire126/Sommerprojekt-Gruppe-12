#include <Keypad.h>
#include <epsnow.h>
#include <ESP8266WiFi.h>
typedef struct codeStruct{
  char a[];
} codeStruct;
codeStruct recvData;
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len){
  memcpy(&recvData, incomingData,sizeof(recvData));
 }

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[ROWS] = { 5, 4, 3, 2 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 9, 8, 7, 6 };  //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

#define PIN_OUTPUT D3

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wifi.mode(WIFI_STA);
  if(esp_now_inti() != 0{
    Serial.println("Error");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // put your main code here, to run repeatedly:
  char key = keypad.getKey();

  if (key) {
    Serial.println(key);
    char *code = recvData.a;
    if(checkCode(code, key)){
      digitalWrite(LED_BUILTIN,HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN,LOW);
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
        return strcmp(input, code);
      }
    }
    delay(1000);
    time += 1;
  }
  return false;
}

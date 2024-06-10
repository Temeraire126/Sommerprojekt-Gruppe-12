#include <Keypad.h>

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
}

void loop() {
  // put your main code here, to run repeatedly:
  char key = keypad.getKey();

  if (key) {
    Serial.println(key);
    char code[] = {'0','0','0','0'};
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
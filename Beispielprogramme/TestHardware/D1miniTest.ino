#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
   {'D','#','0','*'},
   {'C','9','8','7'},
   {'B','6','5','4'},
   {'A','3','2','1'}
};
//D0,D1,D2,D4
byte rowPins[ROWS] = { D4, D3, D2, D1 };  //connect to the row pinouts of the keypad
//D5,D6,D7,D8
byte colPins[COLS] = { D8, D7, D6, D5 };  //connect to the column pinouts of the keypad

#define PIN_OUT D0

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

char Taste;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(PIN_OUT, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  char key = keypad.getKey();


  if (key) {
    Serial.println(key);
    char code[] = {'0','0','0','0'};
    if(checkCode(code, key)){
      digitalWrite(PIN_OUT,HIGH);
      delay(5000);
      digitalWrite(PIN_OUT,LOW);
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
        Serial.println(input);
        Serial.println(code);
        bool cmp = true;
        for (int i = 0; i<sizeof(code);i++){
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

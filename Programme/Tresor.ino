#include <Keypad.h>
#include <Servo.h>

#define LED_GREEN ;
#define LED_RED ;

Servo myservo;
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
bool keyThere = false;
int pos = 0;

void setup() {
  myservo.attach();
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  char code[4] = { 0, 0, 0, 0 };  
  //Mit Stanis code ergänzen...
  if (keyThere) {
    char key = keypad.getKey();
    if (key) {
      if (readCode(code, key)) {
        openSesame();
        while (!checkKey) {
          delay(100);
        }
        //ToDo (Stanis) Senden, dass der Schlüssel weg ist!
        closeSesame();
      }
    }
  } else {
    char key = keypad.getKey();
    if (key) {
      openSesame();
      int t = 0;
      while (t < 50) {
        if (checkKey()) {
          //ToDo (Stanis) Senden, dass der Schlüssel wieder da ist!
          closeSesame();
          break;
        } else {
          delay(500);
          t++;
        }
      }
    }
  }
}

boolean readCode(char code[4], char first) {
  char input[4];
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
      if (index == 4) {
        if (strcmp(input, code)) {
          digitalWrite(LED_GREEN, HIGH);
          delay(200);
          digitalWrite(LED_GREEN, LOW);
        } else {
          digitalWrite(LED_RED, HIGH);
          delay(200);
          digitalWrite(LED_RED, LOW);
        }
        return strcmp(input, code);
      }
    }
    delay(1000);
    time += 1;
  }
  return false;
}

void openSesame() {
  //ToDo Werte anpassen!
  //Tür öffnen
  for (pos = 0; pos <= 180; pos += 1) {  // goes from 0 degrees to 180 degrees in steps of 1 degree
    myservo.write(pos);                  // tell servo to go to position in variable 'pos'
    delay(15);                           // waits 15 ms for the servo to reach the position
  }
}

void closeSesame() {
  //ToDo Werte anpassen!
  //Tür schließen
  for (pos = 180; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
    myservo.write(pos);                  // tell servo to go to position in variable 'pos'
    delay(15);                           // waits 15 ms for the servo to reach the position
  }
}

boolean checkKey() {
  //ToDo RFID-Sensor auslesen!!!
  return false;
}

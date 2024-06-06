#include <Keypad.h>
#include <Servo.h>

#define LED_GREEN;
#define LED_RED;
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

Servo myservo;
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int pos = 0;

void setup() {
  // put your setup code here, to run once:
  myservo.attach();
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  char key = keypad.getKey();

  if (key) {
    Serial.println(key);
    if (key == 'A') {
      // Rote LED leuchtet kurz
      digitalWrite(LED_RED, HIGH);
      delay(500);
      digitalWrite(LED_RED, LOW);
    }
    if (key == 'B') {
      // Gruene LED leuchtet kurz
      digitalWrite(LED_GREEN, HIGH);
      delay(500);
      digitalWrite(LED_GREEN, LOW);
    }
    if (key == 'C') {
      //Servo dreht sich in die eine Richtung
      for (pos = 0; pos <= 180; pos += 1) {  // goes from 0 degrees to 180 degrees in steps of 1 degree
        myservo.write(pos);                  // tell servo to go to position in variable 'pos'
        delay(15);                           // waits 15 ms for the servo to reach the position
      }
    }
    if (key == 'D') {
      //Servo dreht sich in die andere Richtung
      for (pos = 180; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
        myservo.write(pos);                  // tell servo to go to position in variable 'pos'
        delay(15);                           // waits 15 ms for the servo to reach the position
      }
    }
  }
}

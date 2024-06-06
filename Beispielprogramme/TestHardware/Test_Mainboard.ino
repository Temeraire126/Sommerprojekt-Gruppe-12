#include <LiquidCrystal.h>

#define LED_RED D2;
#define LED_GREEN D3;
#define BUTTON D1;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // put your setup code here, to run once:
  pinMode(BUTTON, INPUT);
  lcd.begin(16, 2);
}

void loop() {
  digitalWrite(LED_RED, HIGH);
  if(BUTTON==HIGH){
    lcd.print("hello, world!");
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    delay(500);
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
  }

}

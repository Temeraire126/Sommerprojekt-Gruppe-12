#include <Wire.h>                    // Wire Bibliothek einbinden
#include <LiquidCrystal_I2C.h>       // Vorher hinzugefügte LiquidCrystal_I2C Bibliothek einbinden
LiquidCrystal_I2C lcd(0x27, 16, 2);  //Hier wird festgelegt um was für einen Display es sich handelt. In diesem Fall eines mit 16 Zeichen in 2 Zeilen und der HEX-Adresse 0x27. Für ein vierzeiliges I2C-LCD verwendet man den Code "LiquidCrystal_I2C lcd(0x27, 20, 4)"

#define LED_RED D5
#define LED_GREEN D4
#define BUTTON D6

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  lcd.init();       //Im Setup wird der LCD gestartet
  lcd.backlight();  //Hintergrundbeleuchtung einschalten (lcd.noBacklight(); schaltet die Beleuchtung aus).
  lcd.print("hello, world!");
}

void loop() {
  digitalWrite(LED_RED, HIGH);
  if (digitalRead(BUTTON) == LOW) {
    lcd.setCursor(0, 0);
    lcd.print("Knopf gedrückt!");
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    delay(1000);
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
  } else {
    delay(1000);
    lcd.setCursor(0, 0);
    lcd.print("Knopf nicht gedrückt");
  }
}

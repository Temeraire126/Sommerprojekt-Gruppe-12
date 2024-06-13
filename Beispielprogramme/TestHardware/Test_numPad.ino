#include <Keypad.h>

// Definiere die Anzahl der Reihen und Spalten des Keypads
const byte COLS = 4; // 4 Spalten
const byte ROWS = 4; // 4 Zeilen

// Definiere das Keymap
char hexaKeys[ROWS][COLS] = {
  {'D','#','0','*'},
  {'C','9','8','7'},
  {'B','6','5','4'},
  {'A','3','2','1'}
};

// Definiere die Pins für Reihen und Spalten
byte rowPins[COLS] = { D0, D1, D2, D4 }; // Verbinde die Reihenpins zu D5, D6, D7, D8
byte colPins[COLS] = { D5, D6, D7, D8 }; // Verbinde die Spaltenpins zu D0, D1, D2, D4
char Taste;
// Erstelle ein Keypad-Objekt
Keypad Tastenfeld = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(115200);
}

void loop() {
    Taste = Tastenfeld.getKey();

  if (Taste) {
    Serial.println(Taste);
  }
}

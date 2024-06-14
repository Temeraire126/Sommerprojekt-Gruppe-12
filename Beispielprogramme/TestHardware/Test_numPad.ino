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
byte rowPins[ROWS] = { D4, D3, D2, D1 };  //connect to the row pinouts of the keypad
//D5,D6,D7,D8
byte colPins[COLS] = { D8, D7, D6, D5 };  //connect to the column pinouts of the keypad
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

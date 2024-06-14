// RFID-Bibliothek hinzufügen
#include <MFRC522.h>
#include <Servo.h>

//Anschlüsse definieren
#define SDA_Pin D1
#define RST_Pin D4

//LEDs
#define LED_RED D2
#define LED_GREEN D3

#define Input_Keypad A0

//RFID-Empfänger bennen, Pins zuordnen
MFRC522 mfrc522(SDA_Pin, RST_Pin);

boolean keyThere;

Servo myservo;
int pos = 0;

void setup() {
  myservo.attach(D8);

  Serial.begin(115200);
  SPI.begin();

  //Initialsierung des RFID-Empfängers
  mfrc522.PCD_Init();

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
}

void loop() {
  if(keyThere){
    if(digitalRead(Input_Keypad)==HIGH){
      digitalWrite(LED_GREEN,HIGH);
      digitalWrite(LED_RED,LOW);
      openSesame();
      if(!checkKey()){
        keyThere=false;
        digitalWrite(LED_GREEN,LOW);
      }
    }
  } else{
    if(checkKey){
      digitalWrite(LED_GREEN,HIGH);
      delay(2000);
      closeSesame();
      if(!checkKey){
        digitalWrite(LED_GREEN,LOW);
        openSesame();
      }
      digitalWrite(LED_GREEN,LOW);
      digitalWrite(LED_RED,HIGH);
    }
  }
}
boolean checkKey() {
  String WertDEZ;

  //Wenn keine Karte in Reichweite ist...
  if (!mfrc522.PICC_IsNewCardPresent()) {
    //... wird die Abfrage wiederholt.
    return;
  }

  //Wenn kein RFID-Sender ausgewählt wurde...
  if (!mfrc522.PICC_ReadCardSerial()) {
    //...wird die Abfrage wiederholt.
    return;
  }

  Serial.println("Karte entdeckt!");

  //Dezimal-Wert in Strings schreiben
  for (byte i = 0; i < mfrc522.uid.size; i++)  //über jedes Byte der UID der Karte
  {
    //String zusammenbauen
    WertDEZ = WertDEZ + String(mfrc522.uid.uidByte[i], DEC) + " ";
  }

  //trim() -> Leerzeichen am Ende des Strings entfernen
  WertDEZ.trim();

  //Kennung dezimal anzeigen
  Serial.println("Dezimalwert: " + WertDEZ);

  return (WertDEZ == "39 123 81 211");
}

void openSesame() {
  for (pos = 0; pos <= 180; pos += 1) {  //goes from 0 degrees to 180 degrees in steps of 1 degree
    myservo.write(pos);                  //tell servo to go to position in variable 'pos'
    delay(15);                           //waits 15ms for the servo to reach the position
  }
  delay(1000);
}
void closeSesame() {
  for (pos = 180; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
    myservo.write(pos);                  // tell servo to go to position in variable 'pos'
    delay(15);                           // waits 15 ms for the servo to reach the position
  }
}

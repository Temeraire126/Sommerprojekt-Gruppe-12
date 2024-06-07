// RFID-Bibliothek hinzufügen
#include <MFRC522.h>
#include <Servo.h>

//Anschlüsse definieren 
#define SDA_Pin D1
#define RST_Pin D4

//LEDs
# define LED_RED D2
# define LED_GREEN D3

// Zustand der LEDs false = beim Start ausgeschaltet
bool StatusroteLED = false;
bool StatusgrueneLED = false;

//RFID-Empfänger bennen, Pins zuordnen
MFRC522 mfrc522(SDA_Pin, RST_Pin);


//Servo myservo;
  //int pos = 0;
  
void setup() {
  //myservo.attach();

  Serial.begin(115200);
  SPI.begin();

   //Initialsierung des RFID-Empfängers
  mfrc522.PCD_Init();
  
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  }

void loop()
{
String WertDEZ;

//Wenn keine Karte in Reichweite ist...
if (!mfrc522.PICC_IsNewCardPresent())
{
//... wird die Abfrage wiederholt.
return;
}

//Wenn kein RFID-Sender ausgewählt wurde...
if (!mfrc522.PICC_ReadCardSerial())
{
//...wird die Abfrage wiederholt.
return;
}

Serial.println("Karte entdeckt!");

//Dezimal-Wert in Strings schreiben
for (byte i = 0; i < mfrc522.uid.size; i++) //über jedes Byte der UID der Karte
{
//String zusammenbauen
WertDEZ = WertDEZ + String(mfrc522.uid.uidByte[i], DEC) + " "; 
}

//trim() -> Leerzeichen am Ende des Strings entfernen
WertDEZ.trim();

//Kennung dezimal anzeigen
Serial.println("Dezimalwert: " + WertDEZ);

//gruene LED, wenn Chip richtig ist
if (WertDEZ == "39 123 81 211")
{
//gruene LED schalten
StatusgrueneLED = !StatusgrueneLED;
digitalWrite(LED_GREEN, StatusgrueneLED);
digitalWrite(LED_RED, LOW);
}

// Status eandern: rote LED schalten bei falschem Chip
if (WertDEZ == "169 140 161 226")
{
  Serial.println("chip 2");
StatusroteLED = !StatusroteLED;
digitalWrite(LED_RED,StatusroteLED);
digitalWrite(LED_GREEN, LOW);
//openSesame();
}
//Wartezeit verhindert, dass die Karte nur einmal erkannt wird
delay(2000);
//closeSesame();
}

//Methoden für den Servo
/*void openSesame(){
for (pos = 0; pos <= 180; pos += 1){ //goes from 0 degrees to 180 degrees in steps of 1 degree
myservo.write(pos);                  //tell servo to go to position in variable 'pos'
delay(15);                           //waits 15ms for the servo to reach the position
}
}

void closeSesame(){
for (pos = 0; pos <= 180; pos += 1){ //goes from 0 degrees to 180 degrees in steps of 1 degree
myservo.write(pos);                  //tell servo to go to position in variable 'pos'
delay(15);                           //waits 15ms for the servo to reach the position
}
}*/

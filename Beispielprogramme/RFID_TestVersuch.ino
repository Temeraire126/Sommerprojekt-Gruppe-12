// RFID-Bibliothek hinzufügen
#include <MFRC522.h>
#include <Servo.h>

//Anschlüsse definieren 
#define SDS_Pin 10
#define RST_Pin 9

//LEDs
# define LED_RED
# define LED_GREEN

// Zustand der LEDs false = beim Start ausgeschaltet
bool StatusroteLED = false;
bool StatusgrueneLED = false;

//RFID-Empfänger bennen, Pins zuordnen
MFRC522 rfid(SDS_Pin, RST_Pin);


Servo myservo;
  int pos = 0;
  
void setup() {
  myservo.attach();
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  Serial.begin(9600);
  SPI.begin();

  //Initialsierung des RFID-Empfängers
  rfid.PCD_Init();
  }

void loop()
{
String WertDEZ;

//Wenn keine Karte in Reichweite ist...
if (!rfid.PICC_IsNewCardpresent())
{
//... wird die Abfrage wiederholt.
return;
}

//Wenn kein RFID-Sender ausgewählt wurde...
if (!rfid.PICC_ReadCardSerial())
{
//...wird die Abfrage wiederholt.
return;
}

Serial.println("Karte entdeckt!");

//Dezimal-Wert in Strings schreiben
for (byte i = 0; i < MFRC522.uid.size; i++) //über jedes Byte der UID der Karte
{
//String zusammenbauen
WertDEZ = WertDEZ + String(MFRC522.uid.uidByte[i], DEC) + " "; 
}

//trim() -> Leerzeichen am Ende des Strings entfernen
WertDEZ.trim();

//Kennung dezimal anzeigen
Serial.println("Dezimalwert: " + WertDEZ);

//rote LED, wenn Chip falsch ist
if (WerteDEZ != " 1 ")
{

//rote LED schalten
StatusroteLED = !StatusroteLED;
digitalWrite(LED_RED, StatusroteLED);
}

//gruene LED schalten
if (WerteDEZ == " 1 ")
{
LED_GREEN = !LED_GREEN;
digitalWrite(GREEN_LED,StatusgrueneLED);
openSesame();
}
//Wartezeit verhindert, dass die Karte nur einmal erkannt wird
delay(1000);
closeSesame();
}

//Methoden für den Servo
void openSesame(){
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
}

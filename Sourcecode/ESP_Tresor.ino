// Bibliotheken hinzufügen
#include <espnow.h>
#include <ESP8266WiFi.h>
#include <MFRC522.h>
#include <Servo.h>

//Anschlüsse definieren
#define SDA_Pin D1
#define RST_Pin D4
//mainboardadresse und paket definieren
uint8_t addressMain[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}; //hier die MAC-Adresse deines Mainboards einfügen z.B.: 0x08,0x3A,0x8D,0xCF,0xAE,0x38
 typedef struct packageSend{
  bool a;
 } packageSend;
 packageSend sendData;
//LEDs
#define LED_RED D2
#define LED_GREEN D3
//Signal für korrekte Codeeingabe
#define Input_Keypad D0

//RFID-Empfänger bennen, Pins zuordnen
MFRC522 mfrc522(SDA_Pin, RST_Pin);
//PINs RFID:
//SDA  =  D1
//SCK  =  D7
//MOSI =  D6
//MISO =  D5
//IRQ  =  nicht angeschlossen
//RST  =  D4

//Variable zum Speichern ob der Schlüssel momentan im Tresor ist.
boolean keyThere = true;

//Benennung des Servo
Servo myservo;
int pos = 0;

//Aufruf bei Senden von Daten, Inhalt nur für Debug-swecke:
void onDataSent(uint8_t *mac_addr, uint8_t sendstatus){
      Serial.print("Last Packet Send Status: ");
  if(sendstatus == 0){
    Serial.println("Delivery success");
  }
  else {
    Serial.println("Delivery fail");
  }
}


void setup() {
 //INIT für ESPNOW
  WiFi.mode(WIFI_STA);
  if(esp_now_init() != 0){
    Serial.println("Error initilizing ESPNOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(onDataSent);
  esp_now_add_peer(addressMain, ESP_NOW_ROLE_COMBO, 3, NULL, 0);
//Servo festlegen
 myservo.attach(D8);

  Serial.begin(115200);
  SPI.begin();

  //Initialsierung des RFID-Empfängers
  mfrc522.PCD_Init();
  
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
}
//
void loop() {
  delay(10);
  //Schlüssel ist momentan da.
  if(keyThere){
    delay(10);
   //Korrekter Code wurde eingegeben.
    if(digitalRead(Input_Keypad)==LOW){
      digitalWrite(LED_GREEN,HIGH);
      digitalWrite(LED_RED,LOW);
      openSesame();
     //Solange der chlüssel noch da ist warten bis er entfernt wurde.
      while(checkKey()){
        delay(10);
      }
        keyThere=false;
        sendData.a=keyThere;
        esp_now_send(addressMain, (uint8_t *)&sendData,sizeof(sendData));
        digitalWrite(LED_GREEN,LOW);
    
    }
   //Schlüssel ist momentan nicht da.
  } else{
   //Wurde der Schlüssel zurückgelegt?
    if(checkKey()){
      digitalWrite(LED_GREEN,HIGH);
      delay(2000);
      closeSesame();
      keyThere=true;
      sendData.a=keyThere;
      esp_now_send(addressMain, (uint8_t *)&sendData,sizeof(sendData));

      delay(10);
     //wenn der Schlüssel beim Schließen des Tresors entfernt wurde
      if(!checkKey()){
        digitalWrite(LED_GREEN,LOW);
        openSesame();
        keyThere=false;
        sendData.a=keyThere;
        esp_now_send(addressMain, (uint8_t *)&sendData,sizeof(sendData));
        delay(10);
      }
      digitalWrite(LED_GREEN,LOW);
      digitalWrite(LED_RED,HIGH);
   
    }
  }
}

//gibt bei Korrektem Wert im RFID-Chip TRUE zurück, wenn der Wert falsch oder kein Chip erkannt wird, kommt FALSE zurück
boolean checkKey() {
  int i=0;
  int limit = 100;
String WertDEZ;
while(i<limit){
  i++;
if(mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() ){
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

//Hier den Wert deines RFID-Tacks einfügen:
  return (WertDEZ == "XXXXXXX" );
}
}
return false;
}

//öffnet die Tür des Tresors:
void openSesame() {
  Serial.println("open");
  for (pos = 0; pos <= 180; pos += 1) {  //goes from 0 degrees to 180 degrees in steps of 1 degree
    myservo.write(pos);                  //tell servo to go to position in variable 'pos'
    delay(15);                           //waits 15ms for the servo to reach the position
  }
  delay(1000);
}
//schließt die Tür des Tresors:
void closeSesame() {
  Serial.println("close");
  for (pos = 180; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
    myservo.write(pos);                  // tell servo to go to position in variable 'pos'
    delay(15);                           // waits 15 ms for the servo to reach the position
  }
}

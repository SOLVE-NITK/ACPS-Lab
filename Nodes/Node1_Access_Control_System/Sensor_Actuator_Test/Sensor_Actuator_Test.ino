#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN D8
#define RST_PIN D0

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

#define RED_LED D1
#define GREEN_LED D2
#define SERVO_PIN D3

#define CARD1 "23 CB 66 0E"
#define CARD2 "A3 EF 6C A6"

void setup() {
  Serial.begin(115200);  // Initiate a serial communication

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  analogWrite(SERVO_PIN, 0);

  SPI.begin();         // Initiate  SPI bus
  mfrc522.PCD_Init();  // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
}
void loop() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == CARD1)  //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    digitalWrite(GREEN_LED, HIGH);
    delay(3000);
  }

  else {
    Serial.println(" Access denied");
    digitalWrite(RED_LED, HIGH);
    delay(3000);
  }

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}
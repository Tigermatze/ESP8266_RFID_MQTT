#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#define SDA D8
#define RST D3

MFRC522 mfrc522(SDA, RST);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent())
  {
    mfrc522.PICC_ReadCardSerial();

    // Reading and printing current UID
    Serial.print("Currrent UID: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0x0" : " 0x");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    
    delay(1000);

    Serial.println();
  }
}


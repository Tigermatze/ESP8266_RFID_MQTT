/*
# RFID RC522
This code reads the RFID tags from an attache RC522 module.

# Wifi
This code will connect to the given WLAN SSID via DHCP.

# MQTT
This code will connect to an existing MQTT broker and will send a mqtt message to them.
*/

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//#include <MQTTClient.h>
#include "main.h"

#define RC522_SDA D8
#define RC522_RST D3

// Setup RFID RC522
// create an instance from class by calling a constructor.
MFRC522 mfrc522(RC522_SDA, RC522_RST);
// valid IDs of our personal NFC tags
byte rfid_uid_1[] = {0x8C, 0x2E, 0x43, 0x49};
byte rfid_uid_2[] = {0x13, 0x68, 0xF4, 0x14};

// Setup WiFi
const char* WiFi_ssid = "TempelWiFi";
const char* WiFi_password = "Tempe1@H0me";

// Setup MQTT
const char* host = "192.168.0.90"; //die IP des MQTT Brokers
const char* mqttuser = "DeinOptionalerMQTTuser";
const char* mqttpwd = "DeinOptionalesMQTTpasswort";
//MQTTClient mqtt;

void setup() {
  Serial.begin(9600); 
  SPI.begin();
  mfrc522.PCD_Init(); // Initialisierung des RFID ICs
  WiFiconnect(WiFi_ssid, WiFi_password);
  
  Serial.println("Setup completed...");
}

void loop() {

  if (is_RFID_present())
    if (is_RFID_valid())
      send_RFID_via_MQTT();
  
}

void WiFiconnect(const char* ssid, const char* wifi_pass) {
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, wifi_pass);

  
  while(WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(500);
    Serial.println("WiFi connection failed. Retry.");
  }
 
  Serial.print("Wifi connection successful - IP-Address: ");
  Serial.println(WiFi.localIP());

 
 /* mqtt.begin(host, net);
  while (!mqtt.connect(host, mqttuser, mqttpwd)) {
    Serial.print("*");
  }
  Serial.println("MQTT connected!");

*/  delay(500);
}

boolean is_RFID_present()
{
  // nur eine Aktion ausführen, wenn ein RFID Tag erkannt wurde
  if (mfrc522.PICC_IsNewCardPresent())
    return mfrc522.PICC_ReadCardSerial();

  return false;
}

boolean is_RFID_valid()
{
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0x0" : " 0x");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  
    if (mfrc522.uid.uidByte[i] != rfid_uid_1[i])
    {
      if (mfrc522.uid.uidByte[i] != rfid_uid_2[i])
        return false;
    }
  }

  Serial.println();
  delay(1000);
  
  return true;
}

void send_RFID_via_MQTT()
{

}
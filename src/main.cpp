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
#include <PubSubClient.h>
#include <ArduinoOTA.h>
#include "main.h"

// Setup RFID RC522
#define RC522_SDA D8
#define RC522_RST D3
MFRC522 mfrc522(RC522_SDA, RC522_RST); // create an instance from class by calling a constructor.
String rfid_old_value = "";

// Setup WiFi
const char* WiFi_ssid = "TempelWiFi";
const char* WiFi_password = "Tempe1@H0me";

// Setup MQTT
#define mqtt_broker_ip "192.168.0.10"
#define mqtt_broker_port 1883
#define mqtt_user ""
#define mqtt_pwd ""
#define subscribe_topic ""
#define send_topic "Haus/Dachgeschoss/Mika/ESP8266/RFID"
//#define send_string "RFID,Ausgangsort=Dachgeschoss,Sensorort=Mika,Sensor=ESP8266"
#define send_string ""
WiFiClient espClient;
PubSubClient mqtt_client(espClient);

void setup() {
  Serial.begin(9600); 
  SPI.begin();
  mfrc522.PCD_Init(); // initialisation of RFID IC
  WiFi_connect(WiFi_ssid, WiFi_password); // connect to local WiFi
  MQTT_connect(mqtt_broker_ip, mqtt_broker_port); // connect local mqtt broker
  
  Serial.println("Setup completed...");
}

void loop() {
  // check if the connection to mqtt broker is still established, otherwise reconnet
  if (!mqtt_client.connected())
    MQTT_reconnect(mqtt_user, mqtt_pwd, subscribe_topic);

  // check and handle subscribed topics and so on
  mqtt_client.loop();

  // if RFID tag detected, send mqtt message to the mqtt broker
  if (is_RFID_present())
    send_RFID_via_MQTT(send_topic, send_string);
  
}

void WiFi_connect(const char* ssid, const char* wifi_pass) {
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, wifi_pass);

  while(WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(500);
    Serial.println("WiFi connection failed. Retry.");
  }
 
  Serial.print("Wifi connection successful - IP-Address: ");
  Serial.println(WiFi.localIP());

  delay(500);
}

void MQTT_connect(const char* broker_ip, uint16_t broker_port)
{
  mqtt_client.setServer(broker_ip, broker_port);
  mqtt_client.setCallback(MQTT_callback); // Eingang von Nachrichten wird in dieser Funktion überprüft
}

void MQTT_reconnect(const char* user, const char* password, const char* topic)
{
  String clientID = "ESP8266-";

  while (!mqtt_client.connected())
  {
    Serial.println("Attempting MQTT connection...");
    clientID += String(random(0xffff), HEX); // create a random client ID
    
    // try to connect
    if (mqtt_client.connect(clientID.c_str(), user, password))
    {
      Serial.println("connected");
      if (topic != "")
        mqtt_client.subscribe(topic);
    }
    else
    {
      Serial.print("failed, state=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void MQTT_callback(char* topic, byte* message, unsigned int length)
{

}

boolean is_RFID_present()
{
  // nur eine Aktion ausführen, wenn ein RFID Tag erkannt wurde
  if (mfrc522.PICC_IsNewCardPresent())
    return mfrc522.PICC_ReadCardSerial();

  return false;
}

String get_RFID_value()
{
  String value = "";

  for (byte i = 0; i < mfrc522.uid.size; i++) 
    value += String(mfrc522.uid.uidByte[i], DEC);
  
  //Serial.println(value);

  return value;
}

void send_RFID_via_MQTT(const char* topic, const char* mqtt_string)
{
  String rfid_value = "";
  rfid_value = get_RFID_value();

  // only send mqtt message if the tag changed  
  if (rfid_old_value == rfid_value)
    return;
    
  rfid_old_value = rfid_value;

  String value = mqtt_string;
  //value += " rfid_value=";
  value += rfid_value;
  
  /*Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("mqtt_message: ");
  Serial.println(value.c_str());*/
  
  if (!mqtt_client.publish(topic, value.c_str()))
    Serial.println("MQTT publishing not successfull!");
  else 
    Serial.println("MQTT message send");
}
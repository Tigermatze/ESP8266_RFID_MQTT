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
#include <ArduinoJson.h>
#include "main.h"

// Setup RFID RC522
#define RC522_SDA D8
#define RC522_RST D3

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
#define influxdb_string "RFID,Ausgangsort=Dachgeschoss,Sensorort=Mika,Sensor=ESP8266"

// create some instances
MFRC522 mfrc522(RC522_SDA, RC522_RST); // create an instance from class by calling a constructor.
WiFiClient espClient;
PubSubClient mqtt_client(espClient);
StaticJsonDocument<256> json_doc; // Allocate the JSON document

// global variables
boolean rfid_present_old = false;

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
  delay(1000);
  yield();
  mqtt_client.loop();

  // if RFID tag detected, send mqtt message to the mqtt broker
  send_RFID_via_MQTT(send_topic, influxdb_string);
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
      if (strcmp ("", topic) != 0)
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

void send_RFID_via_MQTT(const char* topic, const char* mqtt_string)
{
  boolean rfid_present = is_RFID_present();
  String influxdb_value = mqtt_string;

  // only send mqtt message if the tag changed  
  if (rfid_present_old == rfid_present)
    return;

  String rfid_tag = get_RFID_value();
  
  // create JSON-Format 
  // Add values in the document
  //json_doc["rfid_tag"] = rfid_tag;
  //json_doc["rfid_present"] = rfid_present;

  //char json_out[128];
  //serializeJson(json_doc, json_out);

  // create influxdb Format
  influxdb_value += " rfid_tag=";
  influxdb_value += rfid_tag;
  influxdb_value += ",rfid_available=";
  influxdb_value += ((rfid_present == true)? "true" : "false");

  //Serial.println(influxdb_value);
  //(mqtt_client.publish(topic, json_out))

  if (!mqtt_client.publish(topic, influxdb_value.c_str()))
      Serial.println("MQTT publishing not successfull!");
  else 
    Serial.println("MQTT message send");
  
  rfid_present_old = rfid_present;
}

boolean is_RFID_present()
{
  boolean return_value = false;

  mfrc522.PCD_SoftPowerUp();
  if (mfrc522.PICC_IsNewCardPresent())
    if (mfrc522.PICC_ReadCardSerial())
      return_value = true;

  delay(200);
  mfrc522.PCD_SoftPowerDown();
  delay(500);

  return return_value;
}

String get_RFID_value()
{
  String value = "";

  for (byte i = 0; i < mfrc522.uid.size; i++) 
    value += String(mfrc522.uid.uidByte[i], DEC);

  return value;
}


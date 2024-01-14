#ifndef __main__
#define __main__

#include <Arduino.h>

void WiFi_connect(const char* ssid, const char* wifi_pass);
void MQTT_connect(const char* broker_ip, uint16_t broker_port);
void MQTT_reconnect(const char* user, const char* password, const char* topic);
void MQTT_callback(char* topic, byte* message, unsigned int length);
boolean is_RFID_present();
String get_RFID_value();
void send_RFID_via_MQTT(const char* topic, const char* mqtt_string);

#endif
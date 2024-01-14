#ifndef __main__
#define __main__

#include <Arduino.h>

void WiFiconnect(const char* ssid, const char* wifi_pass);
boolean is_RFID_present();
boolean is_RFID_valid();
void send_RFID_via_MQTT();

#endif
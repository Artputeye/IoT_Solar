#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include <FS.h>           //this needs to be first, or it all crashes and burns...
#include <WiFiManager.h>  //https://github.com/tzapu/WiFiManager

#ifdef ESP32
#include <LittleFS.h>
#endif

#include <ArduinoJson.h>  //https://github.com/bblanchon/ArduinoJson

//define your default values here, if there are different values in config.json, they are overwritten.
extern char DEVICE_NAME[28]; //= "INVERTER";  //"Anern 4.2kW";  //"ARRTECH INVERTER";
extern char MQTT_ADDR[16]; //= "192.168.101.100";//"192.168.1.247";
extern char MQTT_USERNAME[28]; //= "mqtt-user";//"inverter";  // replace with your credentials
extern char MQTT_PASSWORD[28] ;//= "1234";
extern char MQTT_PORT[5]; //= "1883";

// flag for saving data
extern bool shouldSaveConfig;

void wifi_para();
void saveConfigCallback();
void fsformat();

#endif
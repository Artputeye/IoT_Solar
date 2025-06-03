#ifndef GLOBALS_H
#define GLOBALS_H

#define RX_pin 16 // pin receive to inverter
#define TX_pin 17 // pin transmission to inverter
#define LED 2     // Led status working

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WiFiClient.h>
#include <AsyncTCP.h>

#include <ArduinoJson.h>  //https://github.com/bblanchon/ArduinoJson
#include <ArduinoHA.h>

#include <FS.h>   
#ifdef ESP32
#include <LittleFS.h>
#endif

#include "invCommand.h"


// declare global object out source cpp
extern AsyncWebServer server;
extern WiFiClient client;
extern HADevice device;
extern HAMqtt mqtt;
extern invCommand inv;

//define your default values here, if there are different values in config.json, they are overwritten.
extern char DEVICE_NAME[28]; //= "INVERTER";  //"Anern 4.2kW";  //"ARRTECH INVERTER";
extern char MQTT_ADDR[16]; //= "192.168.101.100";//"192.168.1.247";
extern char MQTT_USERNAME[28]; //= "mqtt-user";//"inverter";  // replace with your credentials
extern char MQTT_PASSWORD[28] ;//= "1234";
extern char MQTT_PORT[5]; //= "1883";

// flag for saving data
extern bool shouldSaveConfig;

extern int t ;
extern bool test;
extern int ledState;


#endif
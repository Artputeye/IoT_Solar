#ifndef GLOBALS_H
#define GLOBALS_H

/////////////////////////////////////////////////////////////////////////
// Libraries
#include <WiFi.h>
#include <WiFiClient.h>
#include <AsyncTCP.h>
#include <WebServer.h>
#include <ESPAsyncWebServer.h>
#include <Update.h>
#include <ESPmDNS.h>
#include <WiFiManager.h>
#include <WiFiMulti.h>
#include <ArduinoJson.h>
#include <ArduinoHA.h>
#include <esp_task_wdt.h>
#include "base64.h"
#include <FS.h>
#ifdef ESP32
#include <LittleFS.h>
#endif

/////////////////////////////////////////////////////////////////////////
// Sub Prongrams
#include "wifiConfig.h"
#include "websocket.h"
#include "webHandle.h"
#include "ota.h"
#include "invCommand.h"
#include "parameterData.h"
#include "iotHA.h"
#include "simulate.h"

/////////////////////////////////////////////////////////////////////////
#define WDT_TIMEOUT 120
#define RX_pin 16 // pin receive to inverter
#define TX_pin 17 // pin transmission to inverter
#define LED 2     // Led status working

// declare global object out source cpp
// extern class object;
extern AsyncWebServer server;
extern WiFiManager wm;

extern WiFiClient client;
extern HADevice device;
extern HAMqtt mqtt;
extern HASwitch grid;

extern WiFiMulti Multi;

extern invCommand inv;

#define FILESYSTEM LittleFS
extern File fsUploadFile;

// define your default values here, if there are different values in config.json, they are overwritten.
extern char DEVICE_NAME[28]; //= "INVERTER";  //"Anern 4.2kW";  //"ARRTECH INVERTER";
extern char WIFI_NAME[30];
extern char PASSWORD[25];
extern char MQTT_ADDR[16];     //= "192.168.101.100";//"192.168.1.247";
extern char MQTT_USERNAME[28]; //= "mqtt-user";//"inverter";  // replace with your credentials
extern char MQTT_PASSWORD[28]; //= "1234";
extern char MQTT_PORT[5];      //= "1883";

extern char D_SoftwareVertion[15];
extern char D_Mfac[15];
extern char D_Model[15];

extern char user[10];
extern char pass[10];

extern uint8_t Mac[6];

extern float power_check;
extern const char *targetDirectory;

extern int ledState;

extern String inputStr;
extern String invStr;

#endif
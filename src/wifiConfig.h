#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include <Arduino.h>
#include "globals.h"

extern const uint8_t AP_PIN;
extern char MacAddr[18];

// ===== Core =====
void wifiSetup();
void wifiLoop();
void restartCheck();

// ===== WiFi Mode =====
void setupWiFiMode();
void setupIPConfig();
void APmode();

// ===== Config =====
void readNetwork();
void mac_config();

// ===== Utils =====
IPAddress parseIP(const char *ipStr);
void showAPClients();

// ===== Events =====
void WiFiEvent(WiFiEvent_t event);

#endif
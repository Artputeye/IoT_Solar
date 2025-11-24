#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H
#include "globals.h"

void restart();
void APmode();
void wifi_Setup();
void mac_config();
void readNetwork();
void setupWiFiMode();
void setupIPConfig();
IPAddress parseIP(const char *ipStr);
void showAPClients();

#endif
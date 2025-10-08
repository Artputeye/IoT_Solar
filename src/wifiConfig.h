#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H
#include "globals.h"

void ledIndicator(unsigned long onTime, unsigned long offTime) ;
void mac_config();
void wifi_config();
void restart();
void readNetwork();
void setupWiFiMode();
void setupIPConfig();
IPAddress parseIP(const char *ipStr);
void showAPClients();

#endif
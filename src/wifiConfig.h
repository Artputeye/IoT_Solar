#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H
#include "globals.h"

void wifi_config();
void mac_config();
void restart();
void readNetwork();
void setupWiFiMode();
void setupIPConfig();
IPAddress parseIP(const char *ipStr);
void showAPClients();
void ledStats();
void ledIndicator(unsigned long onTime, unsigned long offTime) ;

#endif
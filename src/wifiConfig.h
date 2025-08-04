#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H
#include "globals.h"

void mac_config();
void wifi_config();
void wifiManager();
void wifiMulti();
void ledIndicator(unsigned long onTime, unsigned long offTime) ;
void restart();

#endif
#ifndef IOT_HA_H
#define IOT_HA_H

#include "globals.h"
#include <ArduinoHA.h>
#include <esp_wifi.h>

void MacSetup();
void iotHAsetup();
void readMacAddress();
void iotHA();

#endif
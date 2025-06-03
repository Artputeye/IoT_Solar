#ifndef IOT_HA_H
#define IOT_HA_H

#include <WiFi.h>
#include <ArduinoHA.h>
#include <esp_wifi.h>
#include "wifiConfig.h"
#include "invCommand.h"
#include "ota.h"

void MacSetup();
void iotHAsetup();
void readMacAddress();
void iotHA();

#endif
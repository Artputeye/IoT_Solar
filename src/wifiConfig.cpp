#include "wifiConfig.h"

void wifi_config()
{
    wifiManager();
    wifiMulti();
}

void wifiManager()
{
    WiFiManager wm;
    bool res;
    res = wm.autoConnect(DEVICE_NAME, PASSWORD);

    if (!res)
    {
        Serial.println("Failed to connect");
        delay(10000);
        ESP.restart();
    }
    else
    {
        Serial.println();
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    }
}

void wifiMulti()
{
    ////////////////
}

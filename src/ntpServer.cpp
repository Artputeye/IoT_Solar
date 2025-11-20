#include "ntpServer.h"
const char *serverUrl = "pool.ntp.org";
const long gmtOffset_sec = 7 * 3600;
const int daylightOffset_sec = 0;
int volatile dateNow = 0;
struct tm timeinfo;

void timeServer()
{
    if (wifimode == 1)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            configTime(gmtOffset_sec, daylightOffset_sec, serverUrl);
            timeRefresh();
            Serial.println("Syncing time from NTP...");
        }
    }
}

void timeRefresh()
{
    int retry = 0;
    if (wifimode == 1)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            while (!getLocalTime(&timeinfo) && retry < 10)
            {
                Serial.print(".");
                delay(1000);
                retry++;
            }

            if (!getLocalTime(&timeinfo))
            {
                Serial.println("\n❌ Failed to obtain time (still no NTP response)");
            }
            else
            {
                dateNow = timeinfo.tm_mday;
            }
        }
    }
}
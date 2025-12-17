#include "ntpServer.h"

DateTimeCache rtc;

volatile bool ntpSynced = false;
volatile bool needNtpSync = false;

TaskHandle_t ntpTaskHandle = NULL;
TimerHandle_t ntpResyncTimer = NULL;

void NTPbegin()
{
    ntpResyncTimer = xTimerCreate(
        "ntpResync",
        pdMS_TO_TICKS(86400000), // 1 วัน
        pdTRUE,
        NULL,
        ntpResyncCallback);

    xTimerStart(ntpResyncTimer, 0);
}

void TaskNTP(void *pvParameters)
{
    struct tm t;   // ✅ ต้องประกาศตรงนี้

    while (1)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            if (getLocalTime(&t, 10))
            {
                updateDateCache(t);   // ✅ ส่ง t เข้าไป

                ntpSynced = true;
                Serial.println("✅ NTP synced → task stop");

                ntpTaskHandle = NULL;
                vTaskDelete(NULL);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

void ntpResyncCallback(TimerHandle_t xTimer)
{
    needNtpSync = true;
}

void timeUpdade()
{
    time_t now = time(NULL);
    struct tm t;
    localtime_r(&now, &t);
    updateDateCache(t);
}

void updateDateCache(const struct tm &t)
{
    rtc.day    = t.tm_mday;
    rtc.month  = t.tm_mon + 1;
    rtc.year   = t.tm_year + 1900;
    rtc.hour   = t.tm_hour;
    rtc.minute = t.tm_min;
    rtc.second = t.tm_sec;
}

void resyncTime()
{
    if (needNtpSync && WiFi.status() == WL_CONNECTED)
    {
        needNtpSync = false;
        ntpSynced = false;

        if (ntpTaskHandle == NULL)
        {
            Serial.println("⏰ NTP resync");
            xTaskCreatePinnedToCore(
                TaskNTP,
                "TaskNTP",
                4096,
                NULL,
                1,
                &ntpTaskHandle,
                1);
        }
    }
}

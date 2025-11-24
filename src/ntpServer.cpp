#include "ntpServer.h"

struct tm timeinfo;
int dateNow = 0;

// รายชื่อ NTP server สำรอง
const char *ntpList[] = {
    "time.google.com",
    "time1.google.com",
    "time2.google.com",
    "time3.google.com",
    "time4.google.com"};

int ntpIndex = 0;
const int ntpCount = sizeof(ntpList) / sizeof(ntpList[0]);

bool hasSyncedOnce = false; 
unsigned long lastTry = 0;
const unsigned long retryDelay = 2000; // 2 วินาทีต่อ 1 server (ไม่ block)

void initNTP()
{
    configTime(7 * 3600, 0, ntpList[ntpIndex]);
    Serial.printf("Initial NTP server: %s\n", ntpList[ntpIndex]);
    delay(3000);
    ntpLoop();
}

void ntpLoop()
{
   
    if (getLocalTime(&timeinfo))
    {
        if (!hasSyncedOnce)
            Serial.println("NTP sync complete (first sync)");

        hasSyncedOnce = true;
        dateNow = timeinfo.tm_mday;
        return;
    }

    // 2) เวลาหาย / ยังไม่เคยซิง ต้องเริ่ม sync
    unsigned long now = millis();
    if (now - lastTry < retryDelay)
        return; // 2 วินาที ค่อยลองใหม่
    lastTry = now;

    Serial.printf("NTP failed → try server: %s\n", ntpList[ntpIndex]);

    // Poll แบบ non-blocking (เช็ค 200 ms)
    unsigned long t0 = millis();
    bool ok = false;

    while (millis() - t0 < 200)
    {
        if (getLocalTime(&timeinfo))
        {
            ok = true;
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(20)); // ไม่ block core
    }

    if (ok)
    {
        Serial.printf("NTP synced via: %s\n", ntpList[ntpIndex]);
        hasSyncedOnce = true;
        dateNow = timeinfo.tm_mday;
        return;
    }

    // 3) ถ้า server นี้ยังไม่ตอบ → สลับตัวถัดไป
    ntpIndex++;
    if (ntpIndex >= ntpCount)
        ntpIndex = 0;

    Serial.printf("Switching to next NTP server: %s\n", ntpList[ntpIndex]);

    configTime(7 * 3600, 0, ntpList[ntpIndex]);
}
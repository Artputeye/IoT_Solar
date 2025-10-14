#include "gridOperation.h"

struct tm timeinfo;
float energy_kWh = 0.0;
float gridPower = 0.0;

unsigned long lastQvalue = 0;
unsigned long lastQrate = 0;
unsigned long lastRespons = 0;
unsigned long lastGridOpr = 0;
unsigned long lastGridCheck = 0;

const unsigned long interval = 1000;            // วัดทุก 1 วินาที
const unsigned long gridCheckInterval = 900000; // 15 นาที (15 * 60 * 1000 ms)
bool toggle;
bool gridState = false;

void timeServer()
{
    if (wifimode == 1)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
            timeRefresh();
            loadEnergyFromFile();
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
            while (!getLocalTime(&timeinfo) && retry < 3)
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
                // Serial.println("\n✅ Time synchronized successfully");
                // Serial.printf("Current time: %02d-%02d-%04d %02d:%02d:%02d\n",
                // timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900,
                // timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
                dateNow = timeinfo.tm_mday;
                // Serial.printf("Today = %d\n", dateNow);
            }
        }
    }
}

void gridRun()
{
    if ((millis() - lastQvalue) > 3000) // Question to Inverter
    {
        lastQvalue = millis();
        if (inv.RunMode)
        {
            inv.cmd_inv("QPIGS");
        }
        inv.Response();
        wsJsonInverter(inv.invData);
        iotHArun();
        simulateData();
    }

    if ((millis() - lastQrate) > 10000) // Question Rate to Inverter
    {

        if (inv.RunMode)
        {
            if (toggle)
            {
                inv.cmd_inv("QPIRI"); // ส่งคำสั่งแรก
            }
            else
            {
                inv.cmd_inv("QPIWS"); // ส่งคำสั่งที่สอง
            }
        }
        toggle = !toggle;
        lastQrate = millis();
    }

    if ((millis() - lastRespons) > 100) // Respons from Inverter
    {
        lastRespons = millis();
        inv.serialSent();
        wsJsonSerial(inv.serialData);
        inv.Response();
    }
}

void gridOperation()
{
    handleEnergyStorage();
    float outputPower = 0.0;
    float pvPower = 0.0;

    // ==== ทำงานทุก interval ====
    unsigned long currentMillis = millis();
    if (currentMillis - lastGridOpr >= interval)
    {
        lastGridOpr = currentMillis;
        outputPower = inv.data.ActivePower;
        pvPower = inv.data.pvPower;
        gridPower = outputPower - pvPower;

        // ✅ 2. เคลียร์ไฟล์ตอน 18:00:00 - 18:00:10
        if (timeinfo.tm_hour == 18 && timeinfo.tm_min == 0 && timeinfo.tm_sec >= 0 && timeinfo.tm_sec <= 10)
        {
            clearEnergyFile();
            energy_kWh = 0.0;
            inv.energy = true;
            delay(2000);
        }
        if (inv.energy)
        {
            clearEnergyFile();
            energy_kWh = 0.0;
            inv.energy = false;
            ESP.restart();
            delay(2000);
        }
        // คำนวณพลังงานสะสม
        energy_kWh += (gridPower * interval) / 3600.0; // Wh → kWh
        // Serial.printf("Grid Power: %.2f W | Output: %.2f W | PV: %.2f W | Energy: %.6f kWh\n",
        //               gridPower, outputPower, pvPower, energy_kWh);
    }

    // ✅ ตรวจสอบ/สั่ง Grid ทุก 15 นาที
    if (currentMillis - lastGridCheck >= gridCheckInterval)
    {
        lastGridCheck = currentMillis;

        if (timeinfo.tm_mday >= gridCutOff && timeinfo.tm_mday <= gridStart)
        {
            inv.valueToinv("GridTieOperation", 0); // ปิด Grid
            return;
        }
        if (inv.gridOpr)
        {
            if (energy_kWh < 1.0)
            {
                inv.valueToinv("GridTieOperation", 0); // ปิด Grid
                gridState = false;
                Serial.println("🔴 ส่งคำสั่ง: Grid OFF (energy < 1.0 kWh)");
            }
            else if (energy_kWh > 2.0)
            {
                inv.valueToinv("GridTieOperation", 1); // เปิด Grid
                gridState = true;
                Serial.println("🟢 ส่งคำสั่ง: Grid ON (energy > 2.0 kWh)");
            }
            else
            {
                // กรณี energy อยู่ระหว่าง 1–2 → ส่งซ้ำตามสถานะปัจจุบัน
                inv.valueToinv("GridTieOperation", gridState ? 1 : 0);
                Serial.printf("⚙️  ยืนยันสถานะ Grid %s (energy = %.2f)\n",
                              gridState ? "ON" : "OFF", energy_kWh);
            }
        }
    }
}

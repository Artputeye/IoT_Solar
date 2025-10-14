#include "gridOperation.h"

struct tm timeinfo;
float energy_kWh = 0.0;
float gridPower = 0.0;
//////////////////////////////////////////////////////////////////////////////////
unsigned long lastQvalue = 0;
unsigned long lastQrate = 0;
unsigned long lastRespons = 0;
unsigned long lastFile = 0;
unsigned long lastGridOpr = 0;
unsigned long lastGridCheck = 0;
unsigned long lastEnergy = 0;
/////////////////////////////////////////////////////////////////////////////////
const unsigned long qvalInterval = 3000;
const unsigned long qrateInterval = 10000;
const unsigned long resInterval = 100;
const unsigned long fileInterval = 60 * 1000;
const unsigned long gridOprInterval = 1000;
const unsigned long gridCheckInterval = 900000; // 15 นาที (15 * 60 * 1000 ms)
const unsigned long energyInterval = 1000;
/////////////////////////////////////////////////////////////////////////////////
bool toggle;
bool gridState = false;
/////////////////////////////////////////////////////////////////////////////////
void timeServer()
{
    if (wifimode == 1)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
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
    if ((millis() - lastQvalue) > qvalInterval) // Question to Inverter
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
    if ((millis() - lastQrate) > qrateInterval) // Question Rate to Inverter
    {
        lastQrate = millis();
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
    }
    if ((millis() - lastRespons) > resInterval) // Respons from Inverter
    {
        lastRespons = millis();
        inv.serialSent();
        wsJsonSerial(inv.serialData);
        inv.Response();
    }
}

void gridOperation()
{
    unsigned long currentMillis = millis();
    float dt = (currentMillis - lastEnergy) / 1000.0; // หน่วยวินาที
    lastEnergy = currentMillis;
    float outputPower = 0.0;
    float pvPower = 0.0;
    outputPower = inv.data.ActivePower;
    pvPower = inv.data.pvPower;
    gridPower = outputPower - pvPower;

    if (timeinfo.tm_hour == 18 && timeinfo.tm_min == 0 && timeinfo.tm_sec >= 0 && timeinfo.tm_sec <= 10)
    {
        clearEnergyFile();
        energy_kWh = 0.0;
        inv.energy = true;
        delay(100);
    }
    if (inv.energy)
    {
        clearEnergyFile();
        energy_kWh = 0.0;
        inv.energy = false;
        delay(100);
    }

    energy_kWh += ((gridPower * dt) / 3600000);
    energy_kWh = roundf(energy_kWh * 1000) / 1000.000;

    if ((millis() - lastFile) > fileInterval) // Respons from Inverter
    {
        lastFile = millis();
        saveEnergyToFile();
        delay(100);
    }
    if (currentMillis - lastGridCheck >= gridCheckInterval)
    {
        lastGridCheck = currentMillis;
        if (timeinfo.tm_mday >= gridCutOff && timeinfo.tm_mday <= gridStart)
        {
            inv.valueToinv("GridTieOperation", 0); 
            return;
        }
        if (inv.gridOpr)
        {
            if (energy_kWh < 1.0)
            {
                inv.valueToinv("GridTieOperation", 0); 
                gridState = false;
                Serial.println("🔴 ส่งคำสั่ง: Grid OFF (energy < 1.0 kWh)");
            }
            else if (energy_kWh > 2.0)
            {
                inv.valueToinv("GridTieOperation", 1); 
                gridState = true;
                Serial.println("🟢 ส่งคำสั่ง: Grid ON (energy > 2.0 kWh)");
            }
            else
            {
                inv.valueToinv("GridTieOperation", gridState ? 1 : 0);
                Serial.printf("⚙️  ยืนยันสถานะ Grid %s (energy = %.2f)\n",
                              gridState ? "ON" : "OFF", energy_kWh);
            }
        }
    }
}
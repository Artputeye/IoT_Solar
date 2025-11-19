#include "gridOperation.h"

//////////////////////////////////////////////////////////////////////////////////

float energy_kWh = 0.0;
float gridPower = 0.0;
const float GRID_ON_THRESHOLD = 2.0;
const float GRID_OFF_THRESHOLD = 1.0;
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
const unsigned long gridCheckInterval = 1 * 60 * 1000 ; //(1 * 60 * 1000 ms)
const unsigned long energyInterval = 1000;
/////////////////////////////////////////////////////////////////////////////////
bool toggle;
bool gridState = false;
/////////////////////////////////////////////////////////////////////////////////

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
        //wsJsonInverter(inv.invData);
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
    float dt = (currentMillis - lastEnergy) / 1000.0;
    lastEnergy = currentMillis;

    float outputPower = inv.data.ActivePower;
    float pvPower = inv.data.pvPower;
    gridPower = outputPower - pvPower;

    static bool clearedToday = false;
    if (timeinfo.tm_hour == 18 && timeinfo.tm_min == 0 && !clearedToday)
    {
        Serial.println("🕕 18:00 detected — initiating daily reset...");
        bool success = clearEnergyFile();

        if (!success)
        {
            Serial.println("⚠️ Warning: daily reset failed, retrying...");
            delay(1000);
            success = clearEnergyFile();
        }

        if (success)
            Serial.println("✅ Daily energy reset complete");
        else
            Serial.println("❌ Daily reset failed after retry");

        clearedToday = true;
    }

    if (timeinfo.tm_hour == 0 && timeinfo.tm_min == 0)
    {
        clearedToday = false;
    }

    energy_kWh += ((gridPower * dt) / 3600000.0);

    if (millis() - lastGridOpr > gridOprInterval) // debug grid operation
    {
        lastGridOpr = millis();
        if (inv.test)
        {
            Serial.println("Grid Operate : " + String(inv.gridOpr));
        }
    }

    if (millis() - lastFile > fileInterval)
    {
        lastFile = millis();
        saveEnergyToFile();
    }

    if (millis() - lastGridCheck > gridCheckInterval)
    {
        lastGridCheck = millis();
        wsJsonSerial("Grid Operation Check");
        if (timeinfo.tm_mday >= gridCutOff && timeinfo.tm_mday <= gridStart)
        {
            inv.valueToinv("GridTieOperation", 0);
            wsJsonSerial("Grid OFF (within cut-off period)");
            return;
        }

        if (inv.gridOpr)
        {
            if (energy_kWh < GRID_OFF_THRESHOLD)
            {
                inv.valueToinv("GridTieOperation", 0);
                gridState = false;
                Serial.println("🔴 Grid OFF (energy < 1.0 kWh)");
            }
            else if (energy_kWh > GRID_ON_THRESHOLD)
            {
                inv.valueToinv("GridTieOperation", 1);
                gridState = true;
                Serial.println("🟢 Grid ON (energy > 2.0 kWh)");
            }
            else
            {
                inv.valueToinv("GridTieOperation", gridState ? 1 : 0);
                Serial.printf("⚙️ Confirming Grid %s (energy = %.3f)\n",
                              gridState ? "ON" : "OFF", energy_kWh);
            }
        }
    }
}

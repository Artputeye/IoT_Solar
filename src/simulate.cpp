#include "simulate.h"

void simulateData()
{
    int i;
    i = random(0, 100);
    if (inv.test)
    {
        // Serial.println("Test on");
        float volt = 230.3 + (float(i) / 10) + (float(i) / 100);
        float fre = 50.1 + (float(i) / 100);
        float vbus = 360 + (i / 10);
        float t = 30 + (i / 10);
        float appar = 520 + (i / 10);
        float active = 367 + (i / 10);
        float per = (active / 4200)*100;
        float pvc = 7.2 + (float(i) / 100);
        float pvv = 335.7 + (float(i) / 10) + (float(i) / 100);
        float batt = 23.3 + (float(i) / 10);
        float power = pvc * pvv;

        inv.data.ApparentPower = appar;
        inv.data.ActivePower = active;
        inv.data.ActivePower = power;
        inv.data.loadPercent = per;
        inv.data.pvCurrent = pvc;
        inv.data.pvVoltage = pvv;
        inv.data.gridVoltage = volt;
        inv.data.outputVoltage = volt;
        inv.data.busVoltage = vbus;
        inv.data.batteryVoltage = batt;
        inv.data.gridFrequency = fre;
        inv.data.outputFrequency = fre;
        inv.data.temp = t;

        GridVolt.setValue(volt);
        GridFrequency.setValue(fre);
        Voltage.setValue(volt);
        Frequency.setValue(fre);
        ApparentPower.setValue(appar);
        ActivePower.setValue(active);
        LoadPercent.setValue(per);
        BusVoltage.setValue(vbus);
        Temp.setValue(t);
        pvCurrent.setValue(pvc);
        pvVoltage.setValue(pvv);

        Serial.println("V:" + String(volt, 2));
    }

}
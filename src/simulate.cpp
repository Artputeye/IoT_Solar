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
        float per = (active / 4200) * 100;
        float pvc = 7.2 + (float(i) / 100);
        float pvv = 335.7 + (float(i) / 10) + (float(i) / 100);
        float batt = 23.3 + (float(i) / 10);
        float power = pvc * pvv;
        float curr = appar / volt; // I=S/V
        float pf = active / appar;

        // Monitor/////////////////////////////////////////////////

        inv.data.ApparentPower = appar;
        inv.data.ActivePower = active;
        inv.data.loadPercent = per;
        inv.data.powerFactor = pf;
        inv.data.pvPower = power;
        inv.data.pvCurrent = pvc;
        inv.data.pvVoltage = pvv;
        inv.data.outputVoltage = volt;
        inv.data.outputCurrent = curr;
        inv.data.outputFrequency = fre;
        inv.data.busVoltage = vbus;
        inv.data.batteryVoltage = batt;
        inv.data.temp = t;

        // Status////////////////////////////////////////////////

        inv.rated.OutputRatingVoltage = 230;
        inv.rated.OutputRatingFrequency = 50;
        inv.rated.OutputRatingCurrent = 20;
        inv.rated.OutputRatingApparentPower = 1234;
        inv.rated.OutputRatingActivePower = 1221;
        inv.rated.BatteryRatingVoltage = 24.8;
        inv.rated.BatteryReChargeVoltage = 21;
        inv.rated.BatteryUnderVoltage = 22;
        inv.rated.BatteryBulkVoltage = 22;
        inv.rated.BatteryFloatVoltage = 21;
        inv.rated.MaxAC_ChargingCurrent = 5;
        inv.rated.MaxChargingCurrent = 14;

        // Home Assistant////////////////////////////////////////

        OutputVolt.setValue(volt);
        OutputCurrent.setValue(curr);
        OutputFrequency.setValue(fre);
        ApparentPower.setValue(appar);
        ActivePower.setValue(active);
        PowerFactor.setValue(pf);
        LoadPercent.setValue(per);
        BusVoltage.setValue(vbus);
        Temp.setValue(t);
        pvCurrent.setValue(pvc);
        pvVoltage.setValue(pvv);
        pvPower.setValue(power);

        Serial.println("V:" + String(volt, 2));
    }
}
#ifndef IOT_HA_H
#define IOT_HA_H
#include "globals.h"

extern HASwitch grid;
// extern HASwitch beeps("beeps");
// extern HASwitch lcdblacklight("lcdblacklight");
// extern HASwitch returnlcd("returnlcd");
// extern HASwitch powersaveing("powersaveing");
// extern HASwitch overtemp("overtemp");
// extern HASwitch ledlight("ledlight");
// extern HASwitch buzzer("buzzer");
// extern HASwitch dualout("dualout");
// extern HASwitch overloadres("overloadres");
// extern HASwitch transfer("transfer");

// extern HASelect InputRang("ACInputRang");
// extern HASelect PrioritySelect("PrioritySelect");
// extern HASelect OutputSelect("OutputSelect");

extern HASensorNumber GridVolt;
extern HASensorNumber GridFrequency;
extern HASensorNumber Voltage;
extern HASensorNumber Frequency;
extern HASensorNumber ApparentPower;
extern HASensorNumber ActivePower;
extern HASensorNumber LoadPercent;
extern HASensorNumber BusVoltage;
extern HASensorNumber Temp;
extern HASensorNumber pvCurrent;
extern HASensorNumber pvVoltage;
extern HASensorNumber pvPower;

void iotHArun();
void iotHAsetup();

void GridTie(bool state, HASwitch *sender);
// void BeepsInterrupt(bool state, HASwitch *sender);
// void BeepsInterrupt(bool state, HASwitch *sender);
// void ReturnToPage(bool state, HASwitch *sender);
// void OverTempAutoRestart(bool state, HASwitch *sender);
// void Buzzer(bool state, HASwitch *sender);
// void DualOutput(bool state, HASwitch *sender);
// void OverloadAutoRestarte(bool state, HASwitch *sender);
// void TransferToBypassOverload(bool state, HASwitch *sender);
// void ACInputRang(int8_t index, HASelect *sender);
// void OutputSourcePriority(int8_t index, HASelect *sender);

#endif
//iotHA.cpp
#include "iotHA.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Switch object define
HASwitch grid("grid");
// HASwitch beeps("beeps");
// HASwitch lcdblacklight("lcdblacklight");
// HASwitch returnlcd("returnlcd");
// HASwitch powersaveing("powersaveing");
// HASwitch overtemp("overtemp");
// HASwitch ledlight("ledlight");
// HASwitch buzzer("buzzer");
// HASwitch dualout("dualout");
// HASwitch overloadres("overloadres");
// HASwitch transfer("transfer");

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Selection object define
// HASelect InputRang("ACInputRang");
// HASelect PrioritySelect("PrioritySelect");
// HASelect OutputSelect("OutputSelect");

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sensor object define
HASensorNumber OutputVolt("OutputVolt", HASensorNumber::PrecisionP1);
HASensorNumber OutputCurrent("OutputCurrent", HASensorNumber::PrecisionP1);
HASensorNumber OutputFrequency("OutputFrequency", HASensorNumber::PrecisionP1);
HASensorNumber ApparentPower("ApparentPower");
HASensorNumber ActivePower("ActivePower");
HASensorNumber LoadPercent("LoadPercent");
HASensorNumber BusVoltage("BusVoltage");
HASensorNumber Temp("Temp");
HASensorNumber pvCurrent("pvCurrent", HASensorNumber::PrecisionP1);
HASensorNumber pvVoltage("pvVoltage", HASensorNumber::PrecisionP1);
HASensorNumber pvPower("pvPower", HASensorNumber::PrecisionP1);

////////////////////////////////////////////////////////////////////////////////////////
// sent inverter data to MQTT broker
void iotHArun()
{
    if (!inv.test)
    {
        float power;
        OutputVolt.setValue(inv.data.outputVoltage);
        OutputCurrent.setValue(inv.data.outputCurrent);
        OutputFrequency.setValue(inv.data.outputFrequency);
        ApparentPower.setValue(inv.data.ApparentPower);
        ActivePower.setValue(inv.data.ActivePower);
        LoadPercent.setValue(inv.data.loadPercent);
        BusVoltage.setValue(inv.data.busVoltage);
        Temp.setValue(inv.data.temp);
        pvCurrent.setValue(inv.data.pvCurrent);
        pvVoltage.setValue(inv.data.pvVoltage);
        power = inv.data.pvCurrent * inv.data.pvVoltage;
        pvPower.setValue(power);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void iotHAsetup()
{
    Serial.println("Home assistnt setup: ");
    /////////////////////////////////////////////////////////////////////////////
    // set device's details (optional)
    device.setName(DEVICE_NAME);
    device.setSoftwareVersion(D_SoftwareVertion);
    device.setManufacturer(D_Mfac);
    device.setModel(D_Model);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Switch device state
    grid.onCommand(GridTie);
    grid.setName("Grid Tie");
    grid.setIcon("mdi:transmission-tower-export");
    grid.getCurrentState();

    // beeps.onCommand(BeepsInterrupt);
    // beeps.setName("Beeps While Primary Suorce Interrupt");
    // beeps.setIcon("mdi:bell-ring");
    // beeps.getCurrentState();

    // lcdblacklight.onCommand(Blacklight);
    // lcdblacklight.setName("LCD Blacklight");
    // lcdblacklight.setIcon("mdi:monitor-shimmer");
    // lcdblacklight.getCurrentState();

    // returnlcd.onCommand(ReturnToPage);
    // returnlcd.setName("Return To The Main LCD Page");
    // returnlcd.setIcon("mdi:home");
    // returnlcd.getCurrentState();

    // powersaving.onCommand(PowerSavingMode);
    // powersaving.setName("Power Saving Mode");
    // powersaving.setIcon("mdi:power-standby");
    // powersaving.getCurrentState();

    // overtemp.onCommand(OverTempAutoRestart);
    // overtemp.setName("Over Temp Auto Restart");
    // overtemp.setIcon("mdi:thermometer-alert");
    // overtemp.getCurrentState();

    // ledlight.onCommand(LedPatternlight);
    // ledlight.setName("Led Pattern Light");
    // ledlight.setIcon("mdi:home");
    // ledlight.getCurrentState();

    // buzzer.onCommand(Buzzer);
    // buzzer.setName("Buzzer");
    // buzzer.setIcon("mdi:bell-ring");
    // buzzer.getCurrentState();

    // dualout.onCommand(DualOutput);
    // dualout.setName("Dual Output");
    // dualout.setIcon("mdi:power-socket-au");
    // dualout.getCurrentState();

    // overloadres.onCommand(OverloadAutoRestarte);
    // overloadres.setName("Overload Auto Restart");
    // overloadres.setIcon("mdi:transmission-tower-off");
    // overloadres.getCurrentState();

    // transfer.onCommand(TransferToBypassOverload);
    // transfer.setName("Transfer To Bypass Overload");
    // transfer.setIcon("mdi:transmission-tower-import");
    // transfer.getCurrentState();

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Selection AC Input Rang
    // InputRang.setOptions("appliance;ups");  // use semicolons as separator of options
    // InputRang.onCommand(ACInputRang);
    // InputRang.setIcon("mdi:home");
    // InputRang.setName("AC Input Rang");

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Selection Output Source Priority
    // PrioritySelect.setOptions("utilty;solar;sbu"); // use semicolons as separator of options
    // PrioritySelect.onCommand(OutputSourcePriority);
    // PrioritySelect.setIcon("mdi:solar-power");
    // PrioritySelect.setName("Output Source Priority");

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Selection Output Voltage
    // OutputSelect.setOptions("220;230;240");  // use semicolons as separator of options
    // OutputSelect.onCommand(Voltageage);
    // OutputSelect.setIcon("mdi:flash-triangle");        // optional
    // OutputSelect.setName("Output Voltage");  // optional

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Name sensor device state
    OutputVolt.setName("Output Voltage");
    OutputVolt.setIcon("mdi:home");
    OutputVolt.setUnitOfMeasurement("V");

    OutputCurrent.setName("Output Current");
    OutputCurrent.setIcon("mdi:home");
    OutputCurrent.setUnitOfMeasurement("A");

    OutputFrequency.setName("Output Freq");
    OutputFrequency.setIcon("mdi:home");
    OutputFrequency.setUnitOfMeasurement("Hz");

    ApparentPower.setName("Apparent Power");
    ApparentPower.setIcon("mdi:transmission-tower");
    ApparentPower.setUnitOfMeasurement("VA");

    ActivePower.setName("Active Power");
    ActivePower.setIcon("mdi:transmission-tower");
    ActivePower.setUnitOfMeasurement("W");

    LoadPercent.setName("Load");
    LoadPercent.setIcon("mdi:transmission-tower");
    LoadPercent.setUnitOfMeasurement("%");

    BusVoltage.setName("Bus Voltage");
    BusVoltage.setIcon("mdi:flash-triangle");
    BusVoltage.setUnitOfMeasurement("V");

    Temp.setName("Temp");
    Temp.setIcon("mdi:temperature-celsius");
    Temp.setUnitOfMeasurement("°C");

    pvCurrent.setName("PV Current");
    pvCurrent.setIcon("mdi:current-dc");
    pvCurrent.setUnitOfMeasurement("A");

    pvVoltage.setName("PV Voltage");
    pvVoltage.setIcon("mdi:flash-triangle");
    pvVoltage.setUnitOfMeasurement("V");

    pvPower.setName("PV Power");
    pvPower.setIcon("mdi:transmission-tower-import");
    pvPower.setUnitOfMeasurement("W");

    // Converse Port char to uint_16
    uint16_t PORT = atoi(MQTT_PORT);

    // IP converse
    byte octets[4];
    if (sscanf(MQTT_ADDR, "%d.%d.%d.%d", &octets[0], &octets[1], &octets[2], &octets[3]) == 4)
    {
        IPAddress ipAddress(octets[0], octets[1], octets[2], octets[3]);
        Serial.print("Parsed IP address: ");
        Serial.println(ipAddress);
    }
    IPAddress MQTT_IP(octets[0], octets[1], octets[2], octets[3]);

    mqtt.begin(MQTT_IP, PORT, MQTT_USERNAME, MQTT_PASSWORD);

    // Serial.printf("MQTT_IP: %s\n", MQTT_IP);
    // Serial.printf("PORT: %s\n", PORT);
    // Serial.printf("MQTT_USERNAME: %s\n", MQTT_USERNAME);
    // Serial.printf("MQTT_PASSWORD: %s\n", MQTT_PASSWORD);

    Serial.println("Home assistnt begin: ");
    delay(1000);
}

//////////////////////////////////////////////////////////////////////////////////////
// sent switch command to inverter

void GridTie(bool state, HASwitch *sender)
{
    digitalWrite(LED, (state ? HIGH : LOW));
    sender->setState(state); // report state back to the Home Assistant
    if (state)
    {
        inv.valueToinv("GridTieOperation", 1);
    }
    if (!state)
    {
        inv.valueToinv("GridTieOperation", 0);
    }
}

// void BeepsInterrupt(bool state, HASwitch *sender)
// {
//     sender->setState(state); // report state back to the Home Assistant
//     if (state)
//     {
//         inv.cmd_inv("beeps on");
//     }
//     if (!state)
//     {
//         inv.cmd_inv("beeps off");
//     }
// }

// void Blacklight(bool state, HASwitch* sender) {
//   sender->setState(state);  // report state back to the Home Assistant
//   if (state) {
//     inv.cmd_inv("blacklight on");
//   }
//   if (!state) {
//     inv.cmd_inv("blacklight off");
//   }
// }

// void ReturnToPage(bool state, HASwitch *sender)
// {
//     sender->setState(state); // report state back to the Home Assistant
//     if (state)
//     {
//         inv.cmd_inv("returnpage on");
//     }
//     if (!state)
//     {
//         inv.cmd_inv("returnpage off");
//     }
// }

// void PowerSavingMode(bool state, HASwitch* sender) {
//   sender->setState(state);  // report state back to the Home Assistant
//   if (state) {
//     inv.cmd_inv("saving on");
//   }
//   if (!state) {
//     inv.cmd_inv("saving off");
//   }
// }

// void OverTempAutoRestart(bool state, HASwitch *sender)
// {
//     sender->setState(state); // report state back to the Home Assistant
//     if (state)
//     {
//         inv.cmd_inv("overtemp on");
//     }
//     if (!state)
//     {
//         inv.cmd_inv("overtemp off");
//     }
// }

// void LedPatternlight(bool state, HASwitch* sender) {
//   sender->setState(state);  // report state back to the Home Assistant
//   if (state) {
//     inv.cmd_inv("ledpattern on");
//   }
//   if (!state) {
//     inv.cmd_inv("ledpattern off");
//   }
// }

// void Buzzer(bool state, HASwitch *sender)
// {
//     sender->setState(state); // report state back to the Home Assistant
//     if (state)
//     {
//         inv.cmd_inv("buzzer on");
//     }
//     if (!state)
//     {
//         inv.cmd_inv("buzzer off");
//     }
// }

// void DualOutput(bool state, HASwitch *sender)
// {
//     sender->setState(state); // report state back to the Home Assistant
//     if (state)
//     {
//         inv.cmd_inv("dualoutput on");
//     }
//     if (!state)
//     {
//         inv.cmd_inv("dualoutput off");
//     }
// }

// void OverloadAutoRestarte(bool state, HASwitch *sender)
// {
//     sender->setState(state); // report state back to the Home Assistant
//     if (state)
//     {
//         inv.cmd_inv("overload on");
//     }
//     if (!state)
//     {
//         inv.cmd_inv("overload off");
//     }
// }

// void TransferToBypassOverload(bool state, HASwitch *sender)
// {
//     sender->setState(state); // report state back to the Home Assistant
//     if (state)
//     {
//         inv.cmd_inv("bypass on");
//     }
//     if (!state)
//     {
//         inv.cmd_inv("bypass off");
//     }
// }

// void ACInputRang(int8_t index, HASelect *sender)
// {
//     switch (index)
//     {
//     case 0:
//         inv.cmd_inv("appliance");
//         break;
//     case 1:
//         inv.cmd_inv("ups");
//         break;
//     default:
//         return;
//     }

//     sender->setState(index); // report the selected option back to the HA panel
//     // it may return null
//     if (sender->getCurrentOption())
//     {
//         Serial.print("Current option: ");
//         Serial.println(sender->getCurrentOption());
//     }
// }

// void OutputSourcePriority(int8_t index, HASelect *sender)
// {
//     switch (index)
//     {
//     case 0:
//         inv.cmd_inv("utility");
//         break;
//     case 1:
//         inv.cmd_inv("solar");
//         break;
//     case 2:
//         inv.cmd_inv("sbu");
//         break;
//     default:
//         return;
//     }
//     sender->setState(index); // report the selected option back to the HA panel
//     // it may return null
//     if (sender->getCurrentOption())
//     {
//         Serial.print("Current option: ");
//         Serial.println(sender->getCurrentOption());
//     }
// }

// void Voltageage(int8_t index, HASelect* sender) {
//   switch (index) {
//     case 0:
//       inv.cmd_inv("output 220");
//       break;
//     case 1:
//       inv.cmd_inv("output 230");
//       break;
//     case 2:
//       inv.cmd_inv("output 240");
//       break;
//     default:
//       return;
//   }
//   sender->setState(index);  // report the selected option back to the HA panel
//   // it may return null
//   if (sender->getCurrentOption()) {
//     Serial.print("Current option: ");
//     Serial.println(sender->getCurrentOption());
//   }
// }
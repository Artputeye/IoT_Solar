#include "iotHA.h"

uint8_t Mac[6];

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Switch object define
HASwitch grid("grid");
HASwitch beeps("beeps");
// HASwitch lcdblacklight("lcdblacklight");
HASwitch returnlcd("returnlcd");
// HASwitch powersaveing("powersaveing");
HASwitch overtemp("overtemp");
// HASwitch ledlight("ledlight");
HASwitch buzzer("buzzer");
HASwitch dualout("dualout");
HASwitch overloadres("overloadres");
HASwitch transfer("transfer");

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Selection object define
HASelect InputRang("ACInputRang");

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Selection object define
HASelect PrioritySelect("PrioritySelect");

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Selection object define
// HASelect OutputSelect("OutputSelect");

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sensor object define
// HASensorNumber GrindVolt("GrindVolt", HASensorNumber::PrecisionP1);
// HASensorNumber GrindFrequency("GrindFrequency", HASensorNumber::PrecisionP1);
HASensorNumber Voltage("Voltage", HASensorNumber::PrecisionP1);
HASensorNumber Frequency("Frequency", HASensorNumber::PrecisionP1);
HASensorNumber ApparentPower("ApparentPower");
HASensorNumber ActivePower("ActivePower");
HASensorNumber LoadPercent("LoadPercent");
HASensorNumber BusVoltage("BusVoltage");
HASensorNumber Temp("BusVoltage");
HASensorNumber pvCurrent("pvCurrent", HASensorNumber::PrecisionP1);
HASensorNumber pvVoltage("pvVoltage", HASensorNumber::PrecisionP1);
HASensorNumber pvPower("pvPower", HASensorNumber::PrecisionP1);

void GridTie(bool state, HASwitch *sender);
void BeepsInterrupt(bool state, HASwitch *sender);
void BeepsInterrupt(bool state, HASwitch *sender);
void ReturnToPage(bool state, HASwitch *sender);
void OverTempAutoRestart(bool state, HASwitch *sender);
void Buzzer(bool state, HASwitch *sender);
void DualOutput(bool state, HASwitch *sender);
void OverloadAutoRestarte(bool state, HASwitch *sender);
void TransferToBypassOverload(bool state, HASwitch *sender);
void ACInputRang(int8_t index, HASelect *sender);
void OutputSourcePriority(int8_t index, HASelect *sender);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MacSetup()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin();
    Serial.println(" ");
    Serial.print("MAC Address: ");
    readMacAddress();
    WiFi.macAddress(Mac);
    device.setUniqueId(Mac, sizeof(Mac));
}

void iotHAsetup()
{
    /////////////////////////////////////////////////////////////////////////////
    // set device's details (optional)
    device.setName(DEVICE_NAME);
    device.setSoftwareVersion("1.0.0");
    device.setManufacturer("ARTTECH");
    device.setModel("IoT Solar");

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Switch device state
    grid.onCommand(GridTie);
    grid.setName("Grid Tie");
    grid.setIcon("mdi:transmission-tower-export");
    grid.getCurrentState();

    beeps.onCommand(BeepsInterrupt);
    beeps.setName("Beeps While Primary Suorce Interrupt");
    beeps.setIcon("mdi:bell-ring");
    beeps.getCurrentState();

    // lcdblacklight.onCommand(Blacklight);
    // lcdblacklight.setName("LCD Blacklight");
    // lcdblacklight.setIcon("mdi:monitor-shimmer");
    // lcdblacklight.getCurrentState();

    returnlcd.onCommand(ReturnToPage);
    returnlcd.setName("Return To The Main LCD Page");
    returnlcd.setIcon("mdi:home");
    returnlcd.getCurrentState();

    // powersaving.onCommand(PowerSavingMode);
    // powersaving.setName("Power Saving Mode");
    // powersaving.setIcon("mdi:power-standby");
    // powersaving.getCurrentState();

    overtemp.onCommand(OverTempAutoRestart);
    overtemp.setName("Over Temp Auto Restart");
    overtemp.setIcon("mdi:thermometer-alert");
    overtemp.getCurrentState();

    // ledlight.onCommand(LedPatternlight);
    // ledlight.setName("Led Pattern Light");
    // ledlight.setIcon("mdi:home");
    // ledlight.getCurrentState();

    buzzer.onCommand(Buzzer);
    buzzer.setName("Buzzer");
    buzzer.setIcon("mdi:bell-ring");
    buzzer.getCurrentState();

    dualout.onCommand(DualOutput);
    dualout.setName("Dual Output");
    dualout.setIcon("mdi:power-socket-au");
    dualout.getCurrentState();

    overloadres.onCommand(OverloadAutoRestarte);
    overloadres.setName("Overload Auto Restart");
    overloadres.setIcon("mdi:transmission-tower-off");
    overloadres.getCurrentState();

    transfer.onCommand(TransferToBypassOverload);
    transfer.setName("Transfer To Bypass Overload");
    transfer.setIcon("mdi:transmission-tower-import");
    transfer.getCurrentState();

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Selection AC Input Rang
    // InputRang.setOptions("appliance;ups");  // use semicolons as separator of options
    // InputRang.onCommand(ACInputRang);
    // InputRang.setIcon("mdi:home");
    // InputRang.setName("AC Input Rang");

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Selection Output Source Priority
    PrioritySelect.setOptions("utilty;solar;sbu"); // use semicolons as separator of options
    PrioritySelect.onCommand(OutputSourcePriority);
    PrioritySelect.setIcon("mdi:solar-power");
    PrioritySelect.setName("Output Source Priority");

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Selection Output Voltage
    // OutputSelect.setOptions("220;230;240");  // use semicolons as separator of options
    // OutputSelect.onCommand(Voltageage);
    // OutputSelect.setIcon("mdi:flash-triangle");        // optional
    // OutputSelect.setName("Output Voltage");  // optional

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Name sensor device state
    // GridVolt.setName("Grid Volt");
    // GridVolt.setIcon("mdi:home");
    // GridVolt.setUnitOfMeasurement("V");

    // GridFrequency.setName("Grid Freq");
    // GridFrequency.setIcon("mdi:home");
    // GridFrequency.setUnitOfMeasurement("Hz");

    Voltage.setName("Voltage");
    Voltage.setIcon("mdi:flash-triangle");
    Voltage.setUnitOfMeasurement("V");

    Frequency.setName("Frequency");
    Frequency.setIcon("mdi:sine-wave");
    Frequency.setUnitOfMeasurement("Hz");

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
}

void readMacAddress()
{
    esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, Mac);
    if (ret == ESP_OK)
    {
        Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                      Mac[0], Mac[1], Mac[2],
                      Mac[3], Mac[4], Mac[5]);
    }
    else
    {
        Serial.println("Failed to read MAC address");
    }
}

void iotHA()
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
        float per = (i / 10);
        float pvc = 7.2 + (float(i) / 100);
        float pvv = 335.7 + (float(i) / 10) + (float(i) / 100);

        // GridVolt.setValue(volt);
        // GridFrequency.setValue(fre);
        Voltage.setValue(volt);
        Frequency.setValue(fre);
        ApparentPower.setValue(appar);
        ActivePower.setValue(active);
        LoadPercent.setValue(per);
        BusVoltage.setValue(vbus);
        Temp.setValue(t);
        pvCurrent.setValue(pvc);
        pvVoltage.setValue(pvv);
        float power = pvc * pvv;

        Serial.println("V:" + String(volt, 2));
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    // sent inverter data to MQTT broker
    if (!inv.test)
    {
        float power = 123.1;
        // Serial.println("Test OFF");
        //  GridVolt.setValue(inv.data.gridVoltage);
        //  GridFrequency.setValue(inv.data.gridFrequency);
        Voltage.setValue(inv.data.outputVoltage);
        Frequency.setValue(inv.data.outputFrequency);
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

//////////////////////////////////////////////////////////////////////////////////////
// sent switch command to inverter

void GridTie(bool state, HASwitch *sender)
{
    digitalWrite(LED, (state ? HIGH : LOW));
    sender->setState(state); // report state back to the Home Assistant
    if (state)
    {
        inv.cmd_inv("grid on");
    }
    if (!state)
    {
        inv.cmd_inv("grid off");
    }
}

void BeepsInterrupt(bool state, HASwitch *sender)
{
    sender->setState(state); // report state back to the Home Assistant
    if (state)
    {
        inv.cmd_inv("beeps on");
    }
    if (!state)
    {
        inv.cmd_inv("beeps off");
    }
}

// void Blacklight(bool state, HASwitch* sender) {
//   sender->setState(state);  // report state back to the Home Assistant
//   if (state) {
//     inv.cmd_inv("blacklight on");
//   }
//   if (!state) {
//     inv.cmd_inv("blacklight off");
//   }
// }

void ReturnToPage(bool state, HASwitch *sender)
{
    sender->setState(state); // report state back to the Home Assistant
    if (state)
    {
        inv.cmd_inv("returnpage on");
    }
    if (!state)
    {
        inv.cmd_inv("returnpage off");
    }
}

// void PowerSavingMode(bool state, HASwitch* sender) {
//   sender->setState(state);  // report state back to the Home Assistant
//   if (state) {
//     inv.cmd_inv("saving on");
//   }
//   if (!state) {
//     inv.cmd_inv("saving off");
//   }
// }

void OverTempAutoRestart(bool state, HASwitch *sender)
{
    sender->setState(state); // report state back to the Home Assistant
    if (state)
    {
        inv.cmd_inv("overtemp on");
    }
    if (!state)
    {
        inv.cmd_inv("overtemp off");
    }
}

// void LedPatternlight(bool state, HASwitch* sender) {
//   sender->setState(state);  // report state back to the Home Assistant
//   if (state) {
//     inv.cmd_inv("ledpattern on");
//   }
//   if (!state) {
//     inv.cmd_inv("ledpattern off");
//   }
// }

void Buzzer(bool state, HASwitch *sender)
{
    sender->setState(state); // report state back to the Home Assistant
    if (state)
    {
        inv.cmd_inv("buzzer on");
    }
    if (!state)
    {
        inv.cmd_inv("buzzer off");
    }
}

void DualOutput(bool state, HASwitch *sender)
{
    sender->setState(state); // report state back to the Home Assistant
    if (state)
    {
        inv.cmd_inv("dualoutput on");
    }
    if (!state)
    {
        inv.cmd_inv("dualoutput off");
    }
}

void OverloadAutoRestarte(bool state, HASwitch *sender)
{
    sender->setState(state); // report state back to the Home Assistant
    if (state)
    {
        inv.cmd_inv("overload on");
    }
    if (!state)
    {
        inv.cmd_inv("overload off");
    }
}

void TransferToBypassOverload(bool state, HASwitch *sender)
{
    sender->setState(state); // report state back to the Home Assistant
    if (state)
    {
        inv.cmd_inv("bypass on");
    }
    if (!state)
    {
        inv.cmd_inv("bypass off");
    }
}

void ACInputRang(int8_t index, HASelect *sender)
{
    switch (index)
    {
    case 0:
        inv.cmd_inv("appliance");
        break;
    case 1:
        inv.cmd_inv("ups");
        break;
    default:
        return;
    }

    sender->setState(index); // report the selected option back to the HA panel
    // it may return null
    if (sender->getCurrentOption())
    {
        Serial.print("Current option: ");
        Serial.println(sender->getCurrentOption());
    }
}

void OutputSourcePriority(int8_t index, HASelect *sender)
{
    switch (index)
    {
    case 0:
        inv.cmd_inv("utility");
        break;
    case 1:
        inv.cmd_inv("solar");
        break;
    case 2:
        inv.cmd_inv("sbu");
        break;
    default:
        return;
    }
    sender->setState(index); // report the selected option back to the HA panel
    // it may return null
    if (sender->getCurrentOption())
    {
        Serial.print("Current option: ");
        Serial.println(sender->getCurrentOption());
    }
}

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
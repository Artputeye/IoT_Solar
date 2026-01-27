// iotHA.cpp
#include "iotHA.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Diagnostic object define
HASensor ipAddress("ip_address");
HASensor macAddress("mac_address");
HASensor uptime("uptime");
HASensor rssi("rssi");

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Switch object define
HASwitch grid("grid");

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sensor object define
HASensorNumber LoadPercent("LoadPercent");
HASensorNumber EnergyDaily("EnergyDaily", HASensorNumber::PrecisionP3);
HASensorNumber GridPower("GridPower");
HASensorNumber ActivePower("ActivePower");
HASensorNumber ApparentPower("ApparentPower");
HASensorNumber OutputVolt("OutputVolt", HASensorNumber::PrecisionP1);
HASensorNumber OutputCurrent("OutputCurrent", HASensorNumber::PrecisionP1);
HASensorNumber OutputFrequency("OutputFrequency", HASensorNumber::PrecisionP1);
HASensorNumber PowerFactor("PowerFactor", HASensorNumber::PrecisionP2);
HASensorNumber pvPower("pvPower", HASensorNumber::PrecisionP1);
HASensorNumber pvCurrent("pvCurrent", HASensorNumber::PrecisionP1);
HASensorNumber pvVoltage("pvVoltage", HASensorNumber::PrecisionP1);
HASensorNumber BusVoltage("BusVoltage");
HASensorNumber BattVoltage("BattVoltage", HASensorNumber::PrecisionP1);
HASensorNumber Temp("Temp");

////////////////////////////////////////////////////////////////////////////////////////
// sent inverter data to MQTT broker
void HA_Diagnostic()
{
    ipAddress.setValue(WiFi.localIP().toString().c_str());
    macAddress.setValue(MacAddr);
    uptime.setValue("10230");
    rssi.setValue("85");
}

void iotHArun()
{
    if (!inv.test)
    {
        /////////////////////////////////////////////////////
        float power;
        LoadPercent.setValue(inv.data.loadPercent);
        EnergyDaily.setValue(energy_kWh);
        GridPower.setValue(gridPower);
        ActivePower.setValue(inv.data.ActivePower);
        ApparentPower.setValue(inv.data.ApparentPower);
        OutputVolt.setValue(inv.data.outputVoltage);
        OutputCurrent.setValue(inv.data.outputCurrent);
        OutputFrequency.setValue(inv.data.outputFrequency);
        PowerFactor.setValue(inv.data.powerFactor);
        pvPower.setValue(inv.data.pvPower);
        pvCurrent.setValue(inv.data.pvCurrent);
        pvVoltage.setValue(inv.data.pvVoltage);
        BusVoltage.setValue(inv.data.busVoltage);
        BattVoltage.setValue(inv.data.batteryVoltage);
        Temp.setValue(inv.data.temp);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Home Assistant setup
void iotHAsetup()
{
    Serial.println("Home assistnt setup: ");

    // set device's details (optional)
    device.setName(DEVICE_NAME);
    device.setSoftwareVersion(D_SoftwareVertion);
    device.setManufacturer(D_Mfac);
    device.setModel(D_Model);
    
    /////////////////////////////////////////////////////////////////////////////
    // Diagnostic info

    ipAddress.setName("IP Address");
    ipAddress.setIcon("mdi:ip-network");
    ipAddress.setStateClass("Diagnostic");

    macAddress.setName("MAC Address");
    macAddress.setIcon("mdi:lan-connect");
    macAddress.setStateClass("Diagnostic");

    uptime.setName("Uptime");
    uptime.setIcon("mdi:timer-sand");
    uptime.setUnitOfMeasurement("seconds");
    uptime.setStateClass("Diagnostic");

    rssi.setName("WiFi RSSI");
    rssi.setIcon("mdi:wifi-strength-4");
    rssi.setUnitOfMeasurement("dBm");
    rssi.setStateClass("Diagnostic");

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Switch device state
    grid.onCommand(GridTie);
    grid.setName("Grid Tie");
    grid.setIcon("mdi:transmission-tower-export");
    grid.getCurrentState();

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Name sensor device state
    LoadPercent.setName("Load Percent");
    LoadPercent.setIcon("mdi:ticket-percent");
    LoadPercent.setUnitOfMeasurement("%");

    EnergyDaily.setName("Energy Daily");
    EnergyDaily.setIcon("mdi:meter-electric");
    EnergyDaily.setUnitOfMeasurement("kWh");

    GridPower.setName("Grid Power");
    GridPower.setIcon("mdi:transmission-tower");
    GridPower.setUnitOfMeasurement("W");

    ActivePower.setName("Active Power");
    ActivePower.setIcon("mdi:transmission-tower");
    ActivePower.setUnitOfMeasurement("W");

    ApparentPower.setName("Apparent Power");
    ApparentPower.setIcon("mdi:transmission-tower");
    ApparentPower.setUnitOfMeasurement("VA");

    OutputVolt.setName("Output Voltage");
    OutputVolt.setIcon("mdi:flash-triangle");
    OutputVolt.setUnitOfMeasurement("V");

    OutputCurrent.setName("Output Current");
    OutputCurrent.setIcon("mdi:current-ac");
    OutputCurrent.setUnitOfMeasurement("A");

    OutputFrequency.setName("Output Frequency");
    OutputFrequency.setIcon("mdi:sine-wave");
    OutputFrequency.setUnitOfMeasurement("Hz");

    PowerFactor.setName("Power Factor");
    PowerFactor.setIcon("mdi:angle-acute");
    //PowerFactor.setUnitOfMeasurement("cosθ");

    pvPower.setName("PV Power");
    pvPower.setIcon("mdi:transmission-tower-import");
    pvPower.setUnitOfMeasurement("W");

    pvCurrent.setName("PV Current");
    pvCurrent.setIcon("mdi:current-dc");
    pvCurrent.setUnitOfMeasurement("A");

    pvVoltage.setName("PV Voltage");
    pvVoltage.setIcon("mdi:flash-triangle");
    pvVoltage.setUnitOfMeasurement("V");

    BusVoltage.setName("Bus Voltage");
    BusVoltage.setIcon("mdi:flash-triangle");
    BusVoltage.setUnitOfMeasurement("V");

    BattVoltage.setName("Battery Voltage");
    BattVoltage.setIcon("mdi:flash-triangle");
    BattVoltage.setUnitOfMeasurement("V");

    Temp.setName("Temperature");
    Temp.setIcon("mdi:thermometer");
    Temp.setUnitOfMeasurement("°C");

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

#include "websocket.h"

AsyncWebSocket ws("/ws");
unsigned long lastTimeMonitor = 0;
unsigned long lastPingTime = 0;
unsigned long MonitorDelay = 3000;
const unsigned long pingInterval = 25000; // 25 วินาที

String wsAllDataBase64()
{
    JsonDocument doc;

    ///////////////////////Serial Sent////////////////////////////
    doc["Serial"] = wsSerial;
    doc["Inverter"] = wsInverter;

    ///////////////////////Monotor////////////////////////////////
    doc["Apparent Power"] = inv.data.ApparentPower;
    doc["Active Power"] = inv.data.ActivePower;
    doc["PV Power"] = inv.data.pvCurrent * inv.data.pvVoltage;
    doc["Load Percent"] = inv.data.loadPercent;
    doc["PV Current"] = inv.data.pvCurrent;
    doc["PV Voltage"] = inv.data.pvVoltage;
    doc["Grid Voltage"] = inv.data.gridVoltage;
    doc["Output Voltage"] = inv.data.outputVoltage;
    doc["Bus Voltage"] = inv.data.busVoltage;
    doc["Battery Voltage"] = inv.data.batteryVoltage;
    doc["Grid Frequency"] = inv.data.gridFrequency;
    doc["Output Frequency"] = inv.data.outputFrequency;
    doc["Temperature"] = inv.data.temp;
    doc["Inverter Status"] = inv.data.InverterStatus;

    //////////////////////////Status//////////////////////////////
    doc["Grid Rating Voltage"] = inv.rated.GridRatingVoltage;
    doc["Grid Rating Current"] = inv.rated.GridRatingCurrent;
    doc["Output Rating Voltage"] = inv.rated.OutputRatingVoltage;
    doc["Output Rating Frequency"] = inv.rated.OutputRatingFrequency;
    doc["Output Rating Current"] = inv.rated.OutputRatingCurrent;
    doc["Output Rating Apparent Power"] = inv.rated.OutputRatingApparentPower;
    doc["Output Rating Active Power"] = inv.rated.OutputRatingActivePower;
    doc["Battery Rating Voltage"] = inv.rated.BatteryRatingVoltage;
    doc["Battery Re-charge Voltage"] = inv.rated.BatteryReChargeVoltage;
    doc["Battery Under Voltage"] = inv.rated.BatteryUnderVoltage;
    doc["Battery Bulk Voltage"] = inv.rated.BatteryBulkVoltage;
    doc["Battery Float Voltage"] = inv.rated.BatteryFloatVoltage;
    doc["Battery Type"] = inv.rated.BatteryType;
    doc["Max AC Charging Current"] = inv.rated.MaxAC_ChargingCurrent;
    doc["Max Charging Current"] = inv.rated.MaxChargingCurrent;
    doc["Input Voltage Range"] = inv.rated.InputVoltageRange;
    doc["Output Source Priority"] = inv.rated.OutputSourcePriority;
    doc["Charger Source Priority"] = inv.rated.ChargerSourcePriority;
    doc["Parallel Max Num"] = inv.rated.ParallelMaxNum;
    doc["Machine Type"] = inv.rated.MachineType;
    doc["Topology"] = inv.rated.Topology;
    doc["Output Mode"] = inv.rated.OutputMode;
    doc["Battery Re-discharge Voltage"] = inv.rated.BatteryReDischargeVoltage;
    doc["PV Parallel"] = inv.rated.PV_Parallel;
    doc["PV Balance"] = inv.rated.PV_Balance;
    doc["Max Charging Time"] = inv.rated.MaxChargingTime;
    doc["Operation Logic"] = inv.rated.OperationLogic;
    doc["Max Discharging Current"] = inv.rated.MaxDischargingCurrent;
    /////////////////////////////////Alarm////////////////////////////////////
    doc["Inverter Faults"] = inv.faultList;

    /////////////////////////////////////////////////////////////////////////
    String jsonOut; // serialize JSON
    serializeJson(doc, jsonOut);
    return base64::encode(jsonOut); // encode Base64
}

void wsClear()
{
    JsonDocument doc;
    ///////////////////////Monitor////////////////////////////////
    doc["Apparent Power"] = "";
    doc["Active Power"] = "";
    doc["PV Power"] = "";
    doc["Load Percent"] = "";
    doc["PV Current"] = "";
    doc["PV Voltage"] = "";
    doc["Grid Voltage"] = "";
    doc["Output Voltage"] = "";
    doc["Bus Voltage"] = "";
    doc["Battery Voltage"] = "";
    doc["Grid Frequency"] = "";
    doc["Output Frequency"] = "";
    doc["Temperature"] = "";
    doc["Inverter Status"] = "";

    //////////////////////////Status//////////////////////////////
    doc["Grid Rating Voltage"] = "";
    doc["Grid Current"] = "";
    doc["AC Output Rating Voltage"] = "";
    doc["AC Output Rating Frequency"] = "";
    doc["AC Output Rating Current"] = "";
    doc["AC Output Rating Apparent Power"] = "";
    doc["AC Output Rating Active Power"] = "";
    doc["Battery Rating Voltage"] = "";
    doc["Battery Re-charge Voltage"] = "";
    doc["Battery Under Voltage"] = "";
    doc["Battery Bulk Voltage"] = "";
    doc["Battery Float Voltage"] = "";
    doc["Battery Type"] = "";
    doc["Max AC Charging Current"] = "";
    doc["Max Charging Current"] = "";
    doc["Input Voltage Range"] = "";
    doc["Output Source Priority"] = "";
    doc["Charger Source Priority"] = "";
    doc["Parallel Max Num"] = "";
    doc["Machine Type"] = "";
    doc["Topology"] = "";
    doc["Output Mode"] = "";
    doc["Battery Re-discharge Voltage"] = "";
    doc["PV OK Condition for Parallel"] = "";
    doc["PV OK Condition for PV Power Balance"] = "";
    doc["Max Charging Time"] = "";
    doc["Operation Logic"] = "";
    doc["Max Discharging Current"] = "";

    String jsonOut; // serialize JSON
    serializeJson(doc, jsonOut);
}

void notifyClients(const String &base64Msg)
{
    ws.textAll(base64Msg); // ส่ง Text Frame
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len &&
        (info->opcode == WS_TEXT || info->opcode == WS_BINARY))
    {
        notifyClients(wsAllDataBase64());
    }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n",
                      client->id(), client->remoteIP().toString().c_str());
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
    case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
    }
}

void initWebSocket()
{
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}

void wsJsonSerial(const String &msg)
{
    wsSerial = msg;
    if (!wsSerial.isEmpty())
    {
        notifyClients(wsAllDataBase64());
        wsSerial = "";
        inv.serialData = "";
        delay(100);
    }
}

void wsJsonInverter(const String &msg)
{
    wsInverter = msg;
    if (!wsInverter.isEmpty())
    {
        notifyClients(wsAllDataBase64());
        wsInverter = "";
        inv.invData = "";
        delay(100);
    }
}

/////////////////////////////////////////////////////////////////////////////////////
void wsloop()
{
    if (millis() - lastTimeMonitor > MonitorDelay)
    {
        lastTimeMonitor = millis();
        notifyClients(wsAllDataBase64());
        //wsClear();
    }

    if (millis() - lastPingTime > pingInterval)
    {
        lastPingTime = millis();
        ws.pingAll();
    }
    ws.cleanupClients();
}

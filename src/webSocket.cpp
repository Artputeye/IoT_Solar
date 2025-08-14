#include "websocket.h"

AsyncWebSocket ws("/ws");
unsigned long lastTimeMonitor = 0;
unsigned long MonitorDelay = 3000;

String cleanString(String s)
{
    String result = "";
    for (unsigned int i = 0; i < s.length(); i++)
    {
        char c = s[i];
        if (c >= 32 && c != 127)
        { // เอาเฉพาะ ASCII printable
            result += c;
        }
    }
    return result;
}

String wsAllDataBase64()
{
    invStr = cleanString(invStr);
    inputStr = cleanString(inputStr);

    JsonDocument doc;

    ///////////////////////Serial Sent////////////////////////////
    doc["Inverter Data"] = invStr.c_str();
    ;
    doc["Command Data"] = inputStr.c_str();
    ;

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
    doc["Grid Rating Voltage"] = 0;
    doc["Grid Current"] = 0;
    doc["AC Output Rating Voltage"] = 0;
    doc["AC Output Rating Frequency"] = 0;
    doc["AC Output Rating Current"] = 0;
    doc["AC Output Rating Apparent Power"] = 0;
    doc["AC Output Rating Active Power"] = 0;
    doc["Battery Rating Voltage"] = 0;
    doc["Battery Re-charge Voltage"] = 0;
    doc["Battery Under Voltage"] = 0;
    doc["Battery Bulk Voltage"] = 0;
    doc["Battery Float Voltage"] = 0;
    doc["Battery Type"] = 0;
    doc["Max AC Charging Current"] = 0;
    doc["Max Charging Current"] = 0;
    doc["Input Voltage Range"] = 0;
    doc["Output Source Priority"] = 0;
    doc["Charger Source Priority"] = 0;
    doc["Parallel Max Num"] = 0;
    doc["Machine Type"] = 0;
    doc["Topology"] = 0;
    doc["Output Mode"] = 0;
    doc["Battery Re-discharge Voltage"] = 0;
    doc["PV OK Condition for Parallel"] = 0;
    doc["PV OK Condition for PV Power Balance"] = 0;
    doc["Max Charging Time"] = 0;
    doc["Operation Logic"] = 0;
    doc["Max Discharging Current"] = 0;

    String jsonOut; // serialize JSON
    serializeJson(doc, jsonOut);
    return base64::encode(jsonOut); // encode Base64
}

void notifyClients(const String &base64Msg)
{
    ws.binaryAll((const uint8_t *)base64Msg.c_str(), base64Msg.length());
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len &&
        (info->opcode == WS_TEXT || info->opcode == WS_BINARY))
    {
        // ส่ง JSON เดียวรวมทุกอย่าง
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

void wsloop()
{
    if (!inputStr.isEmpty() || !invStr.isEmpty())
    {
        notifyClients(wsAllDataBase64());
        delay(100);
    }

    if (millis() - lastTimeMonitor > MonitorDelay)
    {
        lastTimeMonitor = millis();
        notifyClients(wsAllDataBase64());
    }

    ws.cleanupClients();
}

#include "websocket.h"

AsyncWebSocket ws("/ws");
String jsonString = "";
int i;
// Json Variable to Hold Sensor Readings

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 3000;

// Get Sensor Readings and return JSON object
String invMonitors()
{
    JsonDocument readings;
    readings["Apparent Power"] = inv.data.ApparentPower;
    readings["Active Power"] = inv.data.ActivePower;
    readings["PV Power"] = (inv.data.pvCurrent*inv.data.pvVoltage);
    readings["Load Percent"] = inv.data.loadPercent;
    readings["PV Current"] = inv.data.pvCurrent;
    readings["PV Voltage"] = inv.data.pvVoltage;
    readings["Grid Voltage"] = inv.data.gridVoltage;
    readings["Output Voltage"] = inv.data.outputVoltage;
    readings["Bus Voltage"] = inv.data.busVoltage;
    readings["Battery Voltage"] = inv.data.batteryVoltage;
    readings["Grid Frequency"] = inv.data.gridFrequency;
    readings["Output Frequency"] = inv.data.outputFrequency;
    readings["Temperature"] = inv.data.temp;
    readings["Inverter Status"] = inv.data.InverterStatus;

    serializeJsonPretty(readings, jsonString);
    return jsonString;
}

void notifyClients(String invReadings)
{
    ws.textAll(invReadings);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        // data[len] = 0;
        // String message = (char*)data;
        //  Check if the message is "getReadings"
        // if (strcmp((char*)data, "getReadings") == 0) {
        // if it is, send current sensor readings
        String invReadings = invMonitors();
        // Serial.print(invReadings);
        notifyClients(invReadings);
        //}
    }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
    case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
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
    if ((millis() - lastTime) > timerDelay)
    {
        String invReadings = invMonitors();
        if (inv.test)
        {
            Serial.print(invReadings);
        }
        notifyClients(invReadings);
        lastTime = millis();
    }
    ws.cleanupClients();
}
#ifndef WEB_SOCKET_H
#define WEB_SOCKET_H
#include "globals.h"

String invMonitors();
void notifyClients(String sensorReadings);
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void initWebSocket();
void wsloop();

#endif
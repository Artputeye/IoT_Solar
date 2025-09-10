#ifndef WEB_SOCKET_H
#define WEB_SOCKET_H
#include "globals.h"


String wsAllDataBase64();
void wsClear();
void notifyClients(const String &msg);
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void initWebSocket();
void wsJsonSerial(const String &msg);
void wsJsonInverter(const String &msg);
void wsloop();

#endif
#ifndef WEB_SOCKET_H
#define WEB_SOCKET_H
#include "globals.h"

String cleanString(String s);
String wsAllDataBase64();
void notifyClients(const String &msg);
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void initWebSocket();
void wsloop();



#endif
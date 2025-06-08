#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include "globals.h"
// ยกเลิก define เพื่อคืนค่าเดิมให้ ESPAsyncWebServer ใช้ enum ได้
#define HTTP_GET    _HTTP_GET
#define HTTP_POST   _HTTP_POST
#define HTTP_DELETE _HTTP_DELETE
#define HTTP_PUT    _HTTP_PUT
#define HTTP_HEAD   _HTTP_HEAD
#define HTTP_PATCH  _HTTP_PATCH
#define HTTP_OPTIONS _HTTP_OPTIONS
#include <WiFiManager.h>  //https://github.com/tzapu/WiFiManager

void wifi_para();
void saveConfigCallback();

#endif
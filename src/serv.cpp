#include "serv.h"
#ifdef ESP32
#include <LittleFS.h>
#endif

AsyncWebServer server(80);

void handlePostData(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  String body = "";
  for (size_t i = 0; i < len; i++) {
    body += (char)data[i];
  }
  Serial.println("POST client: " + body);
  request->send(200, "text/plain", "POST received: " + body);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String getContentType(String filename) {
  if (filename.endsWith(".html")) return "text/html";
  if (filename.endsWith(".css")) return "text/css";
  if (filename.endsWith(".js")) return "application/javascript";
  if (filename.endsWith(".png")) return "image/png";
  if (filename.endsWith(".jpg")) return "image/jpeg";
  if (filename.endsWith(".ico")) return "image/x-icon";
  if (filename.endsWith(".svg")) return "image/svg+xml";
  if (filename.endsWith(".json")) return "application/json";
  return "text/plain";
}

void setupServer() {
  // POST endpoint
  server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, handlePostData);

  // เสิร์ฟไฟล์ static จาก LittleFS
  server.onNotFound([](AsyncWebServerRequest *request) {
    String path = request->url();
    if (path == "/") path = "/index.html";
    String contentType = getContentType(path);
    
    if (LittleFS.exists(path)) {
      request->send(LittleFS, path, contentType);
    } else {
      request->send(404, "text/plain", "File Not Found");
    }
  });

  server.begin();
}
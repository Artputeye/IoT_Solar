#include "webHandle.h"
const char *PARAM_MESSAGE PROGMEM = "plain";
void clientCommand();
void staticRoot();
void notfoundRoot();

void setupServer()
{
  setupOTAUpload();
  clientCommand();
  staticRoot();
  notfoundRoot();
}

String getContentType(String filename)
{
  if (filename.endsWith(".html"))
    return "text/html";
  if (filename.endsWith(".css"))
    return "text/css";
  if (filename.endsWith(".js"))
    return "application/javascript";
  if (filename.endsWith(".png"))
    return "image/png";
  if (filename.endsWith(".jpg"))
    return "image/jpeg";
  if (filename.endsWith(".ico"))
    return "image/x-icon";
  if (filename.endsWith(".svg"))
    return "image/svg+xml";
  if (filename.endsWith(".json"))
    return "application/json";
  return "text/plain";
}

void clientCommand()
{
  server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    String message;
    if (request->hasParam(PARAM_MESSAGE, true)) {
      message = request->getParam(PARAM_MESSAGE, true)->value();
    } else {
      message = "No message sent";
      Serial.println("No message sent");
    }
    inv.cmd_inv(message);
    Serial.println("POST client: " + message);
    request->send(200, "text/plain", "POST: " + message); });
}

void staticRoot()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Request to '/': Serving /index.html");
    request->send(LittleFS, "/index.html", "text/html"); });

  server.on("/set", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Request to '/set': Serving /set.html");
    request->send(LittleFS, "/set.html", "text/html"); });

  server.on("/ota", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Request to '/ota': Serving /ota.html");
    request->send(LittleFS, "/ota.html", "text/html"); });

  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
  
}

void notfoundRoot()
{
  server.onNotFound([](AsyncWebServerRequest *request)
                    {
    String path = request->url();
    Serial.println("Check1: " + path);

    if (path == "/")
    {
      path = "/index.html";
    }

    else if (path == "/set" || path == "/ota" || path == "/filelist")
    {
      path += ".html";
    }

    else if (!(path.endsWith(".html") || path.endsWith(".css") || path.endsWith(".js") ||
               path.endsWith(".png") || path.endsWith(".jpg") || path.endsWith(".ico") ||
               path.endsWith(".svg") || path.endsWith(".json")))
    
               {
      // ตรวจสอบว่า path ไม่ได้ลงท้ายด้วย "/" เพื่อเพิ่ม "/" ก่อน index.html
        if (!path.endsWith("/")) {
          path += "/";
        }
        path += "index.html";
      }

      Serial.println("Check2: " + path);
      String contentType = getContentType(path);

      if (LittleFS.exists(path))
      {
        request->send(LittleFS, path, contentType);
      }
      else
      {
        request->send(404, "text/plain", "File Not Found\n\nPath: " + path);
      } });
}

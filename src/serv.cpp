#include "globals.h"
#include "serv.h"
const char *PARAM_MESSAGE PROGMEM = "plain";
void clientCommand();
void staticRoot();
void notfoundRoot();

void setupServer()
{
  clientCommand();
  staticRoot();
  notfoundRoot();
  server.begin();
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
  // 1. จัดการ Root URL ("/") ให้ส่ง index.html
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Request to '/': Serving /index.html");
    request->send(LittleFS, "/index.html", "text/html"); });

  // 2. จัดการ Path พิเศษที่ต้องการให้ลงท้ายด้วย .html
  server.on("/set", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Request to '/set': Serving /set.html");
    request->send(LittleFS, "/set.html", "text/html"); });

  server.on("/ota", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Request to '/ota': Serving /ota.html");
    request->send(LittleFS, "/ota.html", "text/html"); });

  // 3. ใช้ server.serveStatic() สำหรับไฟล์ Static ทั่วไป
  // This will handle all requests for files (e.g., /style.css, /script.js, /images/logo.png)
  // ที่อยู่ใน LittleFS. It also handles the default index.html if a folder is accessed.
  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html"); // สำคัญ: กำหนดให้ /index.html เป็นค่าเริ่มต้นเมื่อเข้าถึง "/" หรือ /folder/
}

void notfoundRoot()
{
server.onNotFound([](AsyncWebServerRequest *request) {
    String path = request->url();
    Serial.println("Check1: " + path);

    // ถ้า path คือ root "/" → ให้เปลี่ยนเป็น index.html
    if (path == "/") {
      path = "/index.html";
    }
    // ถ้าเป็น "/set" หรือ "/ota" (ไม่มีนามสกุล) ให้เพิ่ม ".html"
    else if (path == "/set" || path == "/ota") {
      path += ".html";
    }
    // ถ้า path ไม่มีนามสกุล และไม่ใช่ "/set" หรือ "/ota"
    // เช่น "/myfolder" หรือ "/anotherpath"
    // ให้ลองเพิ่ม "/index.html" ต่อท้าย (ถือว่าเป็นโฟลเดอร์)
    else if (!(path.endsWith(".html") || path.endsWith(".css") || path.endsWith(".js") ||
               path.endsWith(".png") || path.endsWith(".jpg") || path.endsWith(".ico") ||
               path.endsWith(".svg") || path.endsWith(".json"))) {
      
      // ตรวจสอบว่า path ไม่ได้ลงท้ายด้วย "/" เพื่อเพิ่ม "/" ก่อน index.html
      if (!path.endsWith("/")) {
        path += "/";
      }
      path += "index.html";
    }

    Serial.println("Check2: " + path);
    String contentType = getContentType(path);

    if (LittleFS.exists(path)) {
      request->send(LittleFS, path, contentType);
    } else {
      request->send(404, "text/plain", "File Not Found\n\nPath: " + path);
    }
  });

}


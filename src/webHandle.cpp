#include "webHandle.h"
const char *PARAM_MESSAGE PROGMEM = "plain";

void webHandle()
{
  setupOTAUpload();
  staticRoot();
  parameterSetting();
  getSetting();
  saveSetting();
  getbatSetting();
  savebatSetting();
  getNetwork();
  saveNetwork();
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

    else if (path == "/batt" || path == "/device" || path == "/filelist"|| path == "/info"|| path == "/monitor" || 
      path == "/ota"|| path == "/set"|| path == "/wifi"   )
    {
      path += ".html";
    }

    // else if (!(path.endsWith(".html") || path.endsWith(".css") || path.endsWith(".js") ||
    //            path.endsWith(".png") || path.endsWith(".jpg") || path.endsWith(".ico") ||
    //            path.endsWith(".svg") || path.endsWith(".json")))
    
    // //            {
      
    //     if (!path.endsWith("/")) {
    //       path += "/";
    //     }
    //     path += "index.html";
    //   }

    //   Serial.println("Check2: " + path);
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

void parameterSetting() // control route
{
  server.on("/setting", HTTP_POST, [](AsyncWebServerRequest *request)
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

////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// GENERAL SETTING //////////////////////////////////////

void getSetting() // API: ดึง JSON จาก littleFS แล้วส่ง Setting.json ไปยัง Client
{
  server.on("/getsetting", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (!LittleFS.exists("/setting.json")) {
      request->send(404, "application/json", "{\"error\":\"setting.json not found\"}");
      return;
    }

    File file = LittleFS.open("/setting.json", "r");
    if (!file) {
      request->send(500, "application/json", "{\"error\":\"Failed to open file\"}");
      return;
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
      request->send(500, "application/json", "{\"error\":\"Failed to parse JSON\"}");
      return;
    }

    String jsonResponse;
    serializeJson(doc, jsonResponse);
    Serial.println("/get setting : " + jsonResponse);

    request->send(200, "application/json", jsonResponse); });
}

void saveSetting() // API: รับ JSON จาก Client แล้วบันทึกไฟล์ Setting.json ไปยัง littleFS
{
  server.on("/savesetting", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
            {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, data, len);
    if (error) {
      Serial.println("JSON parse failed!");
      request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
      return;
    }
    // Debug: แสดงค่าที่ได้รับ
    for (JsonPair kv : doc.as<JsonObject>()) {
      String key = kv.key().c_str();
      String value = kv.value().as<String>();
      Serial.printf("Received setting: %s = %s\n", key.c_str(), value.c_str());
    }
    // เปิดไฟล์เพื่อเขียนทับ
    File file = LittleFS.open("/setting.json", "w");
    if (!file) {
      request->send(500, "application/json", "{\"error\":\"Failed to open file\"}");
      return;
    }
    // เขียน JSON ลงไฟล์
    if (serializeJson(doc, file) == 0) {
      request->send(500, "application/json", "{\"error\":\"Failed to write JSON\"}");
      file.close();
      return;
    }
    file.close();
    request->send(200, "application/json", "{\"status\":\"ok\"}"); });
    
    //ESP.restart();
}

////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// BATTERY SETTING //////////////////////////////////////

void getbatSetting() // API: ดึง JSON จาก littleFS แล้วส่ง battery.json ไปยัง Client
{
  server.on("/getbattsetting", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (!LittleFS.exists("/battery.json")) {
      request->send(404, "application/json", "{\"error\":\"battery.json not found\"}");
      return;
    }

    File file = LittleFS.open("/battery.json", "r");
    if (!file) {
      request->send(500, "application/json", "{\"error\":\"Failed to open file\"}");
      return;
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
      request->send(500, "application/json", "{\"error\":\"Failed to parse JSON\"}");
      return;
    }

    String jsonResponse;
    serializeJson(doc, jsonResponse);
    Serial.println("/get battsetting : " + jsonResponse);

    request->send(200, "application/json", jsonResponse); });
}

void savebatSetting() // API: รับ JSON จาก Client แล้วบันทึกไฟล์ battery.json ไปยัง littleFS
{
  server.on("/battsetting", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
            {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, data, len);
    if (error) {
      Serial.println("JSON parse failed!");
      request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
      return;
    }
    // Debug: แสดงค่าที่ได้รับ
    for (JsonPair kv : doc.as<JsonObject>()) {
      String key = kv.key().c_str();
      String value = kv.value().as<String>();
      Serial.printf("Received setting: %s = %s\n", key.c_str(), value.c_str());
    }
    // เปิดไฟล์เพื่อเขียนทับ
    File file = LittleFS.open("/battery.json", "w");
    if (!file) {
      request->send(500, "application/json", "{\"error\":\"Failed to open file\"}");
      return;
    }
    // เขียน JSON ลงไฟล์
    if (serializeJson(doc, file) == 0) {
      request->send(500, "application/json", "{\"error\":\"Failed to write JSON\"}");
      file.close();
      return;
    }
    file.close();
    request->send(200, "application/json", "{\"status\":\"ok\"}"); });
}

////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// WIFI SETTING /////////////////////////////////////////

void getNetwork() // API: ดึง JSON จาก littleFS แล้วส่ง battery.json ไปยัง Client
{
  server.on("/getnetworkconfig", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (!LittleFS.exists("/networkconfig.json")) {
      request->send(404, "application/json", "{\"error\":\"battery.json not found\"}");
      return;
    }

    File file = LittleFS.open("/networkconfig.json", "r");
    if (!file) {
      request->send(500, "application/json", "{\"error\":\"Failed to open file\"}");
      return;
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
      request->send(500, "application/json", "{\"error\":\"Failed to parse JSON\"}");
      return;
    }

    String jsonResponse;
    serializeJson(doc, jsonResponse);
    Serial.println("/get networkconfig : " + jsonResponse);

    request->send(200, "application/json", jsonResponse); });
}

void saveNetwork() // API: รับ JSON จาก Client แล้วบันทึกไฟล์ battery.json ไปยัง littleFS
{
  server.on("/networkconfig", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
            {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, data, len);
    if (error) {
      Serial.println("JSON parse failed!");
      request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
      return;
    }
    // Debug: แสดงค่าที่ได้รับ
    for (JsonPair kv : doc.as<JsonObject>()) {
      String key = kv.key().c_str();
      String value = kv.value().as<String>();
      Serial.printf("Received setting: %s = %s\n", key.c_str(), value.c_str());
    }
    // เปิดไฟล์เพื่อเขียนทับ
    File file = LittleFS.open("/networkconfig.json", "w");
    if (!file) {
      request->send(500, "application/json", "{\"error\":\"Failed to open file\"}");
      return;
    }
    // เขียน JSON ลงไฟล์
    if (serializeJson(doc, file) == 0) {
      request->send(500, "application/json", "{\"error\":\"Failed to write JSON\"}");
      file.close();
      return;
    }
    file.close();
    request->send(200, "application/json", "{\"status\":\"ok\"}"); });
}

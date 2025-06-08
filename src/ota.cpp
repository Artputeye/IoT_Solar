#include <ArduinoOTA.h>
#include "globals.h"

void setupOTAUpload()
{
  server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request) {
    Serial.println("Update...");
    bool success = !Update.hasError();
    request->send(200, "text/plain", success ? "OK" : "FAIL");
    if (success) {
      delay(1000);
      ESP.restart();
    }
  }, [](AsyncWebServerRequest *request, String filename, size_t index,
        uint8_t *data, size_t len, bool final) {

    if (!index) {
      String updateType = "firmware";
      if (request->hasParam("type", true)) {
        updateType = request->getParam("type", true)->value();
      }

      if (updateType == "filesystem") {
        Serial.println("Begin LittleFS update");
        Update.begin(LittleFS.totalBytes(), U_SPIFFS);
      } else {
        Serial.println("Begin firmware update");
        Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH);
      }
    }

    if (Update.write(data, len) != len) {
      Serial.printf("Update Write Error: %s\n", Update.errorString());
    }

    if (final) {
      if (Update.end(true)) {
        Serial.printf("Update Success: %u bytes\n", index + len);
      } else {
        Serial.printf("Update Failed: %s\n", Update.errorString());
      }
    }
  });
}
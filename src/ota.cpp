#include <ArduinoOTA.h>
#include "globals.h"

void setupOTAUpload()
{
  server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request) {
    bool success = Update.end(true);
    Serial.printf("[OTA] Update %s\n", success ? "SUCCESS" : "FAILED");
    if (!success) {
        Serial.printf("[OTA] Error: %s\n", Update.errorString());
    }

    request->send(200, "text/plain", success ? "OK" : "FAIL");

    if (success) {
        delay(1000);
        ESP.restart();
    }
}, [](AsyncWebServerRequest *request, String filename, size_t index,
      uint8_t *data, size_t len, bool final) {

    if (!index) {
        Serial.println("[OTA] Begin firmware update");
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
            Serial.printf("[OTA] Error begin: %s\n", Update.errorString());
        }
    }

    if (Update.write(data, len) != len) {
        Serial.printf("[OTA] Write error: %s\n", Update.errorString());
    }

    if (final) {
        Serial.println("[OTA] Finalizing...");
    }
});

}
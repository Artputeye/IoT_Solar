#include <ArduinoOTA.h>
#include "globals.h"

void setupOTAUpload()
{

  server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    bool success = Update.end(true);
    Serial.printf("[OTA] Update %s\n", success ? "SUCCESS" : "FAILED");

    if (!success) {
      Serial.printf("[OTA] Error: %s\n", Update.errorString());
      request->send(500, "text/plain", "Update failed: " + String(Update.errorString()));
    } else {
      request->send(200, "text/plain", "Update successful");
      delay(1000);
      ESP.restart();
    } }, [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
            {
    static int updateCmd = U_FLASH;  // default เป็น firmware

    if (index == 0) {
      String updateType = "firmware"; // ค่า default
      if (request->hasParam("type", true)) {
        updateType = request->getParam("type", true)->value();
      }

      Serial.printf("[OTA] Begin update: %s, filename: %s\n", updateType.c_str(), filename.c_str());

      // if (updateType == "littlefs") {
      //   updateCmd = U_SPIFFS;
      // } else {
      //   updateCmd = U_FLASH;
      // }
//////////////////////////////////////////////////
   if (updateType == "littlefs") {
      updateCmd = U_SPIFFS; // ใช้ U_FS สำหรับ LittleFS บน ESP32
      Serial.println("Selected update type: LittleFS (U_SPIFFS)");
    } else if (updateType == "firmware") {
      updateCmd = U_FLASH;
      Serial.println("Selected update type: Firmware (U_FLASH)");
    } else {
      Serial.println("Invalid update type provided! Aborting OTA.");
      request->send(400, "text/plain", "Invalid update type");
      Update.abort(); // สั่งยกเลิกการอัปเดต
      return; // ออกจากการทำงาน
    }

///////////////////////////////////////////////////////
      if (!Update.begin(UPDATE_SIZE_UNKNOWN, updateCmd)) {
        Serial.printf("[OTA] Error begin: %s\n", Update.errorString());
        return;
      }
    }

    if (len) {
      if (Update.write(data, len) != len) {
        Serial.printf("[OTA] Write error: %s\n", Update.errorString());
      } else {
        Serial.printf("[OTA] Written %u bytes\n", len);
      }
    }

    if (final) {
      Serial.println("[OTA] Finalizing update...");
    } });
}

// server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request)
//             {
//     bool success = Update.end(true);
//     Serial.printf("[OTA] Update %s\n", success ? "SUCCESS" : "FAILED");
//     if (!success) {
//         Serial.printf("[OTA] Error: %s\n", Update.errorString());
//     }

//     request->send(200, "text/plain", success ? "OK" : "FAIL");

//     if (success) {
//         delay(1000);
//         ESP.restart();
//     } }, [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
//             {

//     if (!index) {
//         Serial.println("[OTA] Begin firmware update");
//         if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
//             Serial.printf("[OTA] Error begin: %s\n", Update.errorString());
//         }
//     }

//     if (Update.write(data, len) != len) {
//         Serial.printf("[OTA] Write error: %s\n", Update.errorString());
//     }

//     if (final) {
//         Serial.println("[OTA] Finalizing...");
//     } });

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

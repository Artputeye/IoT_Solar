// main.cpp
#include "globals.h"

unsigned long lastTime1 = 0;
unsigned long lastTime2 = 0;
bool wstate;

WiFiClient client;           // home assistant
HADevice device;             // home assistant
HAMqtt mqtt(client, device); // home assistant ต้องประกาศ Object ใน main.cpp เท่านั้น ไม่งั้น mqtt broker หาไม่เจอ

void TaskMain(void *pvParameters);
void TaskSub(void *pvParameters);

void setup()
{
  // === Serial Setup ===
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.print("Booting");
  for (int i = 0; i < 5; i++)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();

  Serial2.begin(2400, SERIAL_8N1, RX_pin, TX_pin);
  pinMode(LED, OUTPUT);
  ledIndicator(200, 200);
  Serial.println("Serial Setup Completed");
  delay(300);

  // === File System Setup ===
  if (!LittleFS.begin(true))
  {
    Serial.println("LittleFS Mount Failed");
    return;
  }
  Serial.println("LittleFS Mounted Successfully");
  delay(300);

  // === Device Configuration ===
  mac_config();
  wifi_config();
  iotHAsetup();
  Serial.println("Home Assistant Initialized");
  delay(300);

  // === mDNS & Web Setup ===
  MDNS.begin("inverter");
  Serial.println("mDNS Responder Started");
  delay(300);

  webHandle();
  initWebSocket();
  server.begin();
  Serial.println("Web Server Started");
  delay(300);

  // === Watchdog Setup ===
  esp_task_wdt_init(WDT_TIMEOUT, true); // Enable WDT with system reset
  esp_task_wdt_delete(NULL);            // Remove loopTask from WDT
  // esp_task_wdt_add(NULL);            // (Don't add loopTask if it's unused)

  // === Task Creation ===
  TaskHandle_t xHandleMain = NULL;
  TaskHandle_t xHandleSub = NULL;

  xTaskCreatePinnedToCore(
      TaskMain, "TaskMain", 4096, NULL, 1, &xHandleMain, 0);
  xTaskCreatePinnedToCore(
      TaskSub, "TaskSub", 3000, NULL, 1, &xHandleSub, 1);

  // === Add tasks to Watchdog ===
  esp_task_wdt_add(xHandleMain);
  esp_task_wdt_add(xHandleSub);

  Serial.println("Setup Complete");
}

void loop()
{
  esp_task_wdt_reset();
  delay(100);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void TaskMain(void *pvParameters) // CPU Core0
{
  unsigned long last = 0;
  bool toggle = false;
  while (1)
  {
    // Main function
    mqtt.loop();
    fileManage();
    restart();
    if ((millis() - lastTime1) > 3000) // Delay time
    {
      lastTime1 = millis();
      if (inv.RunMode)
      {
        inv.cmd_inv("QPIGS");
      }
      inv.Response();
      wsJsonInverter(inv.invData);
      iotHArun();
      simulateData();
      // Serial.printf("WIFI MODE: %s\n", (wstate == 0 ? "Unconnect" : "Connect"));
    }

    // Stackcheck
    if ((millis() - last) > 10000)
    {
      if (toggle)
      {
        inv.cmd_inv("QPIRI"); // ส่งคำสั่งแรก
      }
      else
      {
        inv.cmd_inv("QPIWS"); // ส่งคำสั่งที่สอง
      }
      toggle = !toggle;
      last = millis();
      UBaseType_t stackRemaining = uxTaskGetStackHighWaterMark(NULL);
      // Serial.print("Remain Stack Maintask: ");
      // Serial.println(stackRemaining);
    }
    // Serial.println("Debug e1");
    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void TaskSub(void *pvParameters) // CPU Core1
{
  unsigned long last = 0;
  while (1)
  {
    wstate = (WiFi.status() == WL_CONNECTED);
    if (wstate)
    {
      ledIndicator(100, 2000);
    }
    else
    {
      ledIndicator(300, 300);
    }

    if ((millis() - lastTime2) > 100) // Delay time
    {
      lastTime2 = millis();
      inv.serialSent();
      wsJsonSerial(inv.serialData);
      inv.Response();
    }

    wsloop();
    // Stackcheck
    if ((millis() - last) > 10000)
    {
      last = millis();
      UBaseType_t stackRemaining = uxTaskGetStackHighWaterMark(NULL);
      showAPClients();
      // Serial.print("Remain Stack Subtask: ");
      // Serial.println(stackRemaining);
    }
    // Serial.println("Debug e2");
    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

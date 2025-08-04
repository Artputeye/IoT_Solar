#include "wifiConfig.h"

unsigned long last = 0;
int t = 500; // time led status
float power = 0.0;
float lastPower = 0.0;
unsigned long lastChangeTime = 0;            // เวลาเปลี่ยนแปลงล่าสุด (ms)
const unsigned long timeout = 5 * 60 * 1000; // 2 นาที (300,000 ms)

void mac_config()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin();
  WiFi.macAddress(Mac);
  device.setUniqueId(Mac, sizeof(Mac));
}

void wifi_config()
{
    wifiManager();
    wifiMulti();
    lastChangeTime = millis();
}

void wifiManager()
{
    WiFiManager wm;
    wm.setConfigPortalTimeout(120);
    bool res;
    res = wm.autoConnect(DEVICE_NAME, PASSWORD);
    if (!res)
    {
        Serial.println("Failed to connect");
        delay(10000);
        ESP.restart();
    }
    else
    {
        Serial.println();
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    }
}

void wifiMulti()
{
    ////////////////
}

void restart()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Lost WiFi connection. Restarting...");
        delay(1000);
        ESP.restart();
    }
//////////////////////////////////////////////////////////////////////////////////
    power = inv.data.ActivePower; 
    if (power != lastPower)
    {
        lastChangeTime = millis(); // อัพเดทเวลาถ้ามีการเปลี่ยน
        lastPower = power;
    }
    if (millis() - lastChangeTime >= timeout)
    {
        Serial.println("Power value unchanged for 5 minutes. Restarting...");
        delay(1000);
        ESP.restart(); 
    }
}

void ledIndicator(unsigned long onTime, unsigned long offTime) {
  static bool ledState = false;
  static unsigned long previousMillis = 0;

  unsigned long currentMillis = millis();
  unsigned long interval = ledState ? onTime : offTime;

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(LED, ledState ? HIGH : LOW);
  }
}
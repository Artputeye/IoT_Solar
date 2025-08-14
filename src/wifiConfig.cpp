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
    readNetwork();
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
    Multi.addAP(DEVICE_NAME, "12345678");
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

void ledIndicator(unsigned long onTime, unsigned long offTime)
{
    static bool ledState = false;
    static unsigned long previousMillis = 0;

    unsigned long currentMillis = millis();
    unsigned long interval = ledState ? onTime : offTime;

    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;
        ledState = !ledState;
        digitalWrite(LED, ledState ? HIGH : LOW);
    }
}

void readNetwork()
{
    if (!LittleFS.exists("/networkconfig.json"))
    {
        Serial.println("networkconfig.json not found\"}");
        return;
    }

    File file = LittleFS.open("/networkconfig.json", "r");
    if (!file)
    {
        Serial.println("Failed to open file\"}");
        return;
    }

    JsonDocument doc; // ขนาดปรับตามไฟล์จริง
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error)
    {
        Serial.printf("Failed to parse JSON: %s\n", error.c_str());
        return;
    }

    // คัดลอก string จาก JSON ลง buffer
    strncpy(WIFI_NAME, doc["wifi_name"] | "", sizeof(WIFI_NAME));
    strncpy(PASSWORD, doc["wifi_password"] | "", sizeof(PASSWORD));
    
    strncpy(MQTT_USERNAME, doc["mqtt_user"] | "", sizeof(MQTT_USERNAME));
    strncpy(MQTT_PASSWORD, doc["mqtt_password"] | "", sizeof(MQTT_PASSWORD));
    strncpy(MQTT_ADDR, doc["mqtt_server"] | "", sizeof(MQTT_ADDR));
    strncpy(MQTT_PORT, doc["mqtt_port"] | "", sizeof(MQTT_PORT));

    // Debug
    Serial.printf("WIFI_NAME: %s\n", WIFI_NAME);
    Serial.printf("PASSWORD: %s\n", PASSWORD);
    Serial.printf("MQTT_ADDR: %s\n", MQTT_ADDR);
    Serial.printf("MQTT_USERNAME: %s\n", MQTT_USERNAME);
    Serial.printf("MQTT_PASSWORD: %s\n", MQTT_PASSWORD);
    Serial.printf("MQTT_PORT: %s\n", MQTT_PORT);
}

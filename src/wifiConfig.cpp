#include "wifiConfig.h"

// ===== Globals =====
char MacAddr[18];

static bool wifiConfigured = false;

const uint8_t AP_PIN = 0;
static const unsigned long HOLD_MS = 5000;
static const unsigned long WIFI_TIMEOUT = 5 * 60 * 1000;

// ===== State =====
static bool pressed = false;
static bool apModeActive = false;
static unsigned long pressStart = 0;

static float lastPower = 0;
static unsigned long lastChangeTime = 0;

// =====================================================
// Restart logic
// =====================================================
void restartCheck()
{
    float power = inv.data.ActivePower;

    if (inv.RunMode)
    {
        if (power != lastPower)
        {
            lastPower = power;
            lastChangeTime = millis();
        }

        if (millis() - lastChangeTime >= WIFI_TIMEOUT)
        {
            Serial.println("⚠️ Power stuck → Restarting...");
            vTaskDelay(pdMS_TO_TICKS(1000));
            ESP.restart();
        }
    }

    if (!wifiConfigured && (inv.wifi_config || inv.ip_config))
    {
        wifiConfigured = true;
        setupWiFiMode();
    }
}

// =====================================================
// Button → AP mode
// =====================================================
void APmode()
{
    bool isPressed = (digitalRead(AP_PIN) == LOW);

    if (isPressed && !pressed)
    {
        pressed = true;
        pressStart = millis();
    }
    else if (!isPressed && pressed)
    {
        pressed = false;
    }

    if (pressed && !apModeActive && millis() - pressStart >= HOLD_MS)
    {
        Serial.println("🔧 Enter AP Mode (Long Press)");

        wifimode = 0;
        saveApSetting();

        vTaskDelay(pdMS_TO_TICKS(2000));
        ESP.restart();
    }
}

// =====================================================
// Setup
// =====================================================
void wifiSetup()
{
    readNetwork();

    if (wifimode == 1)
    {
        setupIPConfig();
    }

    setupWiFiMode();
    lastChangeTime = millis();
}

// =====================================================
// MAC config
// =====================================================
void mac_config()
{
    uint8_t mac[6];
    WiFi.macAddress(mac);

    device.setUniqueId(mac, sizeof(mac));

    snprintf(MacAddr, sizeof(MacAddr),
             "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2],
             mac[3], mac[4], mac[5]);

    Serial.printf("MAC: %s\n", MacAddr);
}

// =====================================================
// Read JSON config
// =====================================================
void readNetwork()
{
    if (!LittleFS.exists("/networkconfig.json"))
    {
        Serial.println("❌ networkconfig.json not found");
        return;
    }

    File file = LittleFS.open("/networkconfig.json", "r");
    if (!file)
    {
        Serial.println("❌ open file failed");
        return;
    }

    JsonDocument doc;
    auto err = deserializeJson(doc, file);
    file.close();

    if (err)
    {
        Serial.printf("❌ JSON parse error: %s\n", err.c_str());
        return;
    }

    wifimode = atoi(doc["wifi_mode"] | "0");
    ipconfig = atoi(doc["ip_config"] | "0");

    strlcpy(WIFI_NAME, doc["wifi_name"] | "", sizeof(WIFI_NAME));
    strlcpy(PASSWORD, doc["wifi_password"] | "", sizeof(PASSWORD));

    strlcpy(IP_ADDR, doc["ip_address"] | "", sizeof(IP_ADDR));
    strlcpy(SUBNET_MASK, doc["subnet_mask"] | "", sizeof(SUBNET_MASK));
    strlcpy(DEFAULT_GATEWAY, doc["default_gateway"] | "", sizeof(DEFAULT_GATEWAY));

    strlcpy(MQTT_USERNAME, doc["mqtt_user"] | "", sizeof(MQTT_USERNAME));
    strlcpy(MQTT_PASSWORD, doc["mqtt_password"] | "", sizeof(MQTT_PASSWORD));
    strlcpy(MQTT_ADDR, doc["mqtt_server"] | "", sizeof(MQTT_ADDR));
    strlcpy(MQTT_PORT, doc["mqtt_port"] | "", sizeof(MQTT_PORT));

    Serial.printf("Mode: %s | IP: %s\n",
                  wifimode ? "STA" : "AP",
                  ipconfig ? "STATIC" : "DHCP");
}

// =====================================================
// WiFi Mode
// =====================================================
void setupWiFiMode()
{
    if (wifimode == 0)
    {
        Serial.println("📡 AP Mode");

        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(IPAddress(192,168,4,1),
                          IPAddress(192,168,4,1),
                          IPAddress(255,255,255,0));

        WiFi.softAP(DEVICE_NAME, DEVICE_PASS);
        Serial.println(WiFi.softAPIP());
    }
    else
    {
        Serial.println("📡 STA Mode");

        WiFi.mode(WIFI_STA);
        WiFi.setSleep(false);
        WiFi.begin(WIFI_NAME, PASSWORD);
    }
}

// =====================================================
// IP config
// =====================================================
void setupIPConfig()
{
    if (ipconfig != 1) return;

    IPAddress ip = parseIP(IP_ADDR);
    IPAddress gw = parseIP(DEFAULT_GATEWAY);
    IPAddress sn = parseIP(SUBNET_MASK);

    Serial.println("🌐 Static IP");

    if (!WiFi.config(ip, gw, sn))
    {
        Serial.println("❌ Static IP failed");
    }
}

// =====================================================
// Utils
// =====================================================
IPAddress parseIP(const char *ipStr)
{
    int a, b, c, d;
    if (sscanf(ipStr, "%d.%d.%d.%d", &a, &b, &c, &d) == 4)
        return IPAddress(a, b, c, d);

    return IPAddress(0, 0, 0, 0);
}

// =====================================================
// Show AP Clients
// =====================================================
void showAPClients()
{
    if (wifimode != 0) return;

    wifi_sta_list_t wifiList;
    esp_netif_sta_list_t netifList;

    if (esp_wifi_ap_get_sta_list(&wifiList) != ESP_OK) return;
    if (esp_netif_get_sta_list(&wifiList, &netifList) != ESP_OK) return;

    Serial.printf("Clients: %d\n", netifList.num);

    for (int i = 0; i < netifList.num; i++)
    {
        auto &s = netifList.sta[i];

        Serial.printf("%d) %02X:%02X:%02X:%02X:%02X:%02X | %s\n",
            i + 1,
            s.mac[0], s.mac[1], s.mac[2],
            s.mac[3], s.mac[4], s.mac[5],
            ip4addr_ntoa((ip4_addr_t*)&s.ip.addr)
        );
    }
}

// =====================================================
// WiFi Event
// =====================================================
void WiFiEvent(WiFiEvent_t event)
{
    switch (event)
    {
    case ARDUINO_EVENT_WIFI_STA_START:
        Serial.println("📡 WiFi STA started");
        break;

    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        Serial.print("✅ WiFi connected, IP: ");
        Serial.println(WiFi.localIP());

        ledMode = LED_CONNECTED;

        // ===== NTP init =====
        Serial.println("⏱ Init NTP");

        configTime(
            7 * 3600,
            0,
            "time.google.com",
            "time1.google.com",
            "time2.google.com");

        ntpSynced = false;

        if (ntpTaskHandle == NULL)
        {
            xTaskCreatePinnedToCore(
                TaskNTP,
                "TaskNTP",
                4096,
                NULL,
                1,
                &ntpTaskHandle,
                1);
        }

        // ===== MQTT / Service start =====
        // mqtt.connect(); (ถ้ามี)

        break;

    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        Serial.println("⚠️ WiFi disconnected");

        ledMode = LED_DISCONNECTED;

        // ป้องกัน reconnect ถี่
        static unsigned long lastRetry = 0;
        if (millis() - lastRetry > 5*60*1000)
        {
            lastRetry = millis();
            Serial.println("🔄 Reconnecting WiFi...");
            if (WiFi.getMode() == WIFI_STA)
            {
                ESP.restart();
            }
        }
        break;

    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
        Serial.println("⚠️ WiFi lost IP");
        break;

    case ARDUINO_EVENT_WIFI_STA_STOP:
        Serial.println("🛑 WiFi stopped");
        break;

    default:
        break;
    }
}

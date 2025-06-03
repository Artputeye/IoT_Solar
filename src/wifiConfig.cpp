#include "globals.h"
#include "wifiConfig.h"

bool shouldSaveConfig = false;

void wifi_para()
{
    Serial.println("mounting FS...");
    if (LittleFS.begin())
    {
        Serial.println("mounted file system");
        if (LittleFS.exists("/config.json"))
        {
            Serial.println("reading config file");
            File configFile = LittleFS.open("/config.json", "r");
            if (configFile)
            {
                Serial.println("opened config file");
                size_t size = configFile.size();
                std::unique_ptr<char[]> buf(new char[size]);
                configFile.readBytes(buf.get(), size);

                JsonDocument json;
                DeserializationError error = deserializeJson(json, buf.get());

                serializeJson(json, Serial); // optional: for debug
                if (!error)
                {
                    Serial.println("\nparsed json");
                    strlcpy(DEVICE_NAME, json["DEVICE_NAME"] | "", sizeof(DEVICE_NAME));
                    strlcpy(MQTT_ADDR, json["MQTT_ADDR"] | "", sizeof(MQTT_ADDR));
                    strlcpy(MQTT_USERNAME, json["MQTT_USERNAME"] | "", sizeof(MQTT_USERNAME));
                    strlcpy(MQTT_PASSWORD, json["MQTT_PASSWORD"] | "", sizeof(MQTT_PASSWORD));
                    strlcpy(MQTT_PORT, json["MQTT_PORT"] | "", sizeof(MQTT_PORT));
                }
                else
                {
                    Serial.println("failed to load json config");
                }
                configFile.close();
            }
        }
    }
    else
    {
        Serial.println("failed to mount LittleFS");
    }

    // custom WiFiManager parameters
    WiFiManagerParameter custom_device_name("DEVICE_NAME", "DEVICE NAME", DEVICE_NAME, 28);
    WiFiManagerParameter custom_mqtt_addr("MQTT_ADDR", "MQTT ADDR", MQTT_ADDR, 16);
    WiFiManagerParameter custom_mqtt_username("MQTT_USERNAME", "MQTT USERNAME", MQTT_USERNAME, 28);
    WiFiManagerParameter custom_mqtt_password("MQTT_PASSWORD", "MQTT PASSWORD", MQTT_PASSWORD, 28);
    WiFiManagerParameter custom_mqtt_port("MQTT_PORT", "MQTT PORT", MQTT_PORT, 5);

    WiFiManager wifiManager;
    wifiManager.setSaveConfigCallback(saveConfigCallback);
    wifiManager.addParameter(&custom_device_name);
    wifiManager.addParameter(&custom_mqtt_addr);
    wifiManager.addParameter(&custom_mqtt_username);
    wifiManager.addParameter(&custom_mqtt_password);
    wifiManager.addParameter(&custom_mqtt_port);

    if (inv.para)
    {
        // reset settings - for testing
        wifiManager.resetSettings();
        inv.para = false;
    }

    if (!wifiManager.autoConnect("ARTTECH", "12345678"))
    {
        Serial.println("failed to connect and hit timeout");
        delay(5000);
        ESP.restart();
        delay(5000);
    }

    Serial.println("Connected... :)");

    strlcpy(DEVICE_NAME, custom_device_name.getValue(), sizeof(DEVICE_NAME));
    strlcpy(MQTT_ADDR, custom_mqtt_addr.getValue(), sizeof(MQTT_ADDR));
    strlcpy(MQTT_USERNAME, custom_mqtt_username.getValue(), sizeof(MQTT_USERNAME));
    strlcpy(MQTT_PASSWORD, custom_mqtt_password.getValue(), sizeof(MQTT_PASSWORD));
    strlcpy(MQTT_PORT, custom_mqtt_port.getValue(), sizeof(MQTT_PORT));

    Serial.println("The values in the file are: ");
    Serial.println("\tDEVICE_NAME : " + String(DEVICE_NAME));
    Serial.println("\tMQTT_ADDR : " + String(MQTT_ADDR));
    Serial.println("\tMQTT_USERNAME : " + String(MQTT_USERNAME));
    Serial.println("\tMQTT_PASSWORD : " + String(MQTT_PASSWORD));
    Serial.println("\tMQTT_PORT : " + String(MQTT_PORT));

    if (shouldSaveConfig)
    {
        Serial.println("saving config");
        JsonDocument json;
        json["DEVICE_NAME"] = DEVICE_NAME;
        json["MQTT_ADDR"] = MQTT_ADDR;
        json["MQTT_USERNAME"] = MQTT_USERNAME;
        json["MQTT_PASSWORD"] = MQTT_PASSWORD;
        json["MQTT_PORT"] = MQTT_PORT;

        File configFile = LittleFS.open("/config.json", "w");
        if (!configFile)
        {
            Serial.println("failed to open config file for writing");
        }
        else
        {
            serializeJson(json, Serial);
            serializeJson(json, configFile);
            configFile.close();
        }
    }

    Serial.println();
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

// callback notifying us of the need to save config
void saveConfigCallback()
{
    Serial.println("Should save config");
    shouldSaveConfig = true;
}

void fsformat()
{
    // clean FS, for testing
    LittleFS.format();
}
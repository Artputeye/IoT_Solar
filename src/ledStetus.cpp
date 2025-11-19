#include "ledStatus.h"

volatile LedMode ledMode = LED_DISCONNECTED; 

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

void ledPatternSelect()
{
    switch (ledMode)
    {
        case LED_OFF:
            ledIndicator(0, 1000);   // always off
            break;

        case LED_ON:
            ledIndicator(1000, 0);   // always on

        case LED_CONNECTED:
            ledIndicator(100, 2000); // short blink every 2 sec
            break;

        case LED_DISCONNECTED:
            ledIndicator(300, 300);  // slow blink
            break;
        
        case LED_AP_MODE:
            ledIndicator(100, 500);  // AP blink
            break;

        case LED_MQTT_FAIL:
            ledIndicator(100, 100);  // fast blink
            break;

        case LED_OTA_RUNNING:
            ledIndicator(20, 20);    // very fast blink
            break;

        case LED_FAULT:
            ledIndicator(500, 500);  // steady blink
            break;

        case LED_BUSY:
            ledIndicator(50, 300);   // medium blink
            break;
    }
}


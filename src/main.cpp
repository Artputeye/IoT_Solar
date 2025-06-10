#include "globals.h"
#include "wifiConfig.h"
#include "serv.h"
#include "ota.h"
#include "invCommand.h"
#include "fileSys.h"
#include "iotHA.h"

unsigned long last = 0;
unsigned long lastTime1 = 0;
unsigned long lastTime2 = 0;
unsigned long timerDelay1 = 3000;
unsigned long timerDelay2 = 100;

void setup()
{
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Booting...");
  Serial2.begin(2400, SERIAL_8N1, RX_pin, TX_pin);
  pinMode(LED, OUTPUT);
  Serial.print("Setup...");

  // setup all function
  delay(300);
  MacSetup();
  wifi_para();
  iotHAsetup();
  ///////////////////////////////////////////////////
  
  setupServer();
}

void condition();
void ledIndicator();

void loop()
{
  // Main function
  mqtt.loop();
  condition();
  ledIndicator();

  if ((millis() - lastTime1) > timerDelay1)
  {
    lastTime1 = millis();
    inv.cmd_inv("QPIGS");
    inv.Response();
    iotHA();
    test = inv.test;
    // Serial.println("Main function");
  }

  // Manual function
  if ((millis() - lastTime2) > timerDelay2)
  {
    lastTime2 = millis();
    inv.serialSent();
    inv.Response();
  }
}

void condition()
{
  if (inv.dir)
  {
    listAllFilesAndFolders(targetDirectory);
    inv.dir = false;
  }

  if (inv.format)
  {
    LittleFS.format();
    inv.format = false;
  }
}

void ledIndicator()
{
  if ((millis() - last) > t)
  {
    last = millis();
    if (ledState == LOW)
    {
      ledState = HIGH;
    }
    else
    {
      ledState = LOW;
    }
    digitalWrite(LED, ledState);
  }
}
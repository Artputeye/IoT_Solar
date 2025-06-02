
#include "wifiConfig.h"
#include "serv.h"
#include "ota.h"
#include "invCommand.h"
#include "iotHA.h"

unsigned long last = 0;
unsigned long lastTime1 = 0;
unsigned long lastTime2 = 0;
unsigned long timerDelay1 = 3000;
unsigned long timerDelay2 = 100;
int t = 1000;
bool test;
int ledState = LOW;

invCommand inv;

void setup()
{
  Serial.begin(115200);
  Serial2.begin(2400, SERIAL_8N1, RX_pin, TX_pin);
  pinMode(LED, OUTPUT);

  Serial.print("Setup...");
  delay(300);
  MacSetup();
  wifi_para();
  iotHAsetup();
  otaSetup();
  setupServer();
}

void loop()
{
}
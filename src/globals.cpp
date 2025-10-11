#include "globals.h"

AsyncWebServer server(80);

invCommand inv; // command inverter

char DEVICE_NAME[28] = "Hybrid Inverter"; //= "INVERTER";  //"Anern 4.2kW";  //"ARRTECH INVERTER";
char DEVICE_PASS[28] = "12345678"; //= "INVERTER";  //"Anern 4.2kW";  //"ARRTECH INVERTER";

char WIFI_NAME[30];
char PASSWORD[25];

char IP_ADDR[16];
char SUBNET_MASK[16];
char DEFAULT_GATEWAY[16];

char MQTT_ADDR[16];     //= "192.168.101.100";//"192.168.1.247";
char MQTT_USERNAME[28]; //= "mqtt-user";//"inverter";  // replace with your credentials
char MQTT_PASSWORD[28]; //= "12345678";
char MQTT_PORT[5];      //= "1883";

char D_SoftwareVertion[15] = "1.2.8";
char D_Mfac[15] = "ARTTECH";
char D_Model[15] = "IoT Solar";

char user[10] = "admin"; // OTA Authentication pass
char pass[10] = "12345678";

uint8_t Mac[6];

const char* ntpServer = "time.google.com";
const long  gmtOffset_sec = 7 * 3600;
const int   daylightOffset_sec = 0;

int dateNow = 0; // ตัวแปรเก็บวันที่แบบ int

int ledState = LOW;

String wsSerial;
String wsInverter;

bool ipconfig;
bool wifimode;

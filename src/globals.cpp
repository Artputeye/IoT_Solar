#include "globals.h"

AsyncWebServer server(80);
invCommand inv; // command inverter

char DEVICE_NAME[28] = "Hybrid Inverter"; //= "INVERTER";  //"Anern 4.2kW";  //"ARRTECH INVERTER";
char PASSWORD[25] = "12345678";
char MQTT_ADDR[16] = "192.168.1.247"; //= "192.168.101.100";//"192.168.1.247";
char MQTT_USERNAME[28] = "inverter";  //= "mqtt-user";//"inverter";  // replace with your credentials
char MQTT_PASSWORD[28] = "12345678";      //= "12345678";
char MQTT_PORT[5] = "1883";           //= "1883";

char D_SoftwareVertion[15]="1.2.8";
char D_Mfac[15]="ARTTECH";
char D_Model[15]="IoT Solar";

char user[10] = "admin"; // OTA Authentication pass
char pass[10] = "12345678";

uint8_t Mac[6];

int ledState = LOW;


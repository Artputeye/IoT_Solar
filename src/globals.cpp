#include "globals.h"

// object define
//WebServer _server(8080);
AsyncWebServer server(80);
WiFiClient client;
HADevice device;             // home assistant
HAMqtt mqtt(client, device); // home assistant
invCommand inv;              // command inverter

char DEVICE_NAME[28] = "INVERTER";    //= "INVERTER";  //"Anern 4.2kW";  //"ARRTECH INVERTER";
char MQTT_ADDR[16] = "192.168.1.247"; //= "192.168.101.100";//"192.168.1.247";
char MQTT_USERNAME[28] = "mqtt-user"; //= "mqtt-user";//"inverter";  // replace with your credentials
char MQTT_PASSWORD[28] = "1234";      //= "1234";
char MQTT_PORT[5] = "1883";           //= "1883";

char user[10] = "admin"; //OTA Authentication pass
char pass[10]= "12345"; 

int ledState = LOW;
int t = 1000; // time led status
bool test;    // test func tion


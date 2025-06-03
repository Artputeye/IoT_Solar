#include "globals.h"

// object define
WiFiClient client;
HADevice device;
HAMqtt mqtt(client, device);
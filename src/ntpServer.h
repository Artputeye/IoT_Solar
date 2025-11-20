#ifndef NTPSERVER_H
#define NTPSERVER_H
#include "globals.h"

extern const char *serverUrl;
extern const long gmtOffset_sec;
extern const int daylightOffset_sec;
extern volatile int dateNow;
extern struct tm timeinfo;

void timeServer();
void timeRefresh();

#endif

#ifndef NTPSERVER_H
#define NTPSERVER_H
#include "globals.h"

extern struct tm timeinfo;
extern int dateNow;

void initNTP();
void ntpLoop();

#endif

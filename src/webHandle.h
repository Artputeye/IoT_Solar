#ifndef WEB_HANDLE_H
#define WEB_HANDLE_H
#include "globals.h"
extern int gridCutOff;
extern int gridStart;

void webHandle();
String getContentType(String filename);
void staticRoot();
void notfoundRoot();
void JsonSetting();
void cmdSetting();
void getSetting();
void saveSetting();
void getbatSetting();
void savebatSetting();
void getNetwork();
void saveNetwork();

#endif
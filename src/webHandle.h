#ifndef WEB_HANDLE_H
#define WEB_HANDLE_H
#include "globals.h"

void webHandle();
String getContentType(String filename);
void staticRoot();
void notfoundRoot();
void parameterSetting();
void getSetting();
void saveSetting();
void getbatSetting();
void savebatSetting();
void getNetwork();
void saveNetwork() ;

#endif
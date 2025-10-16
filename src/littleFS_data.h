#ifndef FILE_SYS_H
#define FILE_SYS_H
#include "globals.h"

void fileManage();
void listAllFilesAndFolders(const char *dirname);
bool loadEnergyFromFile();
bool saveEnergyToFile();
bool clearEnergyFile();
bool loadSetting();

#endif
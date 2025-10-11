#ifndef FILE_SYS_H
#define FILE_SYS_H
#include "globals.h"

void fileManage();
void listAllFilesAndFolders(const char *dirname);
void saveEnergyToFile();
void loadEnergyFromFile();
void clearEnergyFile();
void handleEnergyStorage();

#endif
#ifndef GRID_OPERATION_H
#define GRID_OPERATION_H
#include "globals.h"

extern struct tm timeinfo;
extern float energy_kWh;
extern float gridPower ;

void timeServer();
void timeRefresh();
void gridRun();
void gridOperation();


#endif
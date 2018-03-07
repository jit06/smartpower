#ifndef SMART_POWER_SENSORS_H
#define SMART_POWER_SENSORS_H

#include <SimpleDHT.h>

#include "Timehandler.h"
#include "Settings.h"

typedef struct {
  time_t timeStamp;
  float value;
} Storage;


void readCurrentConsumption();
void readTemperature();
void storeValue(float value, Storage *store, int *counter);
float getLastTemperature();
float getLastCurrentConsumption();
float getLastStoredValue(Storage *store, int *counter);
Storage getStoredTemperature(int pos);
Storage getStoredCurrent(int pos);
#endif

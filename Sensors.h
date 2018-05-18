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
float getCurrentFromACSValue(float acsvalue, int neutral);
float getEffectiveCurrentInAhFromCurrent(float current);
void storeValue(float value, Storage *store, int *counter);
float getLastTemperature();
float getLastCurrent5vConsumption();
float getLastCurrent12vConsumption();
float getLastStoredValue(Storage *store, int *counter);
float getLastPowerConsumption();
Storage getStoredTemperature(int pos);
Storage getStored5vCurrent(int pos);
Storage getStored12vCurrent(int pos);
Storage getStoredPower(int pos);
#endif

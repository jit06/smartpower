#ifndef SMART_POWER_CONTROLLER_H
#define SMART_POWER_CONTROLLER_H

#include <InvertedTM1638.h>
#include <TM1638.h>
#include <TM1638QYF.h>
#include <TM1640.h>
#include <TM16XX.h>
#include <TM16XXFonts.h>

#include <math.h>

#include "Settings.h"

void initController();
void handleKeyPressed();
void updateSwitches(byte keys);
void toggleSwitch(int pos);
void updateLeds();
void updateDisplay(float currentConsumption, float temperature);
float round_to_dp( float in_value, int decimal_place);
void initSwitches();

#endif

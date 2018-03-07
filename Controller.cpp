#include "Controller.h"
#include "Settings.h"

// Global variable to handle states
byte CURRENT_STATE = 0x00;
byte PREVIOUS_KEYS = 0x00;

// Power Switches pin assignement
int POWER_SWITCH_PIN[] = { PW_SWITCH1_PIN,   // control = SW_1 
                           PW_SWITCH2_PIN,   // control = SW_2
                           PW_SWITCH3_PIN,   // control = SW_3
                           PW_SWITCH4_PIN,   // control = SW_4
                           PW_SWITCH5_PIN,   // control = SW_5
                           PW_SWITCH6_PIN,   // control = SW_6
                           PW_SWITCH7_PIN,   // control = SW_7
                           PW_SWITCH8_PIN }; // control = SW_8

// global interface to TM1638
TM1638 module(TM1638_DATA_PIN, TM1638_CLCK_PIN, TM1638_STRB_PIN, true, TM1638_BRIGHTNESS);


void initController() {
  // init TM1638 Keys' previous status storage
  PREVIOUS_KEYS = module.getButtons();

  // Init Power switch : default to OFF except for pin 4...
  for(int i=0; i<sizeof(POWER_SWITCH_PIN); i++) {
    // ... pin 4 is automatically set to LOW by Ethernet module
    if(POWER_SWITCH_PIN[i] == 4) {
      CURRENT_STATE |= (1 << i);
    } else {
      digitalWrite(POWER_SWITCH_PIN[i], HIGH);
    }
    
    pinMode(POWER_SWITCH_PIN[i], OUTPUT);
  }

  updateLeds();
}

// toggle switches and leds status regarding pressed buttons
void handleKeyPressed() {
  byte keys = module.getButtons();

  // is any new button pressed ?
  if(keys!=PREVIOUS_KEYS) {
    updateSwitches(keys);
    PREVIOUS_KEYS = keys;
  }
}

// check for for any pressed button to toggle Power Switch State
void updateSwitches(byte keys) {
  for (int pos = 0 ; pos < 8 ; pos++) {
    if (keys & (1 << pos)) {
        toggleSwitch(pos);
    }
  }
}


// Toggle switch state for given key position
void toggleSwitch(int pos) {
  
  if(CURRENT_STATE & (1 << pos)) {
    Serial.print(F("Switch OFF "));
    CURRENT_STATE &= ~(1 << pos);
    digitalWrite(POWER_SWITCH_PIN[pos], HIGH);
  } else {
    Serial.print(F("Switch ON "));
    CURRENT_STATE |= (1 << pos);
    digitalWrite(POWER_SWITCH_PIN[pos], LOW);
  }

  updateLeds();
  Serial.println(pos);
}
  

// update led's status regarding Switches states
void updateLeds() {
  module.setLEDs(CURRENT_STATE);
}


// update display with powerconsumption and temperature
void updateDisplay(float currentConsumption, float temperature) {
  
    module.setDisplayToDecNumber(   1000000*round_to_dp(currentConsumption,2) +   // first 4 digits
                                    10*round_to_dp(temperature,1),                // last 3 digits
                                    0x42,                                         // dot in 2nd and 7th position
                                    true);
    
}


// round with given number of decimal numbers
float round_to_dp( float in_value, int decimal_place )
{
  float multiplier = powf( 10.0f, decimal_place );
  in_value = roundf( in_value * multiplier ) / multiplier;

  return in_value;
}


// toogle switch regarding chosen delay and order in settings.h 
void initSwitches() {
#ifdef INIT_1ST_SWITCH_DELAY
  delay(INIT_1ST_SWITCH_DELAY);
  toggleSwitch(INIT_1ST_SWITCH_POS);
#endif

#ifdef INIT_2ND_SWITCH_DELAY
  delay(INIT_2ND_SWITCH_DELAY);
  toggleSwitch(INIT_2ND_SWITCH_POS);
#endif

#ifdef INIT_3RD_SWITCH_DELAY
  delay(INIT_3RD_SWITCH_DELAY);
  toggleSwitch(INIT_3RD_SWITCH_POS);
#endif

#ifdef INIT_4TH_SWITCH_DELAY
  delay(INIT_4TH_SWITCH_DELAY);
  toggleSwitch(INIT_4TH_SWITCH_POS);
#endif

#ifdef INIT_5TH_SWITCH_DELAY
  delay(INIT_5TH_SWITCH_DELAY);
  toggleSwitch(INIT_5TH_SWITCH_POS);
#endif

#ifdef INIT_6TH_SWITCH_DELAY
  delay(INIT_6TH_SWITCH_DELAY);
  toggleSwitch(INIT_6TH_SWITCH_POS);
#endif

#ifdef INIT_7TH_SWITCH_DELAY
  delay(INIT_7TH_SWITCH_DELAY);
  toggleSwitch(INIT_7TH_SWITCH_POS);
#endif

#ifdef INIT_8TH_SWITCH_DELAY
  delay(INIT_8TH_SWITCH_DELAY);
  toggleSwitch(INIT_8TH_SWITCH_POS);
#endif
}


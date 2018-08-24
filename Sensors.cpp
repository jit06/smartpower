#include "Sensors.h"
#include "Settings.h"

// Rollng Data Stores
Storage current5vStorage[STORAGE_MAX_ENTRIES]; 
Storage current12vStorage[STORAGE_MAX_ENTRIES]; 
Storage powerStorage[STORAGE_MAX_ENTRIES];
Storage temperatureStorage[STORAGE_MAX_ENTRIES];

// Rolling data store counters
int currentStorageCounter     = 0;
int temperatureStorageCounter = 0;

// global interface to DHT sensor
SimpleDHT11 DHT11;

// read current consumption (amps), and store it when enought samples have been collected
void readCurrentConsumption(){
  static unsigned long lastRead = 0;
  static float value_current5v, value_power5v = 0;
  static float value_current12v, value_power12v = 0;
  static float sum5v,sum12v = 0;
  static int count = 0;
  float current5v,current12v;
  
  if(millis() - lastRead >= CUR_INTERVAL) {
    //Serial.print(F("Current analog read:"));
    //Serial.println(analogRead(ACS1_PIN));
    sum5v  += analogRead(ACS1_PIN);
    sum12v += analogRead(ACS2_PIN);
    lastRead = millis();
    count++;

    if(count == CUR_NB_SAMPLES) {
      // calculate current in mah
      current5v = getCurrentFromACSValue(sum5v, ACS712_NEUTRAL_5V);
      current12v = getCurrentFromACSValue(sum12v, ACS712_NEUTRAL_12V);
      
      // effective value : divide current by square root of 2, and x1000 to get Ah
      value_current5v = getEffectiveCurrentInAhFromCurrent(current5v);
      value_current12v = getEffectiveCurrentInAhFromCurrent(current12v);
      
      // calculate power in Watts
      value_power5v  = value_current5v * 5;
      value_power12v = value_current12v * 12;
      
      //Serial.print(F("Current 5v reads: "));
      //Serial.println(value_current5v);
      //Serial.print(F("Power 5v reads: "));
      //Serial.println(value_power5v);
      Serial.print(F("sum 5v reads: "));
      Serial.println(sum5v / CUR_NB_SAMPLES);
      Serial.print(F("sum 12v reads: "));
      Serial.println(sum12v / CUR_NB_SAMPLES);
      Serial.print(F("Current 12v reads: "));
      Serial.println(value_current12v);
      //Serial.print(F("Power 12v reads: "));
      //Serial.println(value_power12v);

      // clean up values
      if(value_current5v < 0) value_current5v = 0;
      if(value_current12v < 0) value_current12v = 0;
      if(value_power5v < 0) value_power5v = 0;
      if(value_power12v < 0) value_power12v = 0;

      // store values in history
      time_t timestamp = getTimestamp();
      storeValue(value_current5v, current5vStorage, &currentStorageCounter, timestamp);
      storeValue(value_current12v, current12vStorage, &currentStorageCounter, timestamp);
      storeValue(value_power5v+value_power12v, powerStorage, &currentStorageCounter, timestamp);

      // reset counters
      sum5v=0;
      sum12v=0;
      count=0;
    }
  }
}

float getCurrentFromACSValue(float acsvalue, int neutral) {
  return (float) ((acsvalue / CUR_NB_SAMPLES) - neutral)/1024*5/ACS_VPERAMP;
}

float getEffectiveCurrentInAhFromCurrent(float current) {
  return (current / 1.414) * 1000;
}

// read temperature (celcius dregress)and store it when enought samples have been collected
void readTemperature() {
  byte temp = 0;
  byte hum = 0; // not used here but needed to read...
  static float value = 0;
  static unsigned long lastRead = 0;
  static float sum = 0;
  static int count = 0;

  if(millis() - lastRead >= TEMP_INTERVAL) {
    int err = SimpleDHTErrSuccess;
    
    if ((err = DHT11.read(DTH11_PIN, &temp, &hum, NULL)) != SimpleDHTErrSuccess) {
      Serial.print(F("Read DHT11 failed, err=")); Serial.println(err);
    } else {
      sum+=temp;
      count++;

      if(count == TEMP_NB_SAMPLES || value == 0) {
        value = sum / float(TEMP_NB_SAMPLES);
        time_t timestamp = getTimestamp();
        storeValue(value, temperatureStorage, &temperatureStorageCounter, timestamp);
        sum=0;
        count=0;
        Serial.print(F("Temperature reads: "));
        Serial.println(value);
      }
    }
    
    lastRead = millis();  
  }
}


// Store values for history retreival (rolling buffer)
void storeValue(float value, Storage *store, int *counter, time_t timestamp) {

  // if maximum number of entries stored, start from first pos
  if(*counter >= STORAGE_MAX_ENTRIES) {
    *counter=0;
  }
  
  // store value in history rolling buffer
  store[*counter].value = value;
  store[*counter].timeStamp = timestamp;
  *counter+=1;

}


// retreive the lastest stored temperature (= most recent one)
float getLastTemperature() {
  return getLastStoredValue(temperatureStorage, &temperatureStorageCounter);
}


// retreive the lastest stored current (= most recent one)
float getLastCurrent5vConsumption() {
  return getLastStoredValue(current5vStorage, &currentStorageCounter);
}

// retreive the lastest stored current (= most recent one)
float getLastCurrent12vConsumption() {
  return getLastStoredValue(current12vStorage, &currentStorageCounter);
}

// retreive the lastest stored power (= most recent one)
float getLastPowerConsumption() {
  return getLastStoredValue(powerStorage, &currentStorageCounter);
}

// retreive the lastest stored value from a given store
float getLastStoredValue(Storage *store, int *counter) {
  return store[*counter - 1].value;
}

// retreive a specific Temperature value
Storage getStoredTemperature(int pos) {
  return temperatureStorage[pos];
}

// retreive a specific Current value
Storage getStored5vCurrent(int pos) {
  return current5vStorage[pos];
}

// retreive a specific Current value
Storage getStored12vCurrent(int pos) {
  return current12vStorage[pos];
}

// retreive a specific power value
Storage getStoredPower(int pos) {
  return powerStorage[pos];
}

#include "Sensors.h"
#include "Settings.h"

// Rollng Data Stores
Storage currentStorage[STORAGE_MAX_ENTRIES]; 
Storage temperatureStorage[STORAGE_MAX_ENTRIES];

// Rolling data store counters
int currentStorageCounter     = 0;
int temperatureStorageCounter = 0;

// global interface to DHT sensor
SimpleDHT11 DHT11;


// read current consumption (amps), and store it when enought samples have been collected
void readCurrentConsumption(){
  static unsigned long lastRead = 0;
  static float value = 0;
  static float sum = 0;
  static int count = 0;
  
  if(millis() - lastRead >= CUR_INTERVAL) {
    sum += analogRead(ACS1_PIN);
    lastRead = millis();
    count++;

    if(count == CUR_NB_SAMPLES) {
      value = (2.5 - (sum / CUR_NB_SAMPLES) * (5.0 / 1024.0)) / 0.185;
      Serial.print(F("Current reads: "));
      Serial.println(value);
      sum=0;
      count=0;
      if(value < 0) value = 0;
      storeValue(value, currentStorage, &currentStorageCounter);
    }
  }
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
        value = sum / TEMP_NB_SAMPLES;
        storeValue(value, temperatureStorage, &temperatureStorageCounter);
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
void storeValue(float value, Storage *store, int *counter) {

  // store value in history rolling buffer
  store[*counter].value = value;
  store[*counter].timeStamp = getTimestamp();
  *counter+=1;

  // if maximum numer of entries stored, start from first pos
  if(*counter >= STORAGE_MAX_ENTRIES) {
    *counter=0;
  }
}


// retreive the lastest stored temperature (= most recent one)
    float getLastTemperature() {
  return getLastStoredValue(temperatureStorage, &temperatureStorageCounter);
}


// retreive the lastest stored current (= most recent one)
float getLastCurrentConsumption() {
  return getLastStoredValue(currentStorage, &currentStorageCounter);
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
Storage getStoredCurrent(int pos) {
  return currentStorage[pos];
}



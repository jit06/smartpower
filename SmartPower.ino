#include "Timehandler.h"
#include "Controller.h"
#include "Sensors.h"
#include "Http.h"
#include "Settings.h"

byte MAC_ADDR[] = { MAC_ADDRESS };
IPAddress IP_ADDR(IP_ADDRESS_1, IP_ADDRESS_2, IP_ADDRESS_3, IP_ADDRESS_4);
extern bool TIME_SET;
unsigned long LAST_RETRY;

void setup() {
  
  Serial.begin(9600);
  
  // start ethernet connection and HTTP server
  Ethernet.begin(MAC_ADDR, IP_ADDR);
  Serial.print(F("Device IP: "));
  Serial.println(Ethernet.localIP());
  
  initController();
  initTimeHandler();
  initHttp();
  initSwitches();
}

void loop() {
  // handle buttons activity
  handleKeyPressed();

  // read sensors
  readTemperature();
  readCurrentConsumption();
  
  // refesh screen
  updateDisplay(getLastPowerConsumption(), getLastTemperature());
  
  // listen for HTTP connection
  handleHTTPRequest();

  // if time not set during init (eg: router not yet started, retry)
  if(timeStatus() == timeNotSet && (millis() - LAST_RETRY) >  NTP_RETRY_MILLIS) {
    initTimeHandler();
    LAST_RETRY = millis();
  }
}

#include "Http.h"
#include "Sensors.h"
#include "Controller.h"

extern IPAddress IP_ADDR;
EthernetServer HTTP_SERVER(HTTP_SERVER_PORT);

// start listening for HTTP requests
void initHttp() {
  HTTP_SERVER.begin();
}

// Parse request and tr to answer
void handleHTTPRequest() {
  
  EthernetClient client = HTTP_SERVER.available();
  
  if (client) {
    Serial.println(F("HTTP new client"));
    
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    char command[6];
    int nbChar=0;
    int nbLines = 0;
    int nbBlank = 0;

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
  
        if (c == '\n' && currentLineIsBlank) {

          Serial.print(F("HTTP Command:\""));
          Serial.print(command);
          Serial.println("\"");

          if (strcmp("/",command)==0) {
            Serial.println(F("HTTP Answer => default page"));
            sendDefaultHeaders(&client);
            sendHTMLContentType(&client);
            sendDefaultPageContent(&client);
            
          } else if (strcmp("/TEM",command)==0) {
            Serial.println(F("HTTP Answer => json temp page"));
            sendDefaultHeaders(&client);
            sendJsonContentType(&client);
            sendTemPageContent(&client);
            
          } else if (strcmp("/05V",command)==0) {
            Serial.println(F("HTTP Answer => json current page"));
            sendDefaultHeaders(&client);
            sendJsonContentType(&client);
            sendCur5vPageContent(&client);
          
          } else if (strcmp("/12V",command)==0) {
            Serial.println(F("HTTP Answer => json current page"));
            sendDefaultHeaders(&client);
            sendJsonContentType(&client);
            sendCur12vPageContent(&client);

          } else if (strcmp("/PWR",command)==0) {
            Serial.println(F("HTTP Answer => json current page"));
            sendDefaultHeaders(&client);
            sendJsonContentType(&client);
            sendPowerPageContent(&client);
            
          } else if (strcmp("/ALL",command)==0) {
            Serial.println(F("HTTP Answer => json all page"));
            sendDefaultHeaders(&client);
            sendJsonContentType(&client);
            sendAllPageContent(&client);
            
          } else if (strcmp("/HIS",command)==0) {
            Serial.println(F("HTTP Answer => json history"));
            sendDefaultHeaders(&client);
            sendJsonContentType(&client);
            sendHistoryPageContent(&client);
          
#ifdef HTTP_ALLOW_SWITCH_CONTROL
          } else if (strncmp("/TG",command,3)==0) {
            Serial.print(F("HTTP Answer empty + toggle switch : "));
            int pos = command[3]-49; // not using atoi() to avoid toggling switch 0 for any bad value
            Serial.println(pos);

            if(pos >= 0 && pos <= 7) {
              sendDefaultHeaders(&client);
              toggleSwitch(pos);
            }
            else {
              sendNotFoundHeaders(&client);
            }
#endif
          } else {
            sendNotFoundHeaders(&client);
          }

          break;
        }
      
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
          
          if(nbLines == 0) {
            command[nbChar]='\0';
            nbChar=0;
          }
          
          nbLines++;
          
        } else if (c != '\r') {
          // you've gotten a character on the current line
          if(c == ' '){
            nbBlank++;
          } else {
            if(nbLines == 0 && nbChar<6 && nbBlank == 1) {
              command[nbChar++]=c;
            }
          }
          currentLineIsBlank = false;
        }
      }
    }
    
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println(F("HTTP client disconnected"));
  }
}


void sendNotFoundHeaders(EthernetClient *client) {
  client->println(F("HTTP/1.1 404 NOT FOUND"));
  client->println(F("Connection: close"));  // the connection will be closed after completion of the response
}

void sendDefaultHeaders(EthernetClient *client) {
  client->println(F("HTTP/1.1 200 OK"));
  client->println(F("Connection: close"));  // the connection will be closed after completion of the response
}

void sendHTMLContentType(EthernetClient *client) {
  client->println(F("Content-Type: text/html"));
}

void sendJsonContentType(EthernetClient *client) {
  client->println(F("Content-Type: application/json"));
}

void sendAutoRefreshHeader(EthernetClient *client) {
  client->println(F("Refresh: 10"));
}

void sendDefaultPageContent(EthernetClient *client) {
  sendAutoRefreshHeader(client);
  client->println();
  client->println(F("<!DOCTYPE HTML>"));
  client->println(F("<html>"));
  client->println(F("<b>5V Current consumption: </b>"));
  client->println(getLastCurrent5vConsumption());
  client->println(F("&nbsp;&nbsp;&nbsp;&nbsp;"));
  client->println(F("<b>12V Current consumption: </b>"));
  client->println(getLastCurrent12vConsumption());
  client->println(F("&nbsp;&nbsp;&nbsp;&nbsp;"));
  client->println(F("<b>Power consumption: </b>"));
  client->println(getLastPowerConsumption());
  client->println(F("&nbsp;&nbsp;&nbsp;&nbsp;"));
  client->println(F("<b>Temperature: </b>"));
  client->println(getLastTemperature());
  client->println(F("</html>"));
}

void sendTemPageContent(EthernetClient *client) {
  client->println();
  client->print(F("{\"temp\":\""));
  client->print(getLastTemperature());
  client->print(F("\"}"));
}

void sendCur5vPageContent(EthernetClient *client) {
  client->println();
  client->print(F("{\"5v current consumption in Ah\":\""));
  client->print(getLastCurrent5vConsumption());
  client->print(F("\"}"));
}

void sendCur12vPageContent(EthernetClient *client) {
  client->println();
  client->print(F("{\"12v current consumption in Ah\":\""));
  client->print(getLastCurrent12vConsumption());
  client->print(F("\"}"));
}

void sendPowerPageContent(EthernetClient *client) {
  client->println();
  client->print(F("{\"Power consumption in Watt\":\""));
  client->print(getLastPowerConsumption());
  client->print(F("\"}"));
}

void sendAllPageContent(EthernetClient *client) {
  client->println();
  client->print(F("{\"temp\":\""));
  client->print(getLastTemperature());
  client->print(F("\",\"5V current consumption in Ah\":\""));
  client->print(getLastCurrent5vConsumption());
  client->print(F("\",\"12V current consumption in Ah\":\""));
  client->print(getLastCurrent12vConsumption());
  client->print(F("\",\"Power consumption in Watt\":\""));
  client->print(getLastPowerConsumption());
  client->print(F("\"}"));
}

void sendHistoryPageContent(EthernetClient *client) {
  client->println();
  client->println("{");
  client->println("\"values\":[");

  for(int i=0; i<STORAGE_MAX_ENTRIES; i++) {
    Storage entry = getStoredTemperature(i);
    client->print(F("{\"temp_ts\":\""));
    client->print(entry.timeStamp);
    client->print(F("\","));
    client->print(F("\"temp\":\""));
    client->print(entry.value);
    client->print(F("\"},"));

    entry = getStored5vCurrent(i);
    client->print(F("{\"current5v_ts\":\""));
    client->print(entry.timeStamp);
    client->print(F("\","));
    client->print(F("\"current5v\":\""));
    client->print(entry.value);
    client->print(F("\"},"));

    entry = getStored12vCurrent(i);
    client->print(F("{\"current12v_ts\":\""));
    client->print(entry.timeStamp);
    client->print(F("\","));
    client->print(F("\"current12v\":\""));
    client->print(entry.value);
    client->print(F("\"},"));

    entry = getStoredPower(i);
    client->print(F("{\"power_ts\":\""));
    client->print(entry.timeStamp);
    client->print(F("\","));
    client->print(F("\"power\":\""));
    client->print(entry.value);
    client->print(F("\"},"));
  }
  client->println("{\"end\":\"OK\"}");
  client->println("]");
  client->println("}");
}


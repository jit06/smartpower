#ifndef SMART_POWER_SETTINGS_H
#define SMART_POWER_SETTINGS_H

/******************************************************************************/
/* Sensors settings                                                           */
/******************************************************************************/
// set on which arduino pins sensors are plugged
#define DTH11_PIN A0
#define ACS1_PIN  A1
#define ACS2_PIN  A1

// set frequency at which a temperature sample is read (in miliseconds)
#define TEMP_INTERVAL   1000
// set the number of samples to read before publishing the average value
#define TEMP_NB_SAMPLES 10.0

// set frequency at which a current samples are read (in miliseconds)
#define CUR_INTERVAL    10
// set the number of samples to read before publishing average values
#define CUR_NB_SAMPLES  100.0

// How many published sensors entries to keep in history (limited by RAM)
#define STORAGE_MAX_ENTRIES 20
/******************************************************************************/



/******************************************************************************/
/* Controller settings                                                        */
/******************************************************************************/
#define TM1638_DATA_PIN A3
#define TM1638_CLCK_PIN A4
#define TM1638_STRB_PIN A5
#define TM1638_BRIGHTNESS 3
/******************************************************************************/



/******************************************************************************/
/* Ethernet settings                                                          */
/******************************************************************************/
// Ethernet Mac Address
#define MAC_ADDRESS 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED

// Ports to use for HTTP server and NTP requests
#define HTTP_SERVER_PORT 80
#define NTP_QUERY_PORT 8888

// comment out to disabe the possibility to toggle power switch from HTTP (eg: /TG1)
#define HTTP_ALLOW_SWITCH_CONTROL

// IP Address of the SmartPower device
#define IP_ADDRESS_1 192
#define IP_ADDRESS_2 168
#define IP_ADDRESS_3 20
#define IP_ADDRESS_4 200

// IP address of the NTP server to use
#define NTP_ADDRESS_4 254
#define NTP_ADDRESS_1 192
#define NTP_ADDRESS_2 168
#define NTP_ADDRESS_3 20
#define NTP_ADDRESS_4 254

// retry delay while NTP is not responding
#define NTP_RETRY_MILLIS 10000

// Time zone
#define TIME_ZONE 1
/******************************************************************************/



/******************************************************************************/
/* Relay (switches) settings                                                  */
/******************************************************************************/
// assign switches to pins. NOTE: pin 4 start HIGH where all others start LOW
#define PW_SWITCH1_PIN 2
#define PW_SWITCH2_PIN 3
#define PW_SWITCH3_PIN 4
#define PW_SWITCH4_PIN 5
#define PW_SWITCH5_PIN 6
#define PW_SWITCH6_PIN 7 
#define PW_SWITCH7_PIN 8
#define PW_SWITCH8_PIN 9

// Each defined line will Toggle Switch at start after the delay indicated.
// This allow to define a power on sequence for every plugged thing
// POS means position of the switch in the POWER_SWITCH_PIN variable (Controller.cpp)
#define INIT_1ST_SWITCH_DELAY 2000
#define INIT_1ST_SWITCH_POS   0
#define INIT_2ND_SWITCH_DELAY 1000
#define INIT_2ND_SWITCH_POS   1
#define INIT_3RD_SWITCH_DELAY 1000
#define INIT_3RD_SWITCH_POS   2
#define INIT_4TH_SWITCH_DELAY 1000
#define INIT_4TH_SWITCH_POS   3
#define INIT_5TH_SWITCH_DELAY 1000
#define INIT_5TH_SWITCH_POS   4
#define INIT_6TH_SWITCH_DELAY 1000
#define INIT_6TH_SWITCH_POS   5
#define INIT_7TH_SWITCH_DELAY 1000
#define INIT_7TH_SWITCH_POS   6
#define INIT_8TH_SWITCH_DELAY 1000
#define INIT_8TH_SWITCH_POS   7
/******************************************************************************/


#endif

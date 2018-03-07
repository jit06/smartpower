#ifndef SMART_POWER_TIMEHANDLER_H
#define SMART_POWER_TIMEHANDLER_H 

#include <Time.h>
#include <Ethernet.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>

#define NTP_PACKET_SIZE 48

void initTimeHandler();
time_t getNtpTime();
time_t getTimestamp();
void sendNTPpacket(IPAddress &address);

#endif

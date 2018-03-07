#ifndef SMART_POWER_HTTP_H
#define SMART_POWER_HTTP_H

#include "Settings.h"

#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>


void initHttp();
void handleHTTPRequest();
void sendDefaultHeaders(EthernetClient *client);
void sendNotFoundHeaders(EthernetClient *client);
void sendAutoRefreshHeader(EthernetClient *client);
void sendHTMLContentType(EthernetClient *client);
void sendJsonContentType(EthernetClient *client);
void sendDefaultPageContent(EthernetClient *client);
void sendTemPageContent(EthernetClient *client);
void sendCurPageContent(EthernetClient *client);
void sendAllPageContent(EthernetClient *client);
void sendHistoryPageContent(EthernetClient *client);

#endif

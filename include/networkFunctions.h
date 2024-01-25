#ifndef NETWORKFUNCTIONS_H
#define NETWORKFUNCTIONS_H

#include <HTTPClient.h>
#include <Arduino.h>
#include <displayFunctions.h>
#include <WiFiManager.h>

extern WiFiManager wm;

void wifiManagerSetup();
char *getLocalIP();
char *getPublicIP();
char *getSSID();

#endif // NETWORKFUNCTIONS_H

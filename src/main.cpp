#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include "displayFunctions.h"
#include "networkFunctions.h"

LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);
WiFiManager wm;
const uint8_t bootIP[] = {192, 168, 100, 1};
uint8_t screenNo = 0;
uint16_t screenChangeInterval = 5000;
uint32_t timePast = 0;
char publicIP[16] = "0.0.0.0"; // 16 bytes for "xxx.xxx.xxx.xxx\0"


void setup() {
    lcd.begin(16, 2);
    lcd.clear();
    Serial.begin(115200);
    wifiManagerSetup();
}

void loop() {
    wm.process();
    if (WiFi.status() == WL_CONNECTED) {
        char *localIP = getLocalIP();
        char *SSID = getSSID();
        if (strcmp(publicIP, "0.0.0.0") == 0) {
            char *newPublicIP = getPublicIP();
            Serial.println("No Public IP :(");
            strncpy(publicIP, newPublicIP, sizeof(publicIP) - 1);
        }
        if (millis() - timePast >= screenChangeInterval) {
            timePast = millis();
            switch (screenNo) {
            case 0:
                displayLCD("Local IP:", localIP);
                Serial.println(localIP);
                break;
            case 1:
                displayLCD("Public IP:", publicIP);
                Serial.println(publicIP);
                break;
            case 2:
                displayLCD("WiFi Network", SSID);
                break;
            }
            cycleScreen();
        }
    }
}
#include <Arduino.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include "displayFunctions.h"

LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);
WiFiManager wm;
const uint8_t bootIP[] = {192, 168, 100, 1};
uint8_t screenNo = 0;
uint16_t screenChangeInterval = 1000;
uint32_t timePast = 0;
char publicIP[16] = "0.0.0.0"; // 16 bytes for "xxx.xxx.xxx.xxx\0"

// Function declarations:

void wifiManagerSetup();
char *getLocalIP();
char *getPublicIP();
char *getSSID();

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

// Functions

void wifiManagerSetup() {
    displayLCD("ESP! Get My IP!", "mjharrison.org");
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    Serial.println("Entering WiFi config mode...");
    // wm.resetSettings();
    wm.setAPStaticIPConfig(IPAddress(bootIP[0], bootIP[1], bootIP[2], bootIP[3]), IPAddress(bootIP[0], bootIP[1], bootIP[2], bootIP[3]), IPAddress(255, 255, 255, 0));
    wm.setConfigPortalBlocking(false);
    wm.setConnectTimeout(30);
    // automatically connect using saved credentials if they exist
    // If connection fails it starts an access point with the specified name
    if (wm.autoConnect("AutoConnectAP", "password")) {
        Serial.println("connected :)");
    } else {
        displaySetupMode();
        Serial.println("Configportal running");
    }
}

char *getLocalIP() {
    static char localIP[16];
    IPAddress IPLocal = WiFi.localIP();
    sprintf(localIP, "%d.%d.%d.%d", IPLocal[0], IPLocal[1], IPLocal[2], IPLocal[3]);
    return localIP;
}

char *getPublicIP() {
    static char newPublicIP[16] = "0.0.0.0";
    Serial.println("Getting public IP...");

    if (WiFi.status() == WL_CONNECTED) { // Check the current connection status
        HTTPClient http;
        http.begin("https://api.ipify.org"); // Specify the URL
        int httpCode = http.GET();           // Make the request

        if (httpCode > 0) { // Check for the returning code
            String payload = http.getString();
            payload.toCharArray(newPublicIP, sizeof(newPublicIP));
            Serial.println(newPublicIP);
        } else {
            Serial.println("Error on HTTP request");
        }

        http.end(); // Free the resources
    }
    return newPublicIP;
}

char *getSSID() {
    static char SSID[33];
    uint8_t SSIDLength = WiFi.SSID().length();
    WiFi.SSID().toCharArray(SSID, SSIDLength + 1);
    return SSID;
}

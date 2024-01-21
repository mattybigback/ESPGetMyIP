#include <Arduino.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiManager.h>

LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);
WiFiManager wm;
const uint8_t bootIP[] = {192, 168, 100, 1};
char localIP[15]; // Char array to store human readable IP address
char publicIP[15];
char SSID[32];
uint8_t SSIDLength;
bool publicIPObtained = false;
uint8_t screenNo = 0;
uint16_t screenChangeInterval = 8000;
uint32_t timePast = 0;

// Function declarations:
void displaybootScreen();
void displaySetupMode();
void displayLocalIP();
void displayPublicIP();
void displaySSID();
void cycleScreen();

void wifiManagerSetup();
void updateNetworkDetails();
void getLocalIP();
void getPublicIP();
void getSSID();

void setup() {
    lcd.begin(16, 2);
    lcd.clear();
    Serial.begin(115200);
    wifiManagerSetup();
}

void loop() {
    wm.process();
    if (WiFi.status() == WL_CONNECTED) {
        updateNetworkDetails();
        if (millis() - timePast >= screenChangeInterval) {
            timePast = millis();

            switch (screenNo) {
            case 0:
                displayLocalIP();
                break;
            case 1:
                displayPublicIP();
                break;
            case 2:
                displaySSID();
                break;
            }
            cycleScreen();
        }
    }
}

// Functions

void displayBootScreen() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ESP! Get My IP!");
    lcd.setCursor(0, 1);
    lcd.print("mjharrison.co.uk");
}

void displaySetupMode() {
    char bootIPString[16];
    sprintf(bootIPString, "%d.%d.%d.%d", bootIP[0], bootIP[1], bootIP[2], bootIP[3]);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Setup Mode");
    lcd.setCursor(0, 1);
    lcd.print(bootIPString);
}

void displayLocalIP() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Local IP:");
    lcd.setCursor(0, 1);
    lcd.print(localIP);
}

void displayPublicIP() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Public IP:");
    lcd.setCursor(0, 1);
    lcd.print(publicIP);
}

void displaySSID() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Network:");
    lcd.setCursor(0, 1);
    lcd.print(SSID);
}

void cycleScreen() {
    if (screenNo < 2) {
        screenNo++;
    } else {
        screenNo = 0;
    }
}

void wifiManagerSetup() {
    displayBootScreen();
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

void updateNetworkDetails() {
    getPublicIP();
    getLocalIP();
    getSSID();
}

void getLocalIP() {
    sprintf(localIP, "%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
}

void getPublicIP() {
    if ((WiFi.status() == WL_CONNECTED) && publicIPObtained == false) { // Check the current connection status
        HTTPClient http;
        http.begin("https://api.ipify.org"); // Specify the URL
        int httpCode = http.GET();           // Make the request
        if (httpCode > 0) { // Check for the returning code
            String payload = http.getString();
            payload.toCharArray(publicIP, 15);
            publicIPObtained = true;
        } else {
            Serial.println("Error on HTTP request");
            publicIPObtained = false;
            strncpy(publicIP, "0.0.0.0", sizeof(publicIP) - 1);
        }
        http.end(); // Free the resources
    }
}

void getSSID() {
    SSIDLength = WiFi.SSID().length();
    WiFi.SSID().toCharArray(SSID, SSIDLength+1);
}

#include "networkFunctions.hpp"

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
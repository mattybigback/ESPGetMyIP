#include "displayFunctions.h"

void displayLCD(const char *line1, const char *line2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
}

void displaySetupMode() {
    char bootIPString[16];
    sprintf(bootIPString, "%d.%d.%d.%d", bootIP[0], bootIP[1], bootIP[2], bootIP[3]);
    displayLCD(bootIPString, "WiFi Setup Mode");
}

void cycleScreen() {
    if (screenNo < 2) {
        screenNo++;
    } else {
        screenNo = 0;
    }
}
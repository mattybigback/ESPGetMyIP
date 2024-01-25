#ifndef DISPLAYFUNCTIONS_H
#define DISPLAYFUNCTIONS_H

#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;
extern uint8_t screenNo;
extern uint16_t screenChangeInterval;
extern const uint8_t bootIP[];


void displayLCD(const char *line1, const char *line2);
void displaySetupMode();
void cycleScreen();


#endif // DISPLAYFUNCTIONS_H

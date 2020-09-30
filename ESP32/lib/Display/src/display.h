#ifndef display_h
#define display_h
#include "Arduino.h"
#include "segmentbit.h"


class Display
{

public:
    Display(uint8_t clockkpin, uint8_t latchpin, uint8_t datapin);
    Display(uint8_t clockkpin, uint8_t latchpin, uint8_t datapin[], uint8_t size);
    Display(uint8_t clockkpin, uint8_t latchpin, uint8_t datapin, int daisychain);
    void displayBit(uint8_t data);
    void displayBit(uint8_t data[]);
    void daisy(uint8_t data[]);
    void displayClock(int hour, int minute);
    void displayClock(String time);
    void displayClock(int time[]);
private:
    uint8_t clockPin, latchPin;
    uint8_t dataPin;
    uint8_t *dataPinArr;
    uint8_t arrSize;
    uint8_t num[10] = {num0, num1, num2, num3, num4, num5, num6, num7, num8, num9};
    int daisyChain;
};
#endif
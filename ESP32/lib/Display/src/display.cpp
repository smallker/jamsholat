#include "display.h"
#include "Arduino.h"

/// Use this for single pin configuration
Display::Display(uint8_t clockkpin, uint8_t latchpin, uint8_t datapin)
{
    clockPin = clockkpin;
    latchPin = latchpin;
    dataPin = datapin;
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
}

/// Use this for different datapin configuration
Display::Display(uint8_t clockkpin, uint8_t latchpin, uint8_t datapin[], uint8_t size)
{
    clockPin = clockkpin;
    latchPin = latchpin;
    dataPinArr = datapin;
    arrSize = size;
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    for (uint8_t i = 0; i < arrSize; i++)
    {
        pinMode(dataPinArr[i], OUTPUT);
    }
}

Display::Display(uint8_t clockkpin, uint8_t latchpin, uint8_t datapin, int daisychain)
{
    clockPin = clockkpin;
    latchPin = latchpin;
    dataPin = datapin;
    daisyChain = daisychain;
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
}

void Display::displayBit(uint8_t data[])
{
    for (uint8_t i = 0; i < 8; i++)
    {
        digitalWrite(latchPin, LOW);
        digitalWrite(clockPin, LOW);
        for (int j = 0; j < arrSize; j++)
        {
            digitalWrite(dataPinArr[j], (data[j] & (1 << i)));
        }
        digitalWrite(clockPin, HIGH);
        digitalWrite(clockPin, LOW);
        digitalWrite(latchPin, HIGH);
    }
}
void Display::displayBit(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        digitalWrite(latchPin, LOW);
        digitalWrite(clockPin, LOW);
        digitalWrite(dataPin, (data & (1 << i)));
        digitalWrite(clockPin, HIGH);
        digitalWrite(clockPin, LOW);
        digitalWrite(latchPin, HIGH);
    }
}

void Display::displayClock(int hour, int minute)
{
    uint8_t data[] = {num[hour / 10], num[hour % 10], num[minute / 10], num[minute % 10]};
    displayBit(data);
}

void Display::daisy(uint8_t data[])
{
    for (uint8_t i = 0; i < daisyChain; i++)
    {
        for (uint8_t j = 0; j < 8; j++)
        {
            // PORTD = 0b00000000; // all low
            // // digitalWrite(dataPin, (data[i] & (1 << j)));
            // bool bit = (data[i] & (0x01 << j));
            // bit == 0? PORTB = 0b00000000 : PORTB = 0b00000001;
            // PORTD = 0b00000100; // clock pin high
            // PORTD = 0b00000000; // clock pin low
            // PORTD = 0b00001000; // latch pin high
            digitalWrite(clockPin,LOW);
            digitalWrite(latchPin,LOW);
            digitalWrite(dataPin, (data[i] & (1 << j)));
            digitalWrite(clockPin,HIGH);
            digitalWrite(clockPin,LOW);
            digitalWrite(latchPin,HIGH);
        }
    }
}
void Display::turnOffAll()
{   
    for (uint8_t i = 0; i < daisyChain; i++)
    {
        for (uint8_t j = 0; j < 8; j++)
        {
            // PORTD = 0b00000000; // all low
            // // digitalWrite(dataPin, (data[i] & (1 << j)));
            // bool bit = (data[i] & (0x01 << j));
            // bit == 0? PORTB = 0b00000000 : PORTB = 0b00000001;
            // PORTD = 0b00000100; // clock pin high
            // PORTD = 0b00000000; // clock pin low
            // PORTD = 0b00001000; // latch pin high
            digitalWrite(clockPin,LOW);
            digitalWrite(latchPin,LOW);
            digitalWrite(dataPin, (0xFF & (1 << j)));
            digitalWrite(clockPin,HIGH);
            digitalWrite(clockPin,LOW);
            digitalWrite(latchPin,HIGH);
        }
    }
}
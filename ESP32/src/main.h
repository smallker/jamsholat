#include <Arduino.h>
#include <Wire.h>
#include <display.h>
#include <FreeRTOS.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <RTClib.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <config.h>
#include "ble.h"
#include <SPI.h>
#include <DMD.h>
#include "SystemFont5x7.h"
#include "Arial_black_16.h"

#include <HardwareSerial.h>
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
#define SSID "y"
#define PASS "11111111"
#define datapin 26
#define clockpin 25
#define latchpin 17
#define ledpin 0
#define num_ics 12
#define rx_pin 16
#define tx_pin 4
#define df_busy 2
#define H 0
#define M 1
Display daisy(clockpin, latchpin, datapin, num_ics);
RTC_DS3231 rtc;
TaskHandle_t task1;
TaskHandle_t tasknetwork;
TaskHandle_t taskble;
TaskHandle_t taskDFPlayer;
void scanDmd(void *parameter);
void displayClock(void *parameter);
void segment(void *parameter);
void getRtc(void *parameter);
void connectNetwork(void *parameter);
void bleService(void *parameter);
void dfplayer(void *paramater);
volatile int year, month, day, dayofweek, hour, minute, second;
int *dzuhur, *ashar, *maghrib, *isya, *imsak, *subuh, *dhuha;

WaktuSholat waktu;
BleSetup ble("jam sholat");
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
Config config;
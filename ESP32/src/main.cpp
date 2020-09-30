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
#include "main.h"
#define SSID "y"
#define PASS "11111111"
#define datapin 26
#define clockpin 25
#define latchpin 17
#define ledpin 16
#define num_ics 8
Display daisy(clockpin, latchpin, datapin, num_ics);
Display tes(clockpin, latchpin, datapin);
RTC_DS3231 rtc;
TaskHandle_t task1;
TaskHandle_t tasknetwork;
TaskHandle_t taskble;
// TaskHandle_t network;
void segment(void *parameter);
void getRtc(void *parameter);
void connectNetwork(void *parameter);
void bleService(void *parameter);
volatile int year, month, day, hour, minute, second;
int *dzuhur, *ashar, *maghrib, *isya, *imsak, *subuh;
WaktuSholat waktu;
BleSetup ble("jam sholat");
void setup()
{
  Serial.begin(115200);

  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // uint8_t n = 0b00000001;
  // uint8_t num[] = {num0, num1, num2, num3, num4, num5, num6, num7, num8, num9};

  // while (true)
  // {
  //   for(int i =0; i< sizeof(num); i ++){
  //     tes.displayBit(num[i]);
  //     delay(1000);
  //   }
  // }

  xTaskCreatePinnedToCore(
      segment,
      "7 Segment",
      5000,
      NULL,
      1,
      &task1,
      0);
  xTaskCreatePinnedToCore(
      getRtc,
      "GetRtc",
      2000,
      NULL,
      1,
      &task1,
      0);
  xTaskCreatePinnedToCore(
      connectNetwork,
      "Network",
      10000,
      NULL,
      1,
      &tasknetwork,
      1);
  xTaskCreatePinnedToCore(
      bleService,
      "BLE",
      5000,
      NULL,
      1,
      &taskble,
      1);
  Serial.println("Total heap: " + (String)ESP.getHeapSize());
  Serial.println("Free heap: " + (String)ESP.getFreeHeap());
  Serial.println("Total PSRAM: " + (String)ESP.getPsramSize());
  Serial.println("Free PSRAM: " + (String)ESP.getFreePsram());
}

void loop()
{
  delay(1000);
}

void segment(void *parameter)
{
  uint8_t num[] = {num0, num1, num2, num3, num4, num5, num6, num7, num8, num9};
  uint8_t _num[] = {_num0, _num1, _num2, _num3, _num4, _num5, _num6, _num7, _num8, _num9};
  uint8_t del = 1;
  Serial.println("Task created");
  Config config;
  dzuhur = config.schedule(waktu.dzuhur, waktu._dzuhur);
  ashar = config.schedule(waktu.ashar, waktu._ashar);
  maghrib = config.schedule(waktu.maghrib, waktu._maghrib);
  isya = config.schedule(waktu.isya, waktu._isya);
  imsak = config.schedule(waktu.imsak, waktu._imsak);
  subuh = config.schedule(waktu.subuh, waktu._subuh);
  uint8_t arr1[] = {0x01, num[dzuhur[0] / 10], 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, num4, 0x01, num[dzuhur[0] / 10]};    // segment pertama
  uint8_t arr2[] = {0x02, _num[dzuhur[0] % 10], 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, _num5, 0x02, _num[dzuhur[0] % 10]}; // segment kedua
  uint8_t arr3[] = {0x04, num[dzuhur[1] / 10], 0x04, 0x01, 0x04, 0x01, 0x04, 0x01, 0x04, num6, 0x04, num[dzuhur[1] / 10]};    // segment ketiga
  uint8_t arr4[] = {0x08, _num[dzuhur[1] % 10], 0x08, 0x01, 0x08, 0x01, 0x08, 0x01, 0x08, _num7, 0x08, _num[dzuhur[1] % 10]}; // segment keempat

  for (;;)
  {
    daisy.daisy(arr1);
    vTaskDelay(del / portTICK_PERIOD_MS);
    daisy.daisy(arr2);
    vTaskDelay(del / portTICK_PERIOD_MS);
    daisy.daisy(arr3);
    vTaskDelay(del / portTICK_PERIOD_MS);
    daisy.daisy(arr4);
    vTaskDelay(del / portTICK_PERIOD_MS);
  }
}

void getRtc(void *parameter)
{
  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  for (;;)
  {
    DateTime now = rtc.now();
    year = now.year();
    month = now.month();
    day = now.day();
    hour = now.hour();
    minute = now.minute();
    second = now.second();
    Serial.println((String)hour + ":" + (String)minute + ":" + (String)second);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void connectNetwork(void *parameter)
{
  pinMode(ledpin, OUTPUT);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(ledpin, HIGH);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    digitalWrite(ledpin, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Serial.print(". ");
  }
  Serial.println("Wifi Connected");
  for (;;)
  {
    digitalWrite(ledpin, HIGH);
    if (hour == 1 && minute == 39 && second < 10)
    {
      break;
    }
  }
  vTaskDelay(10000);
  Config config;
  config.setApiUrl(735, year, month, day);
  String server = config.getApiUrl();
  HTTPClient http;
  DynamicJsonDocument jadwal(1024);
  ble.stop();
  Serial.println("Ble stopped");
  http.begin(server);
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0)
  {
    String payload = http.getString();
    deserializeJson(jadwal, payload);
    Serial.println(payload);
    config.setschedule(payload);
  }
  http.end();
  ESP.restart();
}

void bleService(void *parameter)
{
  Config config;
  ble.init();
  ble.pServer->setCallbacks(new ServerCallbacks(ble.pCharacteristic, config));
  ble.pCharacteristic->setCallbacks(new ReceiveCallback());
  Serial.println("Ble started");
  for (;;)
  {
    vTaskDelay(1000);
  }
}
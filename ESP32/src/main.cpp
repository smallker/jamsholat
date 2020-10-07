#include "main.h"
void setup()
{
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, rx_pin, tx_pin);
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  // if (player.begin(Serial2))
  // {
  //   Serial.println("OK");
  //   player.volume(30);
  //   player.play(1);
  // }
  // else
  // {
  //   Serial.println("Connecting to DFPlayer Mini failed!");
  // }
  // while (true)
  // {
  //   Serial2.println("Hallo");
  //   delay(1000);
  // }

  xTaskCreatePinnedToCore(
      scanDmd,
      "scan SPI",
      5000,
      NULL,
      1,
      &task1,
      0);
  xTaskCreatePinnedToCore(
      displayClock,
      "display clock",
      2000,
      NULL,
      1,
      &task1,
      1);
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
  dhuha = config.schedule(waktu.dhuha, waktu._dhuha);

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
    // Serial.println((String)day + "-" + (String)month + "-" + (String)year);
    // Serial.println((String)hour + ":" + (String)minute + ":" + (String)second);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
void scanDmd(void *parameter)
{
  dmd.clearScreen(true);
  for (;;)
  {
    dmd.scanDisplayBySPI();
    vTaskDelay(5 / portTICK_PERIOD_MS);
  }
}
void displayClock(void *parameter)
{
  dmd.selectFont(System5x7);
  String h;
  String m;
  String monthOfYear[13] = {"Jan", "Feb", "Mar", "Apr", "Mei", "Jun", "Jul", "Agt", "Sep", "Okt", "Nov", "Des"};
  for (;;)
  {
    String date = String(day) + " " + monthOfYear[month - 1] + " " + (String)year;
    hour < 10 ? h = "0" + (String)hour : h = (String)hour;
    minute < 10 ? m = "0" + (String)minute : m = (String)minute;
    String hourminute = h + ":" + m;
    dmd.drawMarquee(date.c_str(), sizeof(date) - 1, (32 * DISPLAYS_ACROSS) - 1, 8);
    long start = millis();
    long timer = start;
    boolean ret = false;
    int del = 100;
    while (!ret)
    {
      if ((timer + del) < millis())
      {
        ret = dmd.stepMarquee(-1, 0);
        timer = millis();
        for (byte x = 0; x < DISPLAYS_ACROSS; x++)
        {
          for (byte y = 0; y < DISPLAYS_DOWN; y++)
          {
            dmd.drawString(2 + (32 * x), 0 + (16 * y), hourminute.c_str(), 5, GRAPHICS_NORMAL);
          }
        }
      }
    }
    vTaskDelay(1000);
  }
}
void connectNetwork(void *parameter)
{
  pinMode(ledpin, OUTPUT);
  Config config;
  WiFi.begin(config.ssid().c_str(), config.password().c_str());
  for (;;)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      digitalWrite(ledpin, LOW);
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    digitalWrite(ledpin, HIGH);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    if (hour == 1 && minute == 39 && second < 10)
      break;
    // if(WiFi.status() == WL_CONNECTED) break;
  }
  vTaskDelay(10000);
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
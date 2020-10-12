#include "main.h"
void setup()
{
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, rx_pin, tx_pin);
  mp3.player.begin(Serial2);
  // if (mp3.player.begin(Serial2))
  //   Serial.println("init dfplayer success");
  // else
  //   Serial.println("dfplayer fail");
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
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
  xTaskCreatePinnedToCore(
      dfplayer,
      "DFplayer",
      5000,
      NULL,
      1,
      &taskDFPlayer,
      1);
}

void loop()
{
  delay(1000);
}

void segment(void *parameter)
{
  int *on = config.autoOn();
  int *off = config.autoOff();
  uint8_t num[] = {num0, num1, num2, num3, num4, num5, num6, num7, num8, num9};
  uint8_t _num[] = {_num0, _num1, _num2, _num3, _num4, _num5, _num6, _num7, _num8, _num9};
  uint8_t del = 1;
  Serial.println("Task created");
  dzuhur = config.schedule(waktu.dzuhur, waktu._dzuhur);
  ashar = config.schedule(waktu.ashar, waktu._ashar);
  maghrib = config.schedule(waktu.maghrib, waktu._maghrib);
  isya = config.schedule(waktu.isya, waktu._isya);
  imsak = config.schedule(waktu.imsak, waktu._imsak);
  subuh = config.schedule(waktu.subuh, waktu._subuh);
  uint8_t arr1[] = {0x01, num[subuh[H] / 10], 0x01, num[imsak[H] / 10], 0x01, num[isya[H] / 10], 0x01, num[maghrib[H] / 10], 0x01, num[ashar[H] / 10], 0x01, num[dzuhur[H] / 10]};       // segment pertama
  uint8_t arr2[] = {0x02, _num[subuh[H] % 10], 0x02, _num[imsak[H] % 10], 0x02, _num[isya[H] % 10], 0x02, _num[maghrib[H] % 10], 0x02, _num[ashar[H] % 10], 0x02, _num[dzuhur[H] % 10]}; // segment kedua
  uint8_t arr3[] = {0x04, num[subuh[M] / 10], 0x04, num[imsak[M] / 10], 0x04, num[isya[M] / 10], 0x04, num[maghrib[M] / 10], 0x04, num[ashar[M] / 10], 0x04, num[dzuhur[M] / 10]};       // segment ketiga
  uint8_t arr4[] = {0x08, _num[subuh[M] % 10], 0x08, _num[imsak[M] % 10], 0x08, _num[isya[M] % 10], 0x08, _num[maghrib[M] % 10], 0x08, _num[ashar[M] % 10], 0x08, _num[dzuhur[M] % 10]}; // segment keempat

  Serial.println("Jadwal Mati => " + (String)off[0] + ":" + (String)off[1]);
  Serial.println("Jadwal Nyala => " + (String)on[0] + ":" + (String)on[1]);
  for (;;)
  {
    if (hour >= off[0] || hour <= on[0])
    {
      if (minute >= off[1] || minute <= on[1])
      {
        daisy.turnOffAll();
        vTaskDelay(2000 / portTICK_PERIOD_MS);
      }
    }
    else
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
    dayofweek = now.dayOfTheWeek();
    hour = now.hour();
    minute = now.minute();
    second = now.second();
    Serial.println((String)day + "-" + (String)month + "-" + (String)year);
    Serial.println((String)hour + ":" + (String)minute + ":" + (String)second);
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
  int *on = config.autoOn();
  int *off = config.autoOff();
  dmd.selectFont(System5x7);
  String h;
  String m;
  String monthOfYear[13] = {"Jan", "Feb", "Mar", "Apr", "Mei", "Jun", "Jul", "Agt", "Sep", "Okt", "Nov", "Des"};
  for (;;)
  {
    if (hour >= off[0] || hour <= on[0])
    {
      if (minute >= off[1] || minute <= on[1])
      {
        Serial.println("JAM MATI");
        dmd.clearScreen(true);
        vTaskDelay(100 / portTICK_PERIOD_MS);
      }
    }
    else
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
    }
  }
}
void connectNetwork(void *parameter)
{
  pinMode(ledpin, OUTPUT);
  String ssid = config.ssid();
  String password = config.password();
  WiFi.begin(ssid.c_str(), password.c_str());
  for (;;)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      digitalWrite(ledpin, LOW);
      WiFi.reconnect();
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    digitalWrite(ledpin, HIGH);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    if (hour == 0 && minute == 0 && second < 10)
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
  
  ble.init();
  ble.pServer->setCallbacks(new ServerCallbacks(ble.pCharacteristic, config));
  ble.pCharacteristic->setCallbacks(new ReceiveCallback());
  Serial.println("Ble started");
  for (;;)
  {
    vTaskDelay(1000);
  }
}

void dfplayer(void *parameter)
{
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  Iqomah iqo;
  Murrotal tartil;
  int *iqodzuhur = config.iqomah(iqo.dzuhur, dzuhur);
  int *iqoashar = config.iqomah(iqo.ashar, ashar);
  int *iqomaghrib = config.iqomah(iqo.maghrib, maghrib);
  int *iqoisya = config.iqomah(iqo.isya, isya);
  int *iqosubuh = config.iqomah(iqo.subuh, subuh);
  int *tartildzuhur = config.murrotal(tartil.dzuhur, dzuhur);
  int *tartilashar = config.murrotal(tartil.ashar, ashar);
  int *tartilmaghrib = config.murrotal(tartil.maghrib, maghrib);
  int *tartilisya = config.murrotal(tartil.isya, isya);
  int *tartilsubuh = config.murrotal(tartil.subuh, subuh);
  for (;;)
  {
    if (dayofweek != 5)
    {
      if (hour == dzuhur[H] || hour == ashar[H] || hour == maghrib[H] || hour == isya[H])
      {
        if (minute == dzuhur[M] || minute == ashar[M] || minute == maghrib[M] || minute == isya[M])
        {
          Serial.println("adzan");
          mp3.player.stop();
          mp3.player.play(mp3.adzan);
          vTaskDelay(60000 / portTICK_PERIOD_MS);
        }
      }
    }
    if (hour == subuh[H] && minute == subuh[M])
    {
      Serial.println("adzan subuh");
      mp3.player.stop();
      mp3.player.play(mp3.adzanSubuh);
      vTaskDelay(60000 / portTICK_PERIOD_MS);
    }
    if (hour == iqodzuhur[H] || hour == iqoashar[H] || hour == iqomaghrib[H] || hour == iqoisya[H] || hour == iqosubuh[H])
    {
      if (minute == iqodzuhur[M] || minute == iqoashar[M] || minute == iqomaghrib[M] || minute == iqoisya[M] || minute == iqosubuh[M])
      {
        Serial.println("Iqomah");
        mp3.player.stop();
        mp3.player.play(mp3.iqomah);
        vTaskDelay(60000 / portTICK_PERIOD_MS);
      }
    }
    if (hour == tartildzuhur[H] || hour == tartilashar[H] || hour == tartilmaghrib[H] || hour == tartilisya[H] || hour == tartilsubuh[H])
    {
      if (minute == tartildzuhur[M] || minute == tartilashar[M] || minute == tartilmaghrib[M] || minute == tartilisya[M] || minute == tartilsubuh[M])
      {
        Serial.println("murrotal");
        mp3.player.stop();
        mp3.player.play(mp3.murrotal);
        vTaskDelay(60000 / portTICK_PERIOD_MS);
      }
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
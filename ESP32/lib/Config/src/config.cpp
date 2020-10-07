#include "Arduino.h"
#include "config.h"

void Config::setschedule(String config)
{
    File file = SPIFFS.open("/sholat.txt", FILE_WRITE);
    if (!file)
    {
        Serial.println("config jadwal error");
        return;
    }
    if (file.print(config))
    {
        Serial.println("Config was written");
    }
    file.close();
}
String Config::getschedule(String param)
{
    File file = SPIFFS.open("/sholat.txt");
    String buff;
    if (!file)
    {
        Serial.println("config jadwal error");
    }
    while (file.available())
    {
        buff = file.readString();
    }
    file.close();
    DynamicJsonDocument jadwal(1024);
    DeserializationError error = deserializeJson(jadwal, buff);
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
    }
    return jadwal["jadwal"]["data"][param];
}

int *Config::schedule(String param, int arr[])
{
    int i = 0;
    String config = getschedule(param);
    int len = config.length() + 1;
    char buff[len];
    config.toCharArray(buff, len);
    char *jam = strtok(buff, ":");
    while (jam != NULL)
    {
        arr[i] = String(jam).toInt();
        jam = strtok(NULL, ":");
        i++;
    }
    return arr;
}

void Config::setApiUrl(int city, int year, int month, int day)
{
    File file = SPIFFS.open("/url.txt", FILE_WRITE);
    String mo, da;
    month < 10 ? mo = "0" + (String)month : mo = (String)month;
    day < 10 ? da = "0" + (String)day : da = (String)day;
    String config = "https://api.banghasan.com/sholat/format/json/jadwal/kota/" + (String)city + "/tanggal/" + (String)year + "-" + (String)mo + "-" + (String)da;
    if (!file)
    {
        Serial.println("config jadwal error");
        return;
    }
    if (file.print(config))
    {
        Serial.println("Config was written");
    }
    file.close();
}

String Config::getApiUrl()
{
    File file = SPIFFS.open("/url.txt");
    String buff;
    if (!file)
    {
        Serial.println("config jadwal error");
    }
    while (file.available())
    {
        buff = file.readString();
    }
    file.close();
    Serial.println("url dibaca");
    return buff;
}

String Config::rawSchedule()
{
    File file = SPIFFS.open("/sholat.txt");
    String buff;
    if (!file)
    {
        Serial.println("config jadwal error");
    }
    while (file.available())
    {
        buff = file.readString();
    }
    file.close();
    return buff;
}

String Config::rawConfig()
{
    File file = SPIFFS.open("/config.txt");
    String buff;
    if (!file)
    {
        Serial.println("config jadwal error");
    }
    while (file.available())
    {
        buff = file.readString();
    }
    file.close();
    return buff;
}

String Config::ssid()
{
    File file = SPIFFS.open("/wifi.txt");
    DynamicJsonDocument content(100);
    String buff;
    if (!file)
    {
        Serial.println("config jadwal error");
    }
    while (file.available())
    {
        buff = file.readString();
    }
    file.close();
    deserializeJson(content, buff);
    return content["ssid"];
}
String Config::password()
{
    File file = SPIFFS.open("/wifi.txt");
    DynamicJsonDocument content(100);
    String buff;
    if (!file)
    {
        Serial.println("config jadwal error");
    }
    while (file.available())
    {
        buff = file.readString();
    }
    file.close();
    deserializeJson(content, buff);
    return content["password"];
}

void Config::parseConfig(String param)
{
    DynamicJsonDocument content(1024);
    deserializeJson(content, param);
    int config = content["config"];
    if (config == 1)
    {
        Serial.println("Mengganti waktu");
        int minute = content["minute"];
        int hour = content["hour"];
        int day = content["day"];
        int month = content["month"];
        int year = content["year"];
        RTC.adjust(DateTime(year, month, day, hour, minute, 0));
    }
    if (config == 2)
    {
        File file = SPIFFS.open("/wifi.txt", FILE_WRITE);
        if (file.print(param))
            Serial.println("Mengganti password wifi");
        file.close();
        ESP.restart();
    }
    if (config == 3)
    {
        File file = SPIFFS.open("/auto.txt", FILE_WRITE);
        if (file.print(param))
            Serial.println("Set Scheduler");
        file.close();
        ESP.restart();
    }
    if (config == 4)
    {
        File file = SPIFFS.open("/adzan.txt", FILE_WRITE);
        if (file.print(param))
            Serial.println("Set adzan");
        file.close();
        ESP.restart();
    }

    if (config == 5)
    {
        File file = SPIFFS.open("/iqomah.txt", FILE_WRITE);
        if (file.print(param))
            Serial.println("Set iqomah");
        file.close();
        ESP.restart();
    }

    if (config == 6)
    {
        File file = SPIFFS.open("/murrotal.txt", FILE_WRITE);
        if (file.print(param))
            Serial.println("Set murrotal");
        file.close();
        ESP.restart();
    }

    if (config == 7)
    {
        File file = SPIFFS.open("/kota.txt", FILE_WRITE);
        if (file.print(param))
            Serial.println("Set kota");
        file.close();
        ESP.restart();
    }
}

int Config::kota()
{
    File file = SPIFFS.open("/kota.txt");
    DynamicJsonDocument content(100);
    String buff;
    if (!file)
    {
        Serial.println("config jadwal error");
    }
    while (file.available())
    {
        buff = file.readString();
    }
    file.close();
    deserializeJson(content, buff);
    return content["kota"];
}
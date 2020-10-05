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
String Config::retrieveConfig()
{
    String buff;
    StaticJsonDocument<1000> json;
    // json["jadwal"] = rawSchedule();
    json["config"] = rawConfig();
    serializeJson(json, buff);
    return buff;
}

void Config::parseConfig(String param)
{
    DynamicJsonDocument jadwal(1024);
    deserializeJson(jadwal, param);
    int config = jadwal["config"];
}
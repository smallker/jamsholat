#include "Arduino.h"
#include "config.h"

void Config::setschedule(String config)
{
    DynamicJsonDocument jadwal(1024);
    deserializeJson(jadwal, config);
    if (jadwal["status"] == "ok")
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
        File file = SPIFFS.open("/sholat.txt", FILE_WRITE);
        if (file.print(param))
            Serial.println("Set jadwal");
        file.close();
        ESP.restart();
    }

}

int Config::kota()
{
    File file = SPIFFS.open("/sholat.txt");
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
    return content["query"]["kota"];
}

int *Config::iqomah(int arr[], int param[])
{
    File file = SPIFFS.open("/iqomah.txt");
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
    int timer = content["timer"];
    param[1] + timer > 60 ? arr[0] = param[0] + 1 : arr[0] = param[0];
    param[1] + timer > 60 ? arr[1] = param[1] + timer - 60 : arr[1] = param[1] + timer;
    return arr;
}

int *Config::murrotal(int arr[], int param[])
{
    File file = SPIFFS.open("/murrotal.txt");
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
    int timer = content["timer"];
    param[1] < timer ? arr[0] = param[0] - 1 : arr[0] = param[0];
    param[1] < timer ? arr[1] = param[1] - timer + 60 : arr[1] = param[1] - timer;
    return arr;
}

int *Config::autoOn()
{
    File file = SPIFFS.open("/auto.txt");
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
    on[0] = content["on_hour"];
    on[1] = content["on_minute"];
    return on;
}
int *Config::autoOff()
{
    File file = SPIFFS.open("/auto.txt");
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
    off[0] = content["off_hour"];
    off[1] = content["off_minute"];
    return off;
}

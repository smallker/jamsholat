#ifndef _config_h
#define _config_h
#include "Arduino.h"
#include "ArduinoJson.h"
#include "SPIFFS.h"

struct WaktuSholat
{
    String dzuhur = "dzuhur";
    String ashar = "ashar";
    String maghrib = "maghrib";
    String isya = "isya";
    String imsak = "imsak";
    String subuh = "subuh";
    int _dzuhur[2];
    int _ashar[2];
    int _maghrib[2];
    int _isya[2];
    int _imsak[2];
    int _subuh[2];
};
class Config
{
private:
    String getschedule(String param);
public:
    void setclock(String config);
    void setschedule(String config);
    int *schedule(String param, int arr[]);
    void setApiUrl(int city, int year, int month, int day);
    String getApiUrl();
    String retrieveConfig();
};
#endif
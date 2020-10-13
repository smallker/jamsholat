#ifndef _config_h
#define _config_h
#include "Arduino.h"
#include "ArduinoJson.h"
#include "SPIFFS.h"
#include "RTClib.h"
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

struct Iqomah{
    int dzuhur[2];
    int ashar[2];
    int maghrib[2];
    int isya[2];
    int subuh[2];
};
struct Murrotal{
    int dzuhur[2];
    int ashar[2];
    int maghrib[2];
    int isya[2];
    int subuh[2];
};
// struct PowerOn{
//     volatile bool isOn = true;
// }power;

class Config
{
private:
    String getschedule(String param);
    String rawSchedule();
    String rawConfig();
    RTC_DS3231 RTC;
    int on[2], off[2];
public:
    
    void setclock(String config);
    void setschedule(String config);
    int *schedule(String param, int arr[]);
    void setApiUrl(int city, int year, int month, int day);
    void parseConfig(String json);
    String getApiUrl();
    String ssid();
    String password();
    int kota();
    int *iqomah(int arr[],int param[]);
    int *murrotal(int arr[],int param[]);
    int *autoOn();
    int *autoOff();
    bool isOn = true;
};
#endif
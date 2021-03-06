#ifndef _ble_setup_h
#define _ble_setup_h
#include "Arduino.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <config.h>
#include <DFRobotDFPlayerMini.h>
struct df
{
    DFRobotDFPlayerMini player;
    volatile bool isReady;
    int adzanSubuh = 1;
    int adzan = 2;
    int iqomah = 3;
    int murrotal = 4;
    int received = 5;
    int connected = 6;
} mp3;

struct Device{
    volatile bool isOn = true;
}device;

class BleSetup
{
private:
    const char service_uid[37] = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
    const char characteristic_uid[37] = "beb5483e-36e1-4688-b7f5-ea07361b26a8";
    String devicename;

public:
    BleSetup(String name);
    BLEServer *pServer;
    BLEService *pService;
    BLECharacteristic *pCharacteristic;
    void init();
    void stop();
};

BleSetup::BleSetup(String name)
{
    devicename = name;
}
void BleSetup::init()
{
    BLEDevice::init(devicename.c_str());
    pServer = BLEDevice::createServer();
    pService = pServer->createService(service_uid);
    pCharacteristic = pService->createCharacteristic(
        characteristic_uid,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);

    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(service_uid);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
}
void BleSetup::stop()
{
    pService->stop();
    pServer->removeService(pService);
    BLEDevice::deinit(false);
}
class ServerCallbacks : public BLEServerCallbacks
{
public:
    ServerCallbacks(BLECharacteristic *pCharacteristic)
    {
        characteristic = pCharacteristic;
    }
    ServerCallbacks(BLECharacteristic *pCharacteristic, Config _config)
    {
        characteristic = pCharacteristic;
        config = _config;
    }
    void onConnect(BLEServer *pServer)
    {
        Serial.println(F("ble: Connected ..."));
        mp3.player.stop();
        mp3.player.play(mp3.connected);
        characteristic->setValue(((String)config.kota()).c_str());
    };
    void onDisconnect(BLEServer *pServer)
    {
        Serial.println(F("ble : disconnected"));
    }

private:
    BLECharacteristic *characteristic;
    Config config;
};
class ReceiveCallback : public BLECharacteristicCallbacks
{
public:
    ReceiveCallback(Config _config)
    {
        config = _config;
        Serial.println("executed config callback");
    }
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string rxValue = pCharacteristic->getValue();
        Serial.println(rxValue.c_str());
        mp3.player.stop();
        mp3.player.play(mp3.received);
        if (rxValue.length() > 7)
        {
            config.parseConfig(rxValue.c_str());
        }
        else device.isOn = !device.isOn;
    }

private:
    Config config;
};
#endif
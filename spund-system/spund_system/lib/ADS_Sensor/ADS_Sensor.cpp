#include "Arduino.h"

#include "ADS_Sensor.h"

ADS_Sensor::ADS_Sensor() {}

ADS_Sensor::~ADS_Sensor() {}

void ADS_Sensor::begin()
{
    _ads = new Adafruit_ADS1115();
    _ads->begin(); // default address 0x48
}

void ADS_Sensor::begin(uint8_t address)
{
    _ads = new Adafruit_ADS1115();
    _ads->begin(address);
}

void ADS_Sensor::begin(uint8_t address, adsGain_t gain, uint8_t sda, uint8_t scl)
{
    _ads = new Adafruit_ADS1115();
    Wire.end();
    Wire.setPins(sda, scl);
    Wire.begin();
    _ads->begin(address);
    _ads->setGain(gain);
}

void ADS_Sensor::begin(uint8_t address, adsGain_t gain)
{
    _ads = new Adafruit_ADS1115();
    _ads->begin(address);
    _ads->setGain(gain);
}

uint16_t ADS_Sensor::getADC(uint8_t channel)
{
    return _ads->readADC_SingleEnded(channel);
}

float ADS_Sensor::getVolts(uint8_t channel)
{
    return _ads->computeVolts(_ads->readADC_SingleEnded(channel));
}
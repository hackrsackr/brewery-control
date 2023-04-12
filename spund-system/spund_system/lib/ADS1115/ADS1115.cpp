#include "Arduino.h"

#include "ADS1115.h"

ADS1115::ADS1115() {}

ADS1115::~ADS1115() {}

void ADS1115::begin()
{
    _ads = new Adafruit_ADS1115();
    _ads->begin(); // default address 0x48
}

void ADS1115::begin(uint8_t address)
{
    _ads = new Adafruit_ADS1115();
    _ads->begin(address);
}

void ADS1115::begin(uint8_t address, adsGain_t gain)
{
    _ads = new Adafruit_ADS1115();
    _ads->begin(address);
    _ads->setGain(gain);
}

uint16_t ADS1115::getADC(uint8_t channel)
{
    return _ads->readADC_SingleEnded(channel);
}

float ADS1115::getVolts(uint8_t channel)
{
    return _ads->computeVolts(_ads->readADC_SingleEnded(channel));
}
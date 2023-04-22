#include "Arduino.h"

#include "ADS_Sensor.h"
#include "config.h"

ADS_Sensor::ADS_Sensor() {}

ADS_Sensor::~ADS_Sensor() {}

void ADS_Sensor::begin(uint8_t address, adsGain_t gain, uint8_t sda, uint8_t scl)
{
    _ads = new Adafruit_ADS1115();
    Wire.end();
    Wire.setPins(sda, scl);
    Wire.begin();
    _ads->begin(address);
    _ads->setGain(gain);
}

uint16_t ADS_Sensor::readADC(uint8_t channel)
{
    return _ads->readADC_SingleEnded(channel);
}

double ADS_Sensor::readVolts(uint8_t channel)
{
    return _ads->computeVolts(_ads->readADC_SingleEnded(channel));
}

ADS_Pressure_Sensor::ADS_Pressure_Sensor() {}

ADS_Pressure_Sensor::~ADS_Pressure_Sensor() {}

void ADS_Pressure_Sensor::begin(uint8_t ads_address, adsGain_t ads_gain, uint8_t sda, uint8_t scl, uint8_t ads_chan, double offset_vs, uint8_t max_unit)
{
    ADS_Sensor::begin(ads_address, ads_gain, sda, scl);
    ads_channel = ads_chan;
    offset_volts = offset_vs;
    unit_max = max_unit;
}

double ADS_Pressure_Sensor::computePSI()
{
    return (readVolts(ads_channel) - offset_volts) * (unit_max / 4.0);
}
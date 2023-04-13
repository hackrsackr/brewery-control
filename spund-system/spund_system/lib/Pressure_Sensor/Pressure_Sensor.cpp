#include "Arduino.h"

#include "Pressure_Sensor.h"

Pressure_Sensor::Pressure_Sensor() {}

Pressure_Sensor::~Pressure_Sensor() {}

void Pressure_Sensor::begin(uint8_t address, adsGain_t gain, uint8_t sda, uint8_t scl, float offset_volts, uint8_t unit_max)
{
    ADS_Sensor::begin(address, gain, sda, scl);
    this->offset_volts = offset_volts;
    this->unit_max = unit_max;
}

float Pressure_Sensor::getPSI(uint8_t channel)
{
    return ((this->getVolts(channel) - offset_volts) * (unit_max / 4.0));
}
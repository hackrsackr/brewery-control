#pragma once

#include "ADS_Sensor.h"

class Pressure_Sensor : public ADS_Sensor
{
public:
    float offset_volts;
    uint8_t unit_max;

    Pressure_Sensor();
    ~Pressure_Sensor();

    void begin(uint8_t, adsGain_t, float, uint8_t);
    float getPSI(uint8_t);
};
#pragma once

#include "ADS_Sensor.h"

class Pressure_Sensor : public ADS_Sensor
{
public:
    double offset_volts;
    uint8_t unit_max;
    double volts;
    double psi;

    Pressure_Sensor();
    ~Pressure_Sensor();

    void begin(uint8_t, adsGain_t, uint8_t, uint8_t, double, uint8_t);
    double getPSI(uint8_t);
};
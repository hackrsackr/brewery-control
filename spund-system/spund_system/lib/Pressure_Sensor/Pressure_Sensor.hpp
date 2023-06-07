#pragma once

#include <Adafruit_ADS1X15.h>

class Pressure_Sensor : Adafruit_ADS1115
{
public:
    Pressure_Sensor(uint8_t, double, double, double);

    double getPSI(uint8_t chan);

private:
    uint8_t ads_channel;
    double volt_min;
    double volt_max;
    double psi_max;

    // uint8_t channel;

    // uint16_t readADC_SingleEnded(uint8_t);
};
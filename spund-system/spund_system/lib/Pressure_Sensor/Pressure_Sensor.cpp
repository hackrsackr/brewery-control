#include "Pressure_Sensor.hpp"

Pressure_Sensor::Pressure_Sensor(uint8_t ch, double minV, maxV, maxPSI)
{
    ads_channel = chan;
    volt_min = minV;
    volt_max = maxV;
    psi_max = maxPSI;
}
double getPSI()
{
    return (computeVolts(readADC_SingleEnded(ads_channel)))
}
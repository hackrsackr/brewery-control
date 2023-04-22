// #include "Arduino.h"

// #include "Pressure_Sensor.h"

// Pressure_Sensor::Pressure_Sensor() {}

// Pressure_Sensor::~Pressure_Sensor() {}

// void Pressure_Sensor::begin(uint8_t ads_address, adsGain_t ads_gain, uint8_t sda, uint8_t scl, uint8_t channel, double offset_vs, uint8_t max_unit)
// {
//     ADS_Sensor::begin(ads_address, ads_gain, sda, scl);
//     ads_channel = channel;
//     offset_volts = offset_vs;
//     unit_max = max_unit;
// }

// double Pressure_Sensor::computePSI()
// {
//     return (readVolts(ads_channel) - offset_volts) * (unit_max / 4.0);
// }

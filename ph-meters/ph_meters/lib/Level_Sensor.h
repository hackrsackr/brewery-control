#pragma once

#include <memory>

#include "Adafruit_ADS1X15.h"

typedef struct level_sensor_cfg_t
{
    const char *id;
    const uint8_t i2c_address;
    const uint8_t ads_channel;
    const uint16_t adc_offset;
} level_sensor_cfg_t;

class Level_Sensor
{
public:
    const char *id;
    uint8_t i2c_address, ads_channel;
    uint16_t adc, adc_offset;
    float volts, liters, gallons;

    Level_Sensor(level_sensor_cfg_t);

    void read();

private:
    std::shared_ptr<Adafruit_ADS1115> s_ads_;
    auto getADC() -> uint16_t;
    auto getVolts() -> float;
    auto voltsToLiters() -> float;
    auto voltsToGallons() -> float;
};
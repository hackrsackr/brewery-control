#pragma once

#include <memory>

#include "Adafruit_ADS1X15.h"

typedef enum {
    pH,
    o2,
    mV,
    conductivity
}meter_type_t;

typedef struct ma_meter_cfg_t
{
    const char *id;
    const char *description;
    const meter_type_t meter_type;
    const uint8_t i2c_address;
    const uint8_t ads_channel;
    const adsGain_t ads_gain;
} ma_meter_cfg_t;

class MA_Meter
{
public:
    const char *id;
    const char *description;
    meter_type_t meter_type;
    uint8_t i2c_address, ads_channel;
    adsGain_t ads_gain;
    const uint8_t mA_per_volt{4}, pH_per_volt{2}, ppmO2_per_volt{2};
    float volts, ph, ma, ppm;

    MA_Meter(ma_meter_cfg_t);

    void read();

private:
    std::shared_ptr<Adafruit_ADS1115> s_ads_;

    auto getVolts() -> float;
    auto voltsToMA() -> float;
    auto voltsToPH() -> float;
    auto voltsToPPM() -> float;
    auto voltsToPPB() -> uint16_t;
};
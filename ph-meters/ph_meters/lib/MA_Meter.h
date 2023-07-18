#pragma once

#include <memory>

#include "Adafruit_ADS1X15.h"

typedef struct ma_meter_cfg_t
{
    const char *id;
    const uint8_t i2c_address;
    const uint8_t ads_channel;
} ma_meter_cfg_t;

class MA_Meter
{
public:
    const char *id;
    uint8_t i2c_address, ads_channel;
    const uint8_t mA_per_volt{4}, pH_per_volt{2};
    uint16_t adc;
    float volts, ph, ma;
    adsGain_t ads_gain{GAIN_TWOTHIRDS};

    MA_Meter(ma_meter_cfg_t);

    void read();

private:
    std::shared_ptr<Adafruit_ADS1115> s_ads_;

    auto getADC() -> uint16_t;
    auto getVolts() -> float;
    auto voltsToMA() -> float;
    auto voltsToPH() -> float;
};
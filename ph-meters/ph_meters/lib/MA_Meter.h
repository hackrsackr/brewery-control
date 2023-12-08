#pragma once

#include <memory>

#include "Adafruit_ADS1X15.h"

typedef struct ma_meter_cfg_t
{
    const char *id;
    const char *measurement;
    const uint8_t i2c_address;
    const uint8_t ads_channel;
    const adsGain_t ads_gain;
    const uint8_t input_LRV;
    const uint8_t input_URV;
    const uint8_t output_LRV;
    const uint8_t output_URV;
} ma_meter_cfg_t;

class MA_Meter
{
public:
    const char *id;
    const char *measurement;
    float volts;
    float ma;
    float output;

    MA_Meter(ma_meter_cfg_t);

    void read();

private:
    uint8_t i2c_address;
    uint8_t ads_channel;
    adsGain_t ads_gain;
    uint8_t input_LRV;
    uint8_t input_URV;
    uint8_t output_LRV;
    uint8_t output_URV;
    const uint8_t mA_per_volt{4};

    std::shared_ptr<Adafruit_ADS1115> s_ads_;

    auto getVolts() -> float;
    auto voltsToMA() -> float;
    auto voltsToPH() -> float;
    auto voltsToPPM() -> float;
    auto voltsToPPB() -> uint16_t;
    auto voltsToLPM() -> float;
    auto voltsToTempC() -> float;
};

// class FlowMeter : MA_Meter {
// public:
//     FlowMeter();
// };
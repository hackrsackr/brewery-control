#pragma once

#include <memory>

#include "Adafruit_ADS1X15.h"

typedef enum
{
    pH,
    o2,
    mV,
    conductivity,
    flow
} meter_type_t;

typedef struct ma_meter_cfg_t
{
    const meter_type_t meter_type;
    const char *id;
    const char *measurement;
    const uint8_t i2c_address;
    const uint8_t ads_channel;
    const adsGain_t ads_gain;
} ma_meter_cfg_t;

class MA_Meter
{
public:
    meter_type_t meter_type;
    const char *id;
    const char *measurement;
    uint8_t i2c_address, ads_channel;
    adsGain_t ads_gain;
    const uint8_t mA_per_volt{4}, pH_per_volt{2}, ppmO2_per_volt{2}, lpm_per_volt{6}, tempC_per_volt{20};
    float volts, ph, ma, ppm, lpm, tempC;

    MA_Meter(ma_meter_cfg_t);

    void read();

private:
    std::shared_ptr<Adafruit_ADS1115> s_ads_;
    std::unique_ptr<Adafruit_ADS1115> u_ads_;

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
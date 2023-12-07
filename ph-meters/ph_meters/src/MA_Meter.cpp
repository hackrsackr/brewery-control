#include "MA_Meter.h"

#include "Adafruit_ADS1X15.h"

MA_Meter::MA_Meter(ma_meter_cfg_t cfg)
{
    meter_type = cfg.meter_type;
    id = cfg.id;
    measurement = cfg.measurement;
    i2c_address = cfg.i2c_address;
    ads_channel = cfg.ads_channel;
    ads_gain = cfg.ads_gain;

    s_ads_ = std::make_shared<Adafruit_ADS1115>();
    // u_ads_{std::make_unique<Adafruit_ADS1115>()};

    s_ads_->begin(i2c_address);
    s_ads_->setGain(ads_gain);
}

auto MA_Meter::getVolts() -> float
{
    return volts = s_ads_->computeVolts(s_ads_->readADC_SingleEnded(ads_channel));
}

auto MA_Meter::voltsToMA() -> float
{
    return ma = volts * mA_per_volt;
}

auto MA_Meter::voltsToPH() -> float
{
    return ph = volts * pH_per_volt;
}

auto MA_Meter::voltsToPPM() -> float
{
    return ppm = volts * ppmO2_per_volt;
}

auto MA_Meter::voltsToPPB() -> uint16_t
{
    ppm = volts * ppmO2_per_volt;
    return ppm * 1000;
}

auto MA_Meter::voltsToLPM() -> float
{
    lpm = (volts > 1) ? volts * lpm_per_volt - lpm_per_volt : 0;
    return lpm;
}
auto MA_Meter::voltsToTempC() -> float
{
    tempC = (volts > 1) ? volts * tempC_per_volt - tempC_per_volt : 0;
    return tempC;
}

void MA_Meter::read()
{
    getVolts();
    voltsToMA();
    voltsToPH();
    voltsToPPB();
    voltsToLPM();
    voltsToTempC();
}
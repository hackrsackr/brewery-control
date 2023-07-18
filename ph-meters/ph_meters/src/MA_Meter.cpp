#include "MA_Meter.h"

#include "Adafruit_ADS1X15.h"

MA_Meter::MA_Meter(ma_meter_cfg_t cfg)
{
    id = cfg.id;
    i2c_address = cfg.i2c_address;
    ads_channel = cfg.ads_channel;

    s_ads_ = std::make_shared<Adafruit_ADS1115>();
    s_ads_->begin(i2c_address);
    s_ads_->setGain(ads_gain);
}

auto MA_Meter::getADC() -> uint16_t
{
    return adc = s_ads_->readADC_SingleEnded(ads_channel);
}

auto MA_Meter::getVolts() -> float
{
    return volts = s_ads_->computeVolts(getADC());
}

auto MA_Meter::voltsToMA() -> float
{
    return ma = volts * mA_per_volt;
}

auto MA_Meter::voltsToPH() -> float
{
    return ph = volts * pH_per_volt;
}

void MA_Meter::read()
{
    getVolts();
    voltsToMA();
    voltsToPH();
}
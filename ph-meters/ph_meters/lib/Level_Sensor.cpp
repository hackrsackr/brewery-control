#include "Level_Sensor.h"

Level_Sensor::Level_Sensor(level_sensor_cfg_t cfg)
{
    id = cfg.id;
    i2c_address = cfg.i2c_address;
    ads_channel = cfg.ads_channel;
    adc_offset = cfg.adc_offset;

    s_ads_ = std::make_shared<Adafruit_ADS1115>();
    s_ads_->begin(i2c_address);
    s_ads_->setGain(GAIN_TWOTHIRDS);
}

auto Level_Sensor::getADC() -> uint16_t
{
    return adc = s_ads_->readADC_SingleEnded(ads_channel);
}

auto Level_Sensor::getTrimmedADC() -> uint16_t
{
    return adc_trimmed = s_ads_->readADC_SingleEnded(ads_channel) - adc_offset;
}

auto Level_Sensor::getVolts() -> float
{
    return volts = s_ads_->computeVolts(getTrimmedADC());
}

auto Level_Sensor::voltsToLiters() -> float
{
    auto liters_per_volt{9};
    return liters = volts * liters_per_volt;
}

auto Level_Sensor::voltsToGallons() -> float
{
    auto gallons_per_volt{3};
    return gallons = volts * gallons_per_volt;
}

void Level_Sensor::read()
{
    volts = getVolts();
    liters = voltsToLiters();
    gallons = voltsToGallons();
}
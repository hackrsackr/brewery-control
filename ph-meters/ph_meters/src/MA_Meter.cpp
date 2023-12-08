#include "MA_Meter.h"

#include "Adafruit_ADS1X15.h"

MA_Meter::MA_Meter(ma_meter_cfg_t cfg)
{
    id = cfg.id;
    measurement = cfg.measurement;
    i2c_address = cfg.i2c_address;
    ads_channel = cfg.ads_channel;
    ads_gain = cfg.ads_gain;
    input_LRV = cfg.input_LRV;
    input_URV = cfg.input_URV;
    output_LRV = cfg.output_LRV;
    output_URV = cfg.output_URV;

    s_ads_ = std::make_shared<Adafruit_ADS1115>();

    s_ads_->begin(i2c_address);
    s_ads_->setGain(ads_gain);
}

void MA_Meter::read()
{
    volts = s_ads_->computeVolts(s_ads_->readADC_SingleEnded(ads_channel));
    ma = volts * mA_per_volt;
    output = (ma - input_LRV) / (input_URV - input_LRV) * (output_URV - output_LRV);
}
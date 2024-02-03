#include "ADS_Sensor.h"
#include "config.h"

ADS_Sensor::ADS_Sensor() {}

ADS_Sensor::~ADS_Sensor() {}

void ADS_Sensor::begin(uint8_t address, adsGain_t gain, uint8_t sda, uint8_t scl, uint8_t ads_chan)
{
    s_ads = std::make_shared<Adafruit_ADS1115>();
    Wire.begin(sda, scl);
    s_ads->begin(address);
    s_ads->setGain(gain);
    ads_channel = ads_chan;
}

uint16_t ADS_Sensor::readADC()
{
    return s_ads->readADC_SingleEnded(ads_channel);
}

double ADS_Sensor::readVolts()
{
    return s_ads->computeVolts(s_ads->readADC_SingleEnded(ads_channel));
}

// ########################################

ADS_Pressure_Sensor::ADS_Pressure_Sensor() {}

void ADS_Pressure_Sensor::begin(
    uint8_t ads_address,
    adsGain_t ads_gain,
    uint8_t sda,
    uint8_t scl,
    uint8_t ads_chan,
    double min_vs,
    double max_vs,
    double max_unit,
    double offset)
{
    ADS_Sensor::begin(
        ads_address,
        ads_gain,
        sda,
        scl,
        ads_chan);

    volt_min = min_vs;
    volt_max = max_vs;
    psi_max = max_unit;
    sensor_offset_volts = offset;
}

double ADS_Pressure_Sensor::computePSI()
{
    return (readVolts() - volt_min + sensor_offset_volts) * (psi_max / (volt_max - volt_min));
}

// ########################################

ADS_Level_Sensor::ADS_Level_Sensor() {}

void ADS_Level_Sensor::begin(uint8_t ads_address, adsGain_t ads_gain, uint8_t sda, uint8_t scl, uint8_t ads_chan, double min_vs, double max_vs, double max_unit)
{
    ADS_Sensor::begin(
        ads_address,
        ads_gain,
        sda,
        scl,
        ads_chan);

    volt_min = min_vs;
    volt_max = max_vs;
    liters_max = max_unit;
}

double ADS_Level_Sensor::computeLiters()
{
    return (readVolts() - volt_min) * (liters_max / 4.0);
}
// ########################################

ADS_MA_Meter::ADS_MA_Meter() {}

void ADS_MA_Meter::begin(uint8_t ads_address, adsGain_t ads_gain, uint8_t sda, uint8_t scl, uint8_t ads_chan, double min_vs, double max_vs, double max_mAs)
{
    ADS_Sensor::begin(
        ads_address,
        ads_gain,
        sda,
        scl,
        ads_chan);

    volt_min = min_vs;
    volt_max = max_vs;
    mA_max = max_mAs;
}

double ADS_MA_Meter::computeMilliAmps()
{
    return readVolts() * 4.0;
}

ADS_PH_Meter::ADS_PH_Meter() {}

void ADS_PH_Meter::begin(uint8_t ads_address, adsGain_t ads_gain, uint8_t sda, uint8_t scl, uint8_t ads_chan, double min_vs, double max_vs, double max_mAs)
{
    ADS_Sensor::begin(
        ads_address,
        ads_gain,
        sda,
        scl,
        ads_chan);

    volt_min = min_vs;
    volt_max = max_vs;
    mA_max = max_mAs;
}

double ADS_PH_Meter::computePH()
{
    return readVolts() * 2.0;
}

double ADS_PH_Meter::computeORP()
{
    return 400 - (computeMilliAmps() * 28.57);
}

ADS_Conductivity_Meter::ADS_Conductivity_Meter() {}

void ADS_Conductivity_Meter::begin(uint8_t ads_address, adsGain_t ads_gain, uint8_t sda, uint8_t scl, uint8_t ads_chan, double min_vs, double max_vs, double max_mAs)
{
    ADS_Sensor::begin(
        ads_address,
        ads_gain,
        sda,
        scl,
        ads_chan);

    volt_min = min_vs;
    volt_max = max_vs;
    mA_max = max_mAs;
}

double ADS_Conductivity_Meter::computeConductivity()
{
    return computeMilliAmps() * 0.2; // WIP
}
// ########################################

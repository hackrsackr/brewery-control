#pragma once

#include <Adafruit_ADS1X15.h>

#include "config.h"

#define ADS1115_ADDRESS1 (0x48) // ADDR -> GND
#define ADS1115_ADDRESS2 (0x49) // ADDR -> VCC
#define ADS1115_ADDRESS3 (0x4a) // ADDR -> SDA
#define ADS1115_ADDRESS4 (0x4b) // ADDR -> SCL

#define ADS_FULLSCALE 32767

class ADS_Sensor
{
protected:
    // uint16_t readADC();

public:
    ADS_Sensor();
    ~ADS_Sensor();

    void begin(
        uint8_t address,
        adsGain_t gain,
        uint8_t sda,
        uint8_t scl,
        uint8_t ads_chan);

    void setChannel(uint8_t ads_chan);

    double readVolts();

private:
    Adafruit_ADS1115 *_ads;
    uint8_t ads_address;
    uint8_t ads_channel;

    uint16_t readADC();
};

class ADS_MA_Meter : public ADS_Sensor
{
public:
    ADS_MA_Meter();
    ~ADS_MA_Meter();

    void begin(uint8_t address, adsGain_t gain, uint8_t sda, uint8_t scl, uint8_t ads_chan, double offset_vs, double max_unit);
    double computeMilliAmps();
};

class ADS_PH_Meter : public ADS_MA_Meter
{
public:
    ADS_PH_Meter();
    ~ADS_PH_Meter();

    void begin(uint8_t address, adsGain_t gain, uint8_t sda, uint8_t scl, uint8_t ads_chan, double offset_vs, double max_unit);
    double computePH();
    double computeORP();
};

class ADS_Conductivity_Meter : public ADS_MA_Meter
{
public:
    ADS_Conductivity_Meter();
    ~ADS_Conductivity_Meter();

    void begin(uint8_t address, adsGain_t gain, uint8_t sda, uint8_t scl, uint8_t ads_chan, double offset_vs, double max_unit);
    double computeConductivity();
};

class ADS_Pressure_Sensor : public ADS_Sensor
{
public:
    double volt_min;
    double volt_max;
    double psi_max;

    ADS_Pressure_Sensor();
    ~ADS_Pressure_Sensor();

    void begin(
        uint8_t address,
        adsGain_t gain,
        uint8_t sda,
        uint8_t scl,
        uint8_t ads_chan,
        double min_vs,
        double max_vs,
        double max_unit);

    double computePSI();
};

class ADS_Level_Sensor : public ADS_Sensor
{
public:
    uint8_t volt_min;
    uint8_t volt_max;
    double liters_max;

    ADS_Level_Sensor();
    ~ADS_Level_Sensor();

    void begin(
        uint8_t address,
        adsGain_t gain,
        uint8_t sda,
        uint8_t scl,
        uint8_t ads_chan,
        double min_vs,
        double max_vs,
        double max_unit);

    double computeLiters();
};
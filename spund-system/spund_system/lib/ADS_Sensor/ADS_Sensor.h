#pragma once

#include <Adafruit_ADS1X15.h>

#include <memory>

constexpr auto ADS1115_ADDRESS1 = 0x48; // ADDR -> GND
constexpr auto ADS1115_ADDRESS2 = 0x49; // ADDR -> VCC
constexpr auto ADS1115_ADDRESS3 = 0x4a; // ADDR -> SDA
constexpr auto ADS1115_ADDRESS4 = 0x4b; // ADDR -> SCL

typedef struct ads_sensor_cfg_t
{
    uint8_t ads_addr;
    adsGain_t ads_gain;
    uint8_t i2c_sda;
    uint8_t i2c_scl;
    uint8_t ads_channel;
    float input_low_range;
    float input_high_range;
    float output_low_range;
    float output_high_range;
    float offset_volts;
} ads_sensor_cfg_t;

class ADS_Sensor
{
public:
    std::shared_ptr<Adafruit_ADS1115> s_ads;

    ADS_Sensor();
    ~ADS_Sensor();

    uint8_t ads_channel;
    float input_low_range;
    float input_high_range;
    float output_low_range;
    float output_high_range;
    float offset_volts;

    void begin(
        uint8_t address,
        adsGain_t gain,
        uint8_t sda,
        uint8_t scl,
        uint8_t ads_chan);

    double readVolts();
    uint16_t readADC();

private:
};

class ADS_Pressure_Sensor : public ADS_Sensor
{
public:
    double volt_min;
    double volt_max;
    double unit_max;
    double offset_volts;

    ADS_Pressure_Sensor();

    void begin(
        uint8_t address,
        adsGain_t gain,
        uint8_t sda,
        uint8_t scl,
        uint8_t ads_chan,
        double input_low_range,
        double input_high_range,
        double output_high_range,
        double offset_volts);

    double computePSI();
};

// class ADS_Level_Sensor : public ADS_Pressure_Sensor
// {
// public:
//     double volt_min;
//     double volt_max;
//     double liters_max;

//     ADS_Level_Sensor();

//     void begin(
//         uint8_t address,
//         adsGain_t gain,
//         uint8_t sda,
//         uint8_t scl,
//         uint8_t ads_chan,
//         double min_vs,
//         double max_vs,
//         double max_unit);

//     double computeLiters();
// };

// class ADS_MA_Meter : public ADS_Sensor
// {
// public:
//     double volt_min;
//     double volt_max;
//     double mA_max;

//     ADS_MA_Meter();

//     void begin(
//         uint8_t address,
//         adsGain_t gain,
//         uint8_t sda,
//         uint8_t scl,
//         uint8_t ads_chan,
//         double min_vs,
//         double max_vs,
//         double max_unit);
//     double computeMilliAmps();
// };

// class ADS_PH_Meter : public ADS_MA_Meter
// {
// public:
//     ADS_PH_Meter();

//     void begin(
//         uint8_t address,
//         adsGain_t gain,
//         uint8_t sda,
//         uint8_t scl,
//         uint8_t ads_chan,
//         double min_vs,
//         double max_vs,
//         double max_unit);

//     double computePH();
//     double computeORP();
// };

// class ADS_Conductivity_Meter : public ADS_MA_Meter
// {
// public:
//     ADS_Conductivity_Meter();

//     void begin(
//         uint8_t address,
//         adsGain_t gain,
//         uint8_t sda,
//         uint8_t scl,
//         uint8_t ads_chan,
//         double min_vs,
//         double max_vs,
//         double max_unit);

//     double computeConductivity();
// };

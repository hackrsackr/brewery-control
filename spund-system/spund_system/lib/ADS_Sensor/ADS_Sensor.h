#pragma once

#include <Adafruit_ADS1X15.h>

#define ADS1115_ADDRESS1 (0x48) // ADDR -> GND
#define ADS1115_ADDRESS2 (0x49) // ADDR -> VCC
#define ADS1115_ADDRESS3 (0x4a) // ADDR -> SDA
#define ADS1115_ADDRESS4 (0x4b) // ADDR -> SCL

class ADS_Sensor
{
public:
    // constructor
    ADS_Sensor();
    ~ADS_Sensor();

    // initializer
    void begin(uint8_t, adsGain_t, uint8_t, uint8_t);

    // get methods
    uint16_t readADC(uint8_t);
    double readVolts(uint8_t);

private:
    Adafruit_ADS1115 *_ads;
};

class ADS_Pressure_Sensor : public ADS_Sensor
{
public:
    double offset_volts;
    uint8_t unit_max;
    uint8_t ads_channel;

    ADS_Pressure_Sensor();
    ~ADS_Pressure_Sensor();

    void begin(uint8_t, adsGain_t, uint8_t, uint8_t, uint8_t, double, uint8_t);
    double computePSI();
};

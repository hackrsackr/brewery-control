#pragma once
#include <Adafruit_ADS1X15.h>

class ADS_Sensor
{
    // protected:
    // TwoWire *Wire2;

public:
    // constructor
    ADS_Sensor();
    ~ADS_Sensor();

    // initializer
    void begin();
    void begin(uint8_t);
    void begin(uint8_t, adsGain_t, uint8_t, uint8_t);
    void begin(uint8_t, adsGain_t);

    // get methods
    uint16_t getADC(uint8_t);
    double getVolts(uint8_t);

private:
    Adafruit_ADS1115 *_ads;
};
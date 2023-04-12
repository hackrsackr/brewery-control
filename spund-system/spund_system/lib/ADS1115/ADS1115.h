#pragma once

#include "Adafruit_ADS1X15.h"

// #define ADS1115_ADDRESS1 (0x48) // ADDR -> GND
// #define ADS1115_ADDRESS2 (0x49) // ADDR -> VCC
// #define ADS1115_ADDRESS3 (0x4a) // ADDR -> SDA
// #define ADS1115_ADDRESS4 (0x4b) // ADDR -> SCL

/*
GAIN_TWOTHIRDS    = 0 - 6.144V
GAIN_ONE          = 0 - 4.096V
GAIN_TWO          = 0 - 2.048V
GAIN_FOUR         = 0 - 1.024V
GAIN_EIGHT        = 0 - 0.512V
GAIN_SIXTEEN      = 0 - 0.256V
*/

class ADS1115
{
public:
    // constructor
    ADS1115();
    ~ADS1115();

    // initializer
    void begin();
    void begin(uint8_t);
    void begin(uint8_t, adsGain_t);

    // get methods
    uint16_t getADC(uint8_t);
    float getVolts(uint8_t);

private:
    Adafruit_ADS1115 *_ads;
};
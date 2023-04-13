#include "Arduino.h"
#include "config.h"

#include "ADS_Sensor.h"

#define I2C_SCL 25
#define I2C_SDA 26

// ads constructor
ADS_Sensor ads;

void setup(void)
{
    Serial.begin(115200);
    ads.begin(ADS1115_ADDRESS2, GAIN_TWOTHIRDS, I2C_SDA, I2C_SCL);
}

void loop(void)
{
    for (uint8_t i = 0; i < 4; ++i)
    {
        Serial.printf("ADC%d: %6d \t", i, ads.getADC(i));
        Serial.printf("VOLTS%d: %1.2f \n", i, ads.getVolts(i));
    }

    Serial.println("------------------------------------------");
    delay(1000);
}
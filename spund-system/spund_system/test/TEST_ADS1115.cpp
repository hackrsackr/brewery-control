#include "Arduino.h"
#include "ADS1115.h"

// ads constructor
ADS1115 ads;

void setup(void)
{
    Serial.begin(115200);

    ads.begin(ADS1115_ADDRESS2); // address = 0x49, ADDR -> VCC
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

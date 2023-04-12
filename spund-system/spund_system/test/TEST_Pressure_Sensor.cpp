#include "Pressure_Sensor.h"
#include "config.h"

Pressure_Sensor ps;

void setup(void)
{
    Serial.begin(115200);

    ps.begin(
        ADS1115_ADDRESS2,
        GAIN_TWOTHIRDS,
        OFFSET_VOLTS1,
        UNIT_MAX1);
}

void loop(void)
{
    for (uint8_t i = 0; i < 4; ++i)
    {
        Serial.printf("ADC%d: %6d \t", i, ps.getADC(i));
        Serial.printf("VOLTS%d: %1.2f \t", i, ps.getVolts(i));
        Serial.printf("PSI%d: %2.2f \n", i, ps.getPSI(i));
    }

    Serial.println("------------------------------------------");
    delay(1000);
}

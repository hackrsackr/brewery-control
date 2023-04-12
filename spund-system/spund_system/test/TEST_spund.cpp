#include "Arduino.h"
#include <array>

#include "ADS1115.h"
#include "spund.h"
#include "Pressure_Sensor.h"
#include "relay.h"
#include "config.h"

// #define NUMBER_OF_SPUNDERS 4

// std::array<Relay, NUMBER_OF_RELAYS> relay_arr;
std::array<Spund_System, NUMBER_OF_SPUNDERS> spund_arr;

void setup(void)
{
    Serial.begin(115200);

    for (uint8_t i = 0; i < NUMBER_OF_SPUNDERS; ++i)
    {
        spund_arr[i].begin(ADS1115_ADDRESS2, GAIN_TWOTHIRDS, OFFSET_VOLTS[i], UNIT_MAXS[i], RELAY_PINS[i]);
    }
}

void loop(void)
{
    for (uint8_t i = 0; i < NUMBER_OF_SPUNDERS; ++i)
    {
        // Serial.printf("ADC%d: %6d \t", i, _ps->getADC(i));
        Serial.printf("VOLTS%d: %1.2f \t", i, spund_arr[i].readVolts(i));
        Serial.printf("PSI%d: %2.2f \n", i, spund_arr[i].readPSI(i));
        Serial.printf("relay_pin%d: %d\n", i, spund_arr[i].relay_pin);
        spund_arr[i].vent();
        delay(5000);
    }
}
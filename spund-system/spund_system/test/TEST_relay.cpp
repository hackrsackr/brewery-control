#include "Arduino.h"
#include <array>

#include "relay.h"

#define RPIN1 14
#define RPIN2 27
#define RPIN3 16
#define RPIN4 17

std::array<const uint8_t, NUMBER_OF_RELAYS>
    RELAY_PINS = {RPIN1, RPIN2, RPIN3, RPIN4};

std::array<Relay, NUMBER_OF_RELAYS> relay_arr;

void setup(void)
{
    Serial.begin(115200);

    for (uint8_t i = 0; i < NUMBER_OF_RELAYS; ++i)
    {
        relay_arr[i].begin(RELAY_PINS[i]);
    }
}

void loop(void)
{
    for (uint8_t i = 0; i < NUMBER_OF_RELAYS; ++i)
    {
        Serial.printf("relay_pin%d: %d\n", i, relay_arr[i].relay_pin);
        relay_arr[i].toggleRelay(relay_arr[i].relay_pin);
        delay(5000);
    }
}
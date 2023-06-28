#include "Arduino.h"

#include "Relay.h"

Relay::Relay() {}

Relay::~Relay() {}

void Relay::begin(uint8_t vent_pin)
{
    relay_pin = vent_pin;
    relay_state = !RELAY_OPEN;
    relay_toggled = false;
    pinMode(relay_pin, OUTPUT);
    closeRelay();
}

void Relay::openRelay()
{
    digitalWrite(relay_pin, RELAY_OPEN);
    relay_state = RELAY_OPEN;
    relay_toggled = true;
}

void Relay::closeRelay()
{
    digitalWrite(relay_pin, !RELAY_OPEN);
    relay_state = !RELAY_OPEN;
}

#include "Arduino.h"

#include "Relay.h"

Relay::Relay() {}

Relay::~Relay() {}

void Relay::begin(uint8_t pin)
{
    _relay_pin = pin;
    _relay_state = !RELAY_OPEN;
    pinMode(_relay_pin, OUTPUT);
    closeRelay();
}

void Relay::setRelayPin(uint8_t pin)
{
    _relay_pin = pin;
}

void Relay::openRelay()
{
    digitalWrite(_relay_pin, RELAY_OPEN);
    _relay_state = RELAY_OPEN;
}

void Relay::closeRelay()
{
    digitalWrite(_relay_pin, !RELAY_OPEN);
    _relay_state = !RELAY_OPEN;
}

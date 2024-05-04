#include "Arduino.h"

#include "Relay.h"

#include "config.h"

Relay::Relay() {}

Relay::~Relay() {}

void Relay::begin(uint8_t pin)
{
    _relay_pin = pin;
    _relay_state = !RELAY_OPEN;
    pinMode(_relay_pin, OUTPUT);
    closeRelay();
}

auto Relay::getRelayPin() -> uint8_t
{
    return _relay_pin;
}

auto Relay::getRelayState() -> uint8_t
{
    return _relay_state;
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

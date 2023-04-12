#include "Arduino.h"

#include "Relay.h"

// #define RELAY_OPEN HIGH // relay logic

Relay::Relay() {}

Relay::~Relay() {}

void Relay::begin(uint8_t pin)
{
    relay_pin = pin;
    pinMode(relay_pin, OUTPUT);
    digitalWrite(relay_pin, !RELAY_OPEN);
}

void Relay::toggleRelay()
{
    digitalWrite(relay_pin, RELAY_OPEN);
    delay(500);
    digitalWrite(relay_pin, !RELAY_OPEN);
}

void Relay::openRelay()
{
    digitalWrite(relay_pin, RELAY_OPEN);
}

void Relay::closeRelay()
{
    digitalWrite(relay_pin, !RELAY_OPEN);
}

#include "Arduino.h"

#include "Relay.h"

Relay::Relay(uint8_t relay_pin)
{
    m_relay_pin = relay_pin;
    m_relay_state = !RELAY_OPEN;
    pinMode(m_relay_pin, OUTPUT);
    closeRelay();
}

Relay::~Relay() {}

void Relay::openRelay()
{
    digitalWrite(m_relay_pin, RELAY_OPEN);
    m_relay_state = RELAY_OPEN;
}

void Relay::closeRelay()
{
    digitalWrite(m_relay_pin, !RELAY_OPEN);
    m_relay_state = !RELAY_OPEN;
}

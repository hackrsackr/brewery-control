#pragma once

#define RELAY_OPEN HIGH

class Relay
{
private:
    uint8_t m_relay_pin;
    uint8_t m_relay_state;

public:
    Relay(uint8_t);
    ~Relay();

    void openRelay();
    void closeRelay();
};
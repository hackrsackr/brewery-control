#pragma once
#include <cstdint>

#define RELAY_OPEN HIGH

class Relay
{
private:
    uint8_t _relay_pin;
    uint8_t _relay_state;

public:
    Relay();
    ~Relay();

    void begin(uint8_t);
    void setRelayPin(uint8_t);
    auto getRelayPin() -> uint8_t;
    auto getRelayState() -> uint8_t;
    void openRelay();
    void closeRelay();
};
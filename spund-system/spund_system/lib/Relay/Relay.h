#pragma once

#define RELAY_OPEN HIGH

class Relay
{
public:
    uint8_t relay_pin;
    uint8_t relay_state;

    // Constructors
    Relay();
    ~Relay();

    // Initializer
    void begin(uint8_t);

    // Methods
    void openRelay();
    void closeRelay();
    // void toggleRelay();
};
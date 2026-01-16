#pragma once
#include <cstdint>

#define RELAY_OPEN HIGH
constexpr uint32_t MAX_RELAY_OPEN_MS = 2000; // Maximum 2 seconds open
constexpr uint32_t RELAY_PULSE_MS = 500;      // Normal pulse duration

class Relay
{
private:
    uint8_t _relay_pin;
    uint8_t _relay_state;
    uint32_t _relay_opened_at;
    bool _safety_triggered;

public:
    Relay();
    ~Relay();

    void begin(uint8_t);
    void setRelayPin(uint8_t);
    auto getRelayPin() -> uint8_t;
    auto getRelayState() -> uint8_t;
    auto isSafetyTriggered() -> bool;
    void openRelay();
    void closeRelay();
    void pulseRelay(uint32_t duration_ms = RELAY_PULSE_MS);
    void checkSafety(); // Call this in main loop
    void resetSafety();
};

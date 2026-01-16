#include "Arduino.h"
#include "Relay.h"

Relay::Relay() : _relay_opened_at(0), _safety_triggered(false) {}

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

auto Relay::isSafetyTriggered() -> bool
{
    return _safety_triggered;
}

void Relay::setRelayPin(uint8_t pin)
{
    _relay_pin = pin;
}

void Relay::openRelay()
{
    if (_safety_triggered) {
        Serial.println("ERROR: Relay safety triggered, cannot open");
        return;
    }
    
    digitalWrite(_relay_pin, RELAY_OPEN);
    _relay_state = RELAY_OPEN;
    _relay_opened_at = millis();
}

void Relay::closeRelay()
{
    digitalWrite(_relay_pin, !RELAY_OPEN);
    _relay_state = !RELAY_OPEN;
    _relay_opened_at = 0;
}

void Relay::pulseRelay(uint32_t duration_ms)
{
    if (_safety_triggered) {
        Serial.println("ERROR: Relay safety triggered, cannot pulse");
        return;
    }
    
    if (duration_ms > MAX_RELAY_OPEN_MS) {
        Serial.printf("WARNING: Pulse duration %dms exceeds max %dms, clamping\n", 
                      duration_ms, MAX_RELAY_OPEN_MS);
        duration_ms = MAX_RELAY_OPEN_MS;
    }
    
    openRelay();
    delay(duration_ms);
    closeRelay();
}

void Relay::checkSafety()
{
    if (_relay_state == RELAY_OPEN && _relay_opened_at > 0) {
        uint32_t open_duration = millis() - _relay_opened_at;
        
        if (open_duration > MAX_RELAY_OPEN_MS) {
            Serial.printf("CRITICAL: Relay %d open for %dms, FORCE CLOSING\n", 
                          _relay_pin, open_duration);
            closeRelay();
            _safety_triggered = true;
        }
    }
}

void Relay::resetSafety()
{
    _safety_triggered = false;
    Serial.println("Relay safety reset");
}
#include "Arduino.h"

#include "Spund_System.h"
#include "Pressure_Sensor.h"
#include "Relay.h"
#include "config.h"

// #define RELAY_OPEN HIGH // relay logic

Spund_System::Spund_System() {}

Spund_System::~Spund_System() {}

void Spund_System::begin(
    uint8_t ads_address,
    adsGain_t gain,
    uint8_t sda,
    uint8_t scl,
    float offset_volts,
    uint8_t unit_max,
    uint8_t relay_pin,
    float vols_setpoint)
{
    _ps = new Pressure_Sensor();
    _ps->begin(ads_address, gain, sda, scl, offset_volts, unit_max);

    _re = new Relay();
    _re->begin(relay_pin);
    _re->relay_state = 0;

    this->relay_pin = relay_pin;
    this->vols_setpoint = vols_setpoint;
}

float Spund_System::getTempF(float tempC)
{
    tempF = tempC * 1.8 + 32;

    return tempF;
}

float Spund_System::getVolts(uint8_t channel)
{
    volts = _ps->getVolts(channel);

    return volts;
}

float Spund_System::getPSI(uint8_t channel)
{
    return ((Spund_System::getVolts(channel) - this->offset_volts) * (_ps->unit_max / 4.0));
}

float Spund_System::getPSISetpoint()
{
    float a = -16.669 - (.0101059 * tempF) + (.00116512 * (tempF * tempF));
    float b = .173354 * tempF * vols_setpoint;
    float c = (4.24267 * vols_setpoint) - (.0684226 * (vols_setpoint * vols_setpoint));

    psi_setpoint = a + b + c;

    return psi_setpoint;
}

float Spund_System::getVols()
{
    float a = -.0684226;
    float b = ((.173354 * tempF) + 4.24267);
    float c = (-_ps->psi + -16.669 + (-0.0101059 * tempF) + (0.00116512 * tempF * tempF));
    float d = ((b * b) - (4 * a * c));

    vols = ((-b + (pow(d, .5))) / (2 * a));

    return vols;
}

float Spund_System::test_carb()
{
    _re->relay_state = 0;
    // if (psi_value > psi_setpoint)
    if (vols > vols_setpoint)
    {
        vent();
    }
    minutes_since_vent = ((millis() - time_of_last_vent) / 60000.0);

    return minutes_since_vent;
}
void Spund_System::vent()
{
    digitalWrite(relay_pin, RELAY_OPEN);
    _re->relay_state = 1;
    delay(500);
    time_of_last_vent = millis();
    digitalWrite(relay_pin, !RELAY_OPEN);
}
#include "Arduino.h"

#include "ADS_Sensor.h"
#include "Relay.h"
#include "Spund_System.h"

#include "config.h"

Spund_System::Spund_System() {}

Spund_System::~Spund_System() {}

void Spund_System::begin(
    uint8_t ads_addr,
    adsGain_t ads_gain,
    uint8_t i2c_sda,
    uint8_t i2c_scl,
    uint8_t ads_chan,
    double offset_vs,
    uint8_t max_unit,
    uint8_t vent_pin,
    double desired_vols)
{
    _ps = new ADS_Pressure_Sensor();
    _ps->begin(ads_addr, ads_gain, i2c_sda, i2c_scl, ads_chan, offset_vs, max_unit);

    _re = new Relay();
    _re->begin(vent_pin);
    _re->relay_state = 0;

    vols_setpoint = desired_vols;
}

double Spund_System::getVolts()
{
    return _ps->readVolts(_ps->ads_channel);
}

double Spund_System::getPSI()
{
    return _ps->computePSI();
}

double Spund_System::getPSISetpoint()
{
    double a = -16.669 - (.0101059 * tempF) + (.00116512 * (tempF * tempF));
    double b = .173354 * tempF * vols_setpoint;
    double c = (4.24267 * vols_setpoint) - (.0684226 * (vols_setpoint * vols_setpoint));

    psi_setpoint = a + b + c;

    return psi_setpoint;
}

double Spund_System::computeVols()
{
    double a = -.0684226;
    double b = ((.173354 * tempF) + 4.24267);
    double c = (-_ps->computePSI() + -16.669 + (-0.0101059 * tempF) + (0.00116512 * tempF * tempF));
    double d = ((b * b) - (4 * a * c));

    vols = ((-b + (pow(d, .5))) / (2 * a));

    return vols;
}

double Spund_System::test_carb()
{
    // if (vols > vols_setpoint && _ps->computePSI() > getPSISetpoint())
    if (vols > vols_setpoint)
    {
        _re->openRelay();
        delay(500);
        time_of_last_vent = millis();
        _re->closeRelay();
    }
    double minutes_since_vent = (millis() - time_of_last_vent) / 60000.0;

    return minutes_since_vent;
}

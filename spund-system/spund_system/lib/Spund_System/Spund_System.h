#pragma once

#include "ADS_Sensor.h"
#include "Relay.h"

#include "config.h"

class Spund_System
{
public:
    uint32_t time_of_last_vent;
    double psi_setpoint;
    double tempC;
    double tempF;
    double vols;
    double vols_setpoint;

    // Constructors
    Spund_System();
    ~Spund_System();

    // Initializers
    void begin(uint8_t, adsGain_t, uint8_t, uint8_t, uint8_t, double, uint8_t, uint8_t, double);

    // Get Methods
    double getVolts();
    double getPSI();
    double getPSISetpoint();
    double computeVols();
    double test_carb();

private:
    ADS_Pressure_Sensor *_ps;
    Relay *_re;
};
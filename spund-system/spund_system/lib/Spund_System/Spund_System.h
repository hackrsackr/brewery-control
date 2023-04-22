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

    // Initializer
    void begin(
        uint8_t ads_addr,
        adsGain_t ads_gain,
        uint8_t i2c_sda,
        uint8_t i2c_scl,
        uint8_t ads_chan,
        double offset_vs,
        uint8_t max_unit,
        uint8_t vent_pin);

    // Get Methods
    double getVolts();
    double getPSI();

    // Calculations
    double computePSISetpoint();
    double computeVols();

    // Evaluation
    double test_carb();

private:
    ADS_Pressure_Sensor *_ps;
    Relay *_re;
};
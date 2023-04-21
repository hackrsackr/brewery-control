#pragma once

#include "Pressure_Sensor.h"
#include "Relay.h"

#include "config.h"

class Spund_System
{
public:
    String id;
    String mqtt_field;
    uint8_t relay_pin;
    uint8_t relay_state;
    uint32_t time_of_last_vent;
    double minutes_since_vent;
    double offset_volts;
    double psi;
    double psi_setpoint;
    double tempC;
    double tempF;
    double vols;
    double vols_setpoint;
    double volts;

    // Constructors
    Spund_System();
    ~Spund_System();

    // Initializers
    void begin(uint8_t, adsGain_t, uint8_t, uint8_t, double, uint8_t, uint8_t, double);

    // Get Methods
    double getTempF(double);
    double getVolts(uint8_t);
    double getPSI(uint8_t);
    double getPSISetpoint();
    double getVols();
    double test_carb();

    //
    double convertTempCtoF(double);
    void vent();

private:
    Pressure_Sensor *_ps;
    Relay *_re;
};
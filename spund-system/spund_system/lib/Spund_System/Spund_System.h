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
    float minutes_since_vent;
    float volts;
    float psi;
    float offset_volts;
    float tempC;
    float tempF;
    float psi_setpoint;
    float vols_setpoint;
    float vols;

    // Constructors
    Spund_System();
    ~Spund_System();

    // Initializers
    void begin(uint8_t, adsGain_t, uint8_t, uint8_t, float, uint8_t, uint8_t, float);

    // Get Methods
    float getTempF(float);
    float getVolts(uint8_t);
    float getPSI(uint8_t);
    float getPSISetpoint();
    float getVols();
    float test_carb();

    //
    float convertTempCtoF(float);
    void vent();

private:
    Pressure_Sensor *_ps;
    Relay *_re;
};
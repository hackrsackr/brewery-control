#pragma once

#include "ADS_Sensor.h"
#include "Relay.h"
#include "ArduinoJson.h"

// Spunder settings
struct Spund_Settings
{
    String spunder_id;
    String temp_sensor_id;
    double desired_vols;
    uint8_t ads_addr;
    adsGain_t ads_gain;
    uint8_t i2c_sda;
    uint8_t i2c_scl;
    uint8_t ads_channel;
    uint8_t relay_pin;
    double min_sensor_volts;
    double max_sensor_volts;
    double max_sensor_psi;
    String server_setpoint_input;
    String server_sensor_input;
};

class Spund_System
{
public:
    // Spund_Syatem config
    String spunder_id;
    String temp_sensor_id; 
    double desired_vols;

    // ADS1115 config
    uint8_t ads_addr;
    adsGain_t ads_gain;
    uint8_t i2c_sda;
    uint8_t i2c_scl;
    uint8_t ads_channel;

    // Relay config
    uint8_t relay_pin;

    // Pressure_Sensor config
    double min_sensor_volts;
    double max_sensor_volts;
    double max_sensor_psi;

    // Webserver config
    String server_setpoint; 
    String server_sensor;   
    String server_setpoint_input;
    String server_sensor_input; 

    // Program variables
    uint32_t time_of_last_vent;
    double psi_setpoint;
    double tempC;
    double tempF;
    double vols;
    Spund_Settings settings;

    // Constructors
    Spund_System();
    Spund_System(Spund_Settings config);
    Spund_System(
        uint8_t ads_addr,
        adsGain_t ads_gain,
        uint8_t i2c_sda,
        uint8_t i2c_scl,
        uint8_t ads_chan,
        double min_vs,
        double max_vs,
        double max_unit,
        uint8_t vent_pin);

    void begin(
        uint8_t ads_addr,
        adsGain_t ads_gain,
        uint8_t i2c_sda,
        uint8_t i2c_scl,
        uint8_t ads_chan,
        double min_vs,
        double max_vs,
        double max_unit,
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
    std::shared_ptr<ADS_Pressure_Sensor> s_ps;
    std::shared_ptr<Relay> s_re;
};
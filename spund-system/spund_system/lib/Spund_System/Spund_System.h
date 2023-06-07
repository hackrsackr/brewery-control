#pragma once

#include "ADS_Sensor.h"
#include "Relay.h"
#include "ArduinoJson.h"

struct Settings
{
    // Spunder settings
    String name;
    double desired_vols;
    uint8_t relay_pin;
    String temp_sensor_name; // old mqtt field

    // Pressure_Sensor settings
    uint8_t ads_channel;
    double min_sensor_volts;
    double max_sensor_volts;
    double max_psi;

    // Server Config
    String server_setpoint; //= temp_sensor_name; // old mqtt_message = old mqtt_field
    String server_sensor;   //= temp_sensor_name; // old mqtt_message = old mqtt_field

    String server_setpoint_input;
    String server_sensor_input; // old mqtt_input
};

class Spund_System
{
public:
    uint32_t time_of_last_vent;

    double psi_setpoint;
    double tempC;
    double tempF;
    double vols;
    double vols_setpoint;

    // String server_setpoint = String(vols_setpoint); //= temp_sensor_name; // old mqtt_message = old mqtt_field
    // String server_sensor = settings.temp_sensor_name;

    // struct Settings
    // {
    //     // Spunder settings
    //     String name;
    //     double desired_vols;
    //     uint8_t relay_pin;
    //     String temp_sensor_name; // old mqtt field

    //     // Pressure_Sensor settings
    //     uint8_t ads_channel;
    //     double min_sensor_volts;
    //     double max_sensor_volts;
    //     double max_psi;

    //     // Server Config
    //     String server_setpoint; //= temp_sensor_name; // old mqtt_message = old mqtt_field
    //     String server_sensor; //= temp_sensor_name; // old mqtt_message = old mqtt_field

    //     String server_setpoint_input;
    //     String server_sensor_input; // old mqtt_input
    // };

    // Constructor
    Spund_System();
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

    Settings settings;

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
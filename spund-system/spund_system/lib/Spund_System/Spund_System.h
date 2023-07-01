#pragma once

#include "ADS_Sensor.h"
#include "Relay.h"

typedef struct spund_system_cfg_t
{
    struct spund_cfg
    {
        String spunder_id;
        double desired_vols;
        uint8_t relay_pin;
    } spunder;

    struct ads_cfg
    {
        uint8_t ads_addr;
        adsGain_t ads_gain;
        uint8_t i2c_sda;
        uint8_t i2c_scl;
        uint8_t ads_channel;
    } ads1115;

    struct sensor_cfg
    {
        double min_sensor_volts;
        double max_sensor_volts;
        double max_sensor_psi;
        double sensor_offset_volts;
    } sensor;

    struct mqtt_cfg
    {
        String temp_sensor_id;
        String server_setpoint_input;
        String server_sensor_input;
    } mqtt;
} spund_system_cfg_t;

class Spund_System
{
public:
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
    double sensor_offset_volts;
    String server_setpoint;
    String server_sensor;
    String server_setpoint_input;
    String server_sensor_input;
    uint32_t time_of_last_vent;
    double psi_setpoint;
    double tempC;
    double tempF;
    double vols;

    Spund_System(spund_system_cfg_t);

    double getVolts();
    double getPSI();
    double computePSISetpoint();
    double computeVols();
    uint8_t testCarb();
    double getLastVent();

private:
    std::shared_ptr<ADS_Pressure_Sensor> s_ps;
    std::shared_ptr<Relay> s_re;
};
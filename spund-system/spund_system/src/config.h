#pragma once

#include <vector>

#include "Adafruit_ADS1X15.h"
#include "Spund_System.h"

// I2C Pins default for esp is scl=22, sda=21
const uint8_t _I2C_SCL = 25;
const uint8_t _I2C_SDA = 26;

// MQTT Setup
const auto _MQTTHOST = "192.168.1.6";
const auto _MQTTPORT = 1883;
const auto _CLIENTID = "spund-dev";
const auto _SUBTOPIC = "brewcast/history/spark-dev";
const auto _PUBTOPIC = "brewcast/history/spund-dev";

/*
Output message option
true = publish over mqtt
false = print to serial, to be read by docker container
*/
bool _PUBLISHMQTT = true;

/*
Config vector:
Change all parameters here
*/
std::vector<Spund_Settings> _SETTINGS{
    {
        "spunder-1",        // spunder_id
        "TempSensor-1",     // temp_sensor_id
        3.01,               // desired_vols
        ADS1115_ADDRESS1,   // ads_addr
        GAIN_TWOTHIRDS,     // ads_gain
        _I2C_SDA,           // i2c_sda
        _I2C_SCL,           // i2c_scl
        0,                  // ads_channel
        14,                 // relay_pin
        .5,                 // min_sensor_volts
        4.5,                // max_sensor_volts
        60.0,               // max_psi
        "setpoint_input-1", // server_setpoint_input
        "mqtt_input-1",     // server_sensor_input
    },
    {
        "spunder-2",        // spunder_id
        "TempSensor-1",     // temp_sensor_id
        3.02,               // desired_vols
        ADS1115_ADDRESS1,   // ads_addr
        GAIN_TWOTHIRDS,     // ads_gain
        _I2C_SDA,           // i2c_sda
        _I2C_SCL,           // i2c_scl
        1,                  // ads_channel
        27,                 // relay_pin
        .5,                 // min_sensor_volts
        4.5,                // max_sensor_volts
        60.0,               // max_psi
        "setpoint_input-2", // server_setpoint_input
        "mqtt_input-2",     // server_sensor_input
    },
    {
        "spunder-3",        // spunder_id
        "TempSensor-2",     // temp_sensor_id
        3.03,               // desired_vols
        ADS1115_ADDRESS1,   // ads_addr
        GAIN_TWOTHIRDS,     // ads_gain
        _I2C_SDA,           // i2c_sda
        _I2C_SCL,           // i2c_scl
        2,                  // ads_channel
        16,                 // relay_pin
        .5,                 // min_sensor_volts
        4.5,                // max_sensor_volts
        60.0,               // max_psi
        "setpoint_input-3", // server_setpoint_input
        "mqtt_input-3"      // server_sensor_input
    },
    {
        "spunder-4",        // spunder_id
        "TempSensor-2",     // temp_sensor_id
        3.04,               // desired_vols
        ADS1115_ADDRESS1,   // ads_addr
        GAIN_TWOTHIRDS,     // ads_gain
        _I2C_SDA,           // i2c_sda
        _I2C_SCL,           // i2c_scl
        3,                  // ads_channel
        17,                 // relay_pin
        .5,                 // min_sensor_volts
        4.5,                // max_sensor_volts
        60.0,               // max_psi
        "setpoint_input-4", // server_setpoint_input
        "mqtt_input-4"      // server_sensor_input
    }};
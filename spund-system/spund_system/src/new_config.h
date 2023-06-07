#pragma once

#include <array>
#include <vector>

#include "Adafruit_ADS1X15.h"
#include "Spund_System.h"
#include "secrets.h"

// #define ADS1115_ADDRESS1 (0x48) // ADDR -> GND
// #define ADS1115_ADDRESS2 (0x49) // ADDR -> VCC
// #define ADS1115_ADDRESS3 (0x4a) // ADDR -> SDA
// #define ADS1115_ADDRESS4 (0x4b) // ADDR -> SCL
#define _NUMBER_OF_SPUNDERS 4

// WiFi
#define _SSID SECRET_SSID
#define _PASS SECRET_PASS

// I2C Pins
// #define _I2C_SCL 22 // default
// #define _I2C_SDA 21 // default
#define _I2C_SCL 25
#define _I2C_SDA 26

// MQTT Setup
#define _MQTTHOST "192.168.1.6"
#define _MQTTPORT 1883
#define _CLIENTID "spund-dev"
#define _SUBTOPIC "brewcast/history/spark-dev"
#define _PUBTOPIC "brewcast/history/spund-dev"

std::vector<Settings> _SETTINGS{
    {
        "spunder-1",    // name
        3.01,           // desired_vols
        14,             // relay_pin
        "TempSensor-1", // temp_sensor_name
        0,              // ads_channel
        .5,             // min_sensor_volts
        4.5,            // max_sensor_volts
        60.0,           // max_psi
        "3.01",             // server_setpoint = String(desired_vols)
        "TempSensor-1",     // server_sensor = temp_sensor_name
        "setpoint_input-1", // server_setpoint_input
        "mqtt_input-1"      // server_sensor_input
    },
    {
        "spunder-2",    // name
        3.02,           // desired_vols
        27,             // relay_pin
        "TempSensor-1", // temp_sensor_name
        1,              // ads_channel
        .5,             // min_sensor_volts
        4.5,            // max_sensor_volts
        60.0,           // max_psi
        "3.02",             // server_setpoint = String(desired_vols)
        "TempSensor-1",     // server_sensor = temp_sensor_name
        "setpoint_input-2", // server_setpoint_input
        "mqtt_input-2"      // server_sensor_input
    },
    {
        "spunder-3",    // name
        3.03,           // desired_vols
        16,             // relay_pin
        "TempSensor-2", // temp_sensor_name
        2,              // ads_channel
        .5,             // min_sensor_volts
        4.5,            // max_sensor_volts
        60.0,           // max_psi
        "3.03",             // server_setpoint = String(desired_vols)
        "TempSensor-2",     // server_sensor = temp_sensor_name
        "setpoint_input-3", // server_setpoint_input
        "mqtt_input-3"      // server_sensor_input
    },
    {
        "spunder-4",    // name
        3.04,           // desired_vols
        17,             // relay_pin
        "TempSensor-2", // temp_sensor_name
        3,              // ads_channel
        .5,             // min_sensor_volts
        4.5,            // max_sensor_volts
        60.0,           // max_psi
        "3.04",             // server_setpoint = String(desired_vols)
        "TempSensor-2",     // server_sensor = temp_sensor_name
        "setpoint_input-4", // server_setpoint_input
        "mqtt_input-4"      // server_sensor_input
    }};

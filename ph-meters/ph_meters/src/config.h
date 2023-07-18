#pragma once

#include <vector>

#include "MA_Meter.h"
#include "Level_Sensor.h"

#include "secrets.h"

// WiFi credentials
constexpr auto _SSID{SECRET_SSID};
constexpr auto _PASS{SECRET_PASS};

/* Output message option
 * true = publish over mqtt
 * false = print to serial to be read by docker container
 */
constexpr bool _PUBLISHMQTT{true};

// MQTT constants
constexpr auto _MQTTHOST{"192.168.1.2"};
constexpr auto _MQTTPORT{1883};
constexpr auto _CLIENTID{"ph-dev2"};
constexpr auto _PUBTOPIC{"brewcast/history/ph-dev2"};

// I2C pins
constexpr uint8_t _I2C_SCL{22};
constexpr uint8_t _I2C_SDA{21};
// constexpr uint8_t _I2C_SCL{25};
// constexpr uint8_t _I2C_SDA{26};

// I2C adresses
constexpr byte _I2C_ADDRESS1{0x48}; // GND
constexpr byte _I2C_ADDRESS2{0x49}; // VIN
constexpr byte _I2C_ADDRESS3{0x4a}; // SDA
constexpr byte _I2C_ADDRESS4{0x4b}; // SCL

// MA_Meter configs
std::vector<ma_meter_cfg_t> meter_cfgs{
    {
        .id = "meter-1",
        .i2c_address = _I2C_ADDRESS1,
        .ads_channel = 0,
    },
    {
        .id = "meter-2",
        .i2c_address = _I2C_ADDRESS1,
        .ads_channel = 1,
    },
    {
        .id = "meter-3",
        .i2c_address = _I2C_ADDRESS1,
        .ads_channel = 2,
    },
    {
        .id = "meter-4",
        .i2c_address = _I2C_ADDRESS1,
        .ads_channel = 3,
    },
};

std::vector<level_sensor_cfg_t> level_sensor_cfgs{
    {
        .id = "Level_Sensor-1",
        .i2c_address = _I2C_ADDRESS2,
        .ads_channel = 0,
        .adc_offset = 8000,
    },
    {
        .id = "Level_Sensor-2",
        .i2c_address = _I2C_ADDRESS2,
        .ads_channel = 1,
        .adc_offset = 5824,
    },
    {
        .id = "Level_Sensor-3",
        .i2c_address = _I2C_ADDRESS2,
        .ads_channel = 2,
        .adc_offset = 6960,
    },
    {
        .id = "Level_Sensor-4",
        .i2c_address = _I2C_ADDRESS2,
        .ads_channel = 3,
        .adc_offset = 6960,
    },
};
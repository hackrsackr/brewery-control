#pragma once

#include <vector>

#include "MA_Meter.h"
// #include "Level_Sensor.h"

#include "secrets.h"

// WiFi credentials
constexpr auto _SSID{SECRET_SSID};
constexpr auto _PASS{SECRET_PASS};

/* Output message option
 * true = publish over mqtt
 * false = print to serial to be read by docker container
 */
constexpr bool _PUBLISHMQTT{false};

// MQTT constants
constexpr auto _MQTTHOST{"10.0.0.101"};
constexpr auto _MQTTPORT{1883};
constexpr auto _CLIENTID{"mA-meters"};
constexpr auto _PUBTOPIC{"brewcast/history/mA-meters"};

// I2C pins
// constexpr uint8_t _I2C_SCL{22};
// constexpr uint8_t _I2C_SDA{21};
constexpr uint8_t _I2C_SCL{25};
constexpr uint8_t _I2C_SDA{26};

// I2C adresses
constexpr byte _I2C_ADDRESS1{0x48}; // GND
constexpr byte _I2C_ADDRESS2{0x49}; // VIN
constexpr byte _I2C_ADDRESS3{0x4a}; // SDA
constexpr byte _I2C_ADDRESS4{0x4b}; // SCL

std::vector<ma_meter_cfg_t> meter_cfgs{
    {
        .id = "mash_pH",
        .measurement = "pH",
        .i2c_address = _I2C_ADDRESS1,
        .ads_channel = 0,
        .ads_gain = GAIN_TWOTHIRDS,
        .input_LRV = 4,
        .input_URV = 20,
        .output_LRV = 0,
        .output_URV = 80,
    },
    {
        .id = "mash_pH",
        .measurement = "TempC",
        .i2c_address = _I2C_ADDRESS1,
        .ads_channel = 1,
        .ads_gain = GAIN_TWOTHIRDS,
        .input_LRV = 4,
        .input_URV = 20,
        .output_LRV = 0,
        .output_URV = 24,
    },
    {
        .id = "boil_pH",
        .measurement = "pH",
        .i2c_address = _I2C_ADDRESS1,
        .ads_channel = 2,
        .ads_gain = GAIN_TWOTHIRDS,
        .input_LRV = 0,
        .input_URV = 20,
        .output_LRV = 0,
        .output_URV = 10,
    },
    {
        .id = "boil_pH",
        .measurement = "TempC",
        .i2c_address = _I2C_ADDRESS1,
        .ads_channel = 3,
        .ads_gain = GAIN_TWOTHIRDS,
        .input_LRV = 0,
        .input_URV = 20,
        .output_LRV = 0,
        .output_URV = 20,
    },
    {
        .id = "liqr_O2",
        .measurement = "PPM",
        .i2c_address = _I2C_ADDRESS2,
        .ads_channel = 0,
        .ads_gain = GAIN_TWOTHIRDS,
        .input_LRV = 4,
        .input_URV = 20,
        .output_LRV = 0,
        .output_URV = 80,
    },
    {
        .id = "liqr_O2",
        .measurement = "TempC",
        .i2c_address = _I2C_ADDRESS2,
        .ads_channel = 1,
        .ads_gain = GAIN_TWOTHIRDS,
        .input_LRV = 4,
        .input_URV = 20,
        .output_LRV = 0,
        .output_URV = 24,
    },
    {
        .id = "inline_O2",
        .measurement = "PPM",
        .i2c_address = _I2C_ADDRESS2,
        .ads_channel = 2,
        .ads_gain = GAIN_TWOTHIRDS,
        .input_LRV = 0,
        .input_URV = 20,
        .output_LRV = 0,
        .output_URV = 10,
    },
    {
        .id = "inline_O2",
        .measurement = "TempC",
        .i2c_address = _I2C_ADDRESS2,
        .ads_channel = 3,
        .ads_gain = GAIN_TWOTHIRDS,
        .input_LRV = 0,
        .input_URV = 20,
        .output_LRV = 0,
        .output_URV = 20,
    },
    {
        .id = "liqr_volume",
        .measurement = "L",
        .i2c_address = _I2C_ADDRESS3,
        .ads_channel = 0,
        .ads_gain = GAIN_TWOTHIRDS,
        .input_LRV = 4,
        .input_URV = 20,
        .output_LRV = 0,
        .output_URV = 80,
    },
    {
        .id = "mash_volume",
        .measurement = "L",
        .i2c_address = _I2C_ADDRESS3,
        .ads_channel = 1,
        .ads_gain = GAIN_TWOTHIRDS,
        .input_LRV = 4,
        .input_URV = 20,
        .output_LRV = 0,
        .output_URV = 24,
    },
    {
        .id = "boil_volume",
        .measurement = "L",
        .i2c_address = _I2C_ADDRESS3,
        .ads_channel = 2,
        .ads_gain = GAIN_TWOTHIRDS,
        .input_LRV = 0,
        .input_URV = 20,
        .output_LRV = 0,
        .output_URV = 10,
    },
    // {
    //     .id = "ads3.4",
    //     .measurement = "O2_ppm",
    //     .i2c_address = _I2C_ADDRESS3,
    //     .ads_channel = 3,
    //     .ads_gain = GAIN_TWOTHIRDS,
    //     .input_LRV = 0,
    //     .input_URV = 20,
    //     .output_LRV = 0,
    //     .output_URV = 20,
    // },
    {
        .id = "liqr_flow",
        .measurement = "L/min",
        .i2c_address = _I2C_ADDRESS4,
        .ads_channel = 0,
        .ads_gain = GAIN_TWOTHIRDS,
        .input_LRV = 4,
        .input_URV = 20,
        .output_LRV = 0,
        .output_URV = 24,
    },
    {
        .id = "liqr_flow",
        .measurement = "TempC",
        .i2c_address = _I2C_ADDRESS4,
        .ads_channel = 1,
        .ads_gain = GAIN_TWOTHIRDS,
        .input_LRV = 4,
        .input_URV = 20,
        .output_LRV = -20,
        .output_URV = 80,
    },
    // {
    //     .id = "ads4.3",
    //     .measurement = "pH",
    //     .i2c_address = _I2C_ADDRESS4,
    //     .ads_channel = 2,
    //     .ads_gain = GAIN_TWOTHIRDS,
    //     .input_LRV = 0,
    //     .input_URV = 20,
    //     .output_LRV = 0,
    //     .output_URV = 10,
    // },
    // {
    //     .id = "ads4.4",
    //     .measurement = "O2_ppm",
    //     .i2c_address = _I2C_ADDRESS4,
    //     .ads_channel = 3,
    //     .ads_gain = GAIN_TWOTHIRDS,
    //     .input_LRV = 0,
    //     .input_URV = 20,
    //     .output_LRV = 0,
    //     .output_URV = 20,
    // },
};

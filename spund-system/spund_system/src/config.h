#pragma once

#include <vector>

#include "Spund_System.h"
#include "secrets.h"

/* Output message option
true = publish over mqtt
false = print to serial, to be read by docker container
*/
constexpr bool _PUBLISHMQTT = true;
// constexpr bool _PUBLISHMQTT = false;

/* Pressure control option
true = test carbonation and vent if neccessary
false = monitor pressure only
*/
// constexpr bool _VENT_TO_AIR = true;
constexpr bool _VENT_TO_AIR = false;

// WiFi credentials
constexpr auto _SSID = SECRET_SSID;
constexpr auto _PASS = SECRET_PASS;

// MQTT parameters
constexpr auto _MQTTHOST = "10.0.0.115";
constexpr auto _MQTTPORT = 1883;
constexpr auto _CLIENTID = "spund-system";
constexpr auto _SUBTOPIC = "brewcast/history/spark-one";
constexpr auto _PUBTOPIC = "brewcast/history/spund-system";

// ADS I2C addresses
constexpr auto ADS1115_ADDRESS1 = 0x48; // ADDR -> GND
constexpr auto ADS1115_ADDRESS2 = 0x49; // ADDR -> VCC
constexpr auto ADS1115_ADDRESS3 = 0x4a; // ADDR -> SDA
constexpr auto ADS1115_ADDRESS4 = 0x4b; // ADDR -> SCL

// GPIO Pins (espduino)
constexpr uint8_t _I2C_SCL = 25;
constexpr uint8_t _I2C_SDA = 26;

constexpr auto RELAY_PIN1 = 14;
constexpr auto RELAY_PIN2 = 27;
constexpr auto RELAY_PIN3 = 16;
constexpr auto RELAY_PIN4 = 17;

// GPIO Pins (esp32-s3-devkit)
// constexpr uint8_t _I2C_SCL = 17;
// constexpr uint8_t _I2C_SDA = 18;

// constexpr auto RELAY_PIN1 = 14;
// constexpr auto RELAY_PIN2 = 03;
// constexpr auto RELAY_PIN3 = 20;
// constexpr auto RELAY_PIN4 = 19;

std::vector<spund_system_cfg_t> spund_cfgs{
    {
            .spunder{
                .spunder_id = "spunder-1",
                .desired_vols = 3.41,
                .relay_pin = RELAY_PIN1,
            },
            .ads1115{
                .i2c_addr = ADS1115_ADDRESS1,
                .ads_channel = 0,
                .ads_gain = GAIN_TWOTHIRDS,
                .ads_sensor_unit = "PSI",
                .input_low_val = 0.5,
                .input_high_val = 4.5,
                .output_low_val = 0.0,
                .output_high_val = 60.0,
            },
            .mqtt{
                .temp_sensor_id = "TEMP_conicalBeer",
                .server_setpoint_input = "setpoint_input-1",
                .server_sensor_input = "mqtt_input-1",
            },
        },
        {
            .spunder{
                .spunder_id = "spunder-2",
                .desired_vols = 3.42,
                .relay_pin = RELAY_PIN2,
            },
            .ads1115{
                .i2c_addr = ADS1115_ADDRESS1,
                .ads_channel = 1,
                .ads_gain = GAIN_TWOTHIRDS,
                .ads_sensor_unit = "PSI",
                .input_low_val = 0.5,
                .input_high_val = 4.5,
                .output_low_val = 0.0,
                .output_high_val = 60.0,
            },
            .mqtt{
                .temp_sensor_id = "TEMP_conicalBeer",
                .server_setpoint_input = "setpoint_input-2",
                .server_sensor_input = "mqtt_input-2",
            },
        },
        {
            .spunder{
                .spunder_id = "spunder-3",
                .desired_vols = 3.43,
                .relay_pin = RELAY_PIN3,
            },
            .ads1115{
                .i2c_addr = ADS1115_ADDRESS1,
                .ads_channel = 2,
                .ads_gain = GAIN_TWOTHIRDS,
                .ads_sensor_unit = "PSI",
                .input_low_val = 0.5,
                .input_high_val = 4.5,
                .output_low_val = 0.0,
                .output_high_val = 30.0,
            },
            .mqtt{
                .temp_sensor_id = "TEMP_conicalBeer",
                .server_setpoint_input = "setpoint_input-3",
                .server_sensor_input = "mqtt_input-3",
            },
        },
        {
            .spunder{
                .spunder_id = "spunder-4",
                .desired_vols = 3.44,
                .relay_pin = RELAY_PIN4,
            },
            .ads1115{
                .i2c_addr = ADS1115_ADDRESS1,
                .ads_channel = 3,
                .ads_gain = GAIN_TWOTHIRDS,
                .ads_sensor_unit = "PSI",
                .input_low_val = 0.5,
                .input_high_val = 4.5,
                .output_low_val = 0.0,
                .output_high_val = 60.0,
            },
            .mqtt{
                .temp_sensor_id = "TEMP_conicalBeer",
                .server_setpoint_input = "setpoint_input-4",
                .server_sensor_input = "mqtt_input-4",
            },
    },
};

// std::vector<ads_sensor_cfg_t> ads_cfgs{
//     {
//         .i2c_addr = ADS1115_ADDRESS1,
//         .ads_channel = 0,
//         .ads_gain = GAIN_TWOTHIRDS,
//         .ads_sensor_unit = "PSI",
//         .input_low_val = 0.5,
//         .input_high_val = 4.5,
//         .output_low_val = 0.0,
//         .output_high_val = 60.0,
//     },
//     {
//         .i2c_addr = ADS1115_ADDRESS1,
//         .ads_channel = 1,
//         .ads_gain = GAIN_TWOTHIRDS,
//         .ads_sensor_unit = "PSI",
//         .input_low_val = 0.5,
//         .input_high_val = 4.5,
//         .output_low_val = 0.0,
//         .output_high_val = 60.0,
//     },
//     {
//         .i2c_addr = ADS1115_ADDRESS1,
//         .ads_channel = 2,
//         .ads_gain = GAIN_TWOTHIRDS,
//         .ads_sensor_unit = "PSI",
//         .input_low_val = 0.5,
//         .input_high_val = 4.5,
//         .output_low_val = 0.0,
//         .output_high_val = 60.0,
//     },
//     {
//         .i2c_addr = ADS1115_ADDRESS1,
//         .ads_channel = 3,
//         .ads_gain = GAIN_TWOTHIRDS,
//         .ads_sensor_unit = "PSI",
//         .input_low_val = 0.5,
//         .input_high_val = 4.5,
//         .output_low_val = 0.0,
//         .output_high_val = 60.0,
//     },
// };
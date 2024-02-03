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

// WiFi credentials
constexpr auto _SSID{SECRET_SSID};
constexpr auto _PASS{SECRET_PASS};

// MQTT parameters
constexpr auto _MQTTHOST = "10.0.0.113";
constexpr auto _MQTTPORT = 1883;
constexpr auto _CLIENTID = "spund-system";
constexpr auto _SUBTOPIC = "brewcast/history/spark-four";
constexpr auto _PUBTOPIC = "brewcast/history/spund-system";

// I2C pins
constexpr uint8_t _I2C_SCL = 25;
constexpr uint8_t _I2C_SDA = 26;

std::vector<spund_system_cfg_t> spund_cfgs{
    {
        .spunder{
            .spunder_id = "spunder-1",
            .desired_vols = 3.41,
            .relay_pin = 14,
        },
        .ads1115{
            .ads_cfg{
                .ads_addr = ADS1115_ADDRESS1,
                .ads_gain = GAIN_TWOTHIRDS,
                .i2c_sda = _I2C_SDA,
                .i2c_scl = _I2C_SCL,
                .ads_channel = 0,
                .input_low_range = .5,
                .input_high_range = 4.5,
                .output_low_range = 0.0,
                .output_high_range = 60.0,
                .offset_volts = 0.0,
            },
        },
        .mqtt{
            .temp_sensor_id = "Mock_temp-1",
            .server_setpoint_input = "setpoint_input-1",
            .server_sensor_input = "mqtt_input-1",
        },
    },
    {
        .spunder{
            .spunder_id = "spunder-2",
            .desired_vols = 3.42,
            .relay_pin = 27,
        },
        .ads1115{
            .ads_cfg{
                .ads_addr = ADS1115_ADDRESS1,
                .ads_gain = GAIN_TWOTHIRDS,
                .i2c_sda = _I2C_SDA,
                .i2c_scl = _I2C_SCL,
                .ads_channel = 1,
                .input_low_range = .5,
                .input_high_range = 4.5,
                .output_low_range = 0.0,
                .output_high_range = 60.0,
                .offset_volts = 0.0,
            },
        },
        .mqtt{
            .temp_sensor_id = "Mock_temp-2",
            .server_setpoint_input = "setpoint_input-2",
            .server_sensor_input = "mqtt_input-2",
        },
    },
    {
        .spunder{
            .spunder_id = "spunder-3",
            .desired_vols = 3.43,
            .relay_pin = 16,
        },
        .ads1115{
            .ads_cfg{
                .ads_addr = ADS1115_ADDRESS1,
                .ads_gain = GAIN_TWOTHIRDS,
                .i2c_sda = _I2C_SDA,
                .i2c_scl = _I2C_SCL,
                .ads_channel = 2,
                .input_low_range = .5,
                .input_high_range = 4.5,
                .output_low_range = 0.0,
                .output_high_range = 30.0,
                .offset_volts = 0.0,
            },
        },
        .mqtt{
            .temp_sensor_id = "Mock_temp-3",
            .server_setpoint_input = "setpoint_input-3",
            .server_sensor_input = "mqtt_input-3",
        },
    },
    {
        .spunder{
            .spunder_id = "spunder-4",
            .desired_vols = 3.44,
            .relay_pin = 17,
        },
        .ads1115{
            .ads_cfg{
                .ads_addr = ADS1115_ADDRESS1,
                .ads_gain = GAIN_TWOTHIRDS,
                .i2c_sda = _I2C_SDA,
                .i2c_scl = _I2C_SCL,
                .ads_channel = 3,
                .input_low_range = .5,
                .input_high_range = 4.5,
                .output_low_range = 0.0,
                .output_high_range = 60.0,
                .offset_volts = 0.0,
            },
        },
        .mqtt{
            .temp_sensor_id = "Mock_temp-4",
            .server_setpoint_input = "setpoint_input-4",
            .server_sensor_input = "mqtt_input-4",
        },
    },
};

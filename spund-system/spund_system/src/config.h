#pragma once

#include "Spund_System.h"

#include <vector>

/* Output message option
true = publish over mqtt
false = print to serial, to be read by docker container
*/
constexpr bool _PUBLISHMQTT = true;

// I2C pins
constexpr uint8_t _I2C_SCL = 25;
constexpr uint8_t _I2C_SDA = 26;

// MQTT parameters
constexpr auto _MQTTHOST = "192.168.1.2";
constexpr auto _MQTTPORT = 1883;
constexpr auto _CLIENTID = "spund-system";
constexpr auto _SUBTOPIC = "brewcast/history/spark-one";
constexpr auto _PUBTOPIC = "brewcast/history/spund-system";

std::vector<spund_system_cfg_t> spund_cfgs{
    {
        .spunder{
            .spunder_id = "spunder-1",
            .desired_vols = 3.41,
            .relay_pin = 14,
        },
        .ads1115{
            .ads_addr = ADS1115_ADDRESS1,
            .ads_gain = GAIN_TWOTHIRDS,
            .i2c_sda = _I2C_SDA,
            .i2c_scl = _I2C_SCL,
            .ads_channel = 0,
        },
        .sensor{
            .min_sensor_volts = .5,
            .max_sensor_volts = 4.5,
            .max_sensor_psi = 60.0,
            .sensor_offset_volts = 0.00,
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
            .relay_pin = 27,
        },
        .ads1115{
            .ads_addr = ADS1115_ADDRESS1,
            .ads_gain = GAIN_TWOTHIRDS,
            .i2c_sda = _I2C_SDA,
            .i2c_scl = _I2C_SCL,
            .ads_channel = 1,
        },
        .sensor{
            .min_sensor_volts = .5,
            .max_sensor_volts = 4.5,
            .max_sensor_psi = 60.0,
            .sensor_offset_volts = 0.02,
        },
        .mqtt{
            .temp_sensor_id = "TEMP_glycolReservoir",
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
            .ads_addr = ADS1115_ADDRESS1,
            .ads_gain = GAIN_TWOTHIRDS,
            .i2c_sda = _I2C_SDA,
            .i2c_scl = _I2C_SCL,
            .ads_channel = 2,
        },
        .sensor{
            .min_sensor_volts = .5,
            .max_sensor_volts = 4.5,
            .max_sensor_psi = 60.0,
            .sensor_offset_volts = 0.0,
        },
        .mqtt{
            .temp_sensor_id = "TEMP_mock1",
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
            .ads_addr = ADS1115_ADDRESS1,
            .ads_gain = GAIN_TWOTHIRDS,
            .i2c_sda = _I2C_SDA,
            .i2c_scl = _I2C_SCL,
            .ads_channel = 3,
        },
        .sensor{
            .min_sensor_volts = .5,
            .max_sensor_volts = 4.5,
            .max_sensor_psi = 60.0,
            .sensor_offset_volts = 0.0,
        },
        .mqtt{
            .temp_sensor_id = "TEMP_mock2",
            .server_setpoint_input = "setpoint_input-4",
            .server_sensor_input = "mqtt_input-4",
        },
    },
};

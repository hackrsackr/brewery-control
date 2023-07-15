#pragma once

#include "FlowMeter.hpp"
#include "secrets.h"

#include <vector>

constexpr auto _SSID = SECRET_SSID;
constexpr auto _PASS = SECRET_PASS;
constexpr auto _MQTTHOST = "192.168.1.2";
constexpr auto _MQTTPORT = 1883;
constexpr auto _CLIENTID = "DEV-FlowMeters";
// constexpr auto _SUBTOPIC = "brewcast/history/spark-dev";
constexpr auto _PUBTOPIC = "brewcast/history/DEV-FlowMeters";
/*
    [Q * 23 - 3]
    (60*23)-(23*3) = 1311   bits/liter/min
    1311 / 60 = 21.85       bits/liter/sec
*/
constexpr auto _YF_S302 = 21.85;
/*
    [Q * 32]
    60 * 32 = 1920 bits/liter/min
    1920 / 60 = 32 bits/liter/sec
*/
constexpr auto _YFS402B = 32;
/*
    [Q * 9.93]
    60 * 9.93 = 596 bits/liter/min
    596 / 60 = 9.93 bits/liter/sec
*/
constexpr auto _SS_FLOW = 9.93;

std::vector<flowmeter_cfg_t> FLOW_CFGS{
    {
        .flow{
            .id = "flow-meter-1",
            .sensor_pin = 35,
            .calibration_factor = _YFS402B,
            .percent_correction_factor = 1.00,
        },
    },
    {
        .flow{
            .id = "flow-meter-2",
            .sensor_pin = 37,
            .calibration_factor = _YFS402B,
            .percent_correction_factor = 1.00,
        },
    },
    {
        .flow{
            .id = "flow-meter-3",
            .sensor_pin = 36,
            .calibration_factor = _YFS402B,
            .percent_correction_factor = 1.00,
        },
    },
};

#pragma once

#include "FlowMeter.hpp"
#include "secrets.h"

#include <vector>

constexpr auto _SSID = SECRET_SSID;
constexpr auto _PASS = SECRET_PASS;
constexpr auto _MQTTHOST = "192.168.1.2";
constexpr auto _MQTTPORT = 1883;
constexpr auto _CLIENTID = "flow-meters";
constexpr auto _PUBTOPIC = "brewcast/history/flow-meters";

// Calibration factors
/*
    (hz) = [Q * 21.85]
    60 * 21.85 = 1311       pulses/minute/liter
    1311 / 60 = 21.85       pulses/second/liter
*/
constexpr auto _YF_S302 = 21.85;
/*
    (hz) = [Q * 32]
    60 * 32 = 1920 	    pulses/minute/liter
    1920 / 60 = 32          pulses/second/liter
*/
constexpr auto _YFS402B = 32;
/*
    (hz) = [Q * 9.93]
    60 * 9.93 = 596         pulses/minute/liter
    596 / 60 = 9.93         pulses/second/liter
*/
constexpr auto _SS_FLOW1 = 9.93;
/*
    (hz) = [Q * 17.95] 
    60 * 17.95 = 1077	    pulses/minute/liter
    1077 / 60 = 17.95       pulses/second/liter
*/
constexpr auto _SS_FLOW2 = 17.95;

std::vector<flowmeter_cfg_t> FLOW_CFGS{
    {
//        .flow{
//            .id = "main",
//            .sensor_pin = 34,
//            .calibration_factor = _SS_FLOW2,
//            .percent_correction_factor = 1.05,
//        },
//    },
//    {
        .flow{
            .id = "liqr",
            .sensor_pin = 35,
            .calibration_factor = _SS_FLOW1,
            .percent_correction_factor = 1.05,
        },
    },
};

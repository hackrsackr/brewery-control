#pragma once

#include "FlowMeter.hpp"
#include "secrets.h"

#include <vector>

constexpr auto _SSID = SECRET_SSID;
constexpr auto _PASS = SECRET_PASS;

constexpr bool _PUBLISHMQTT = true;
constexpr auto _MQTTHOST = "10.0.0.101";
constexpr auto _MQTTPORT = 1883;
constexpr auto _CLIENTID = "flow-meters";
constexpr auto _PUBTOPIC = "brewcast/history/flow-meters";

// Calibration factors
/*
    FLOW_CHARACTERISTICS_CODE: YF-S302
    1311 pulses = liter

    f(hz) = Q(L/s) * 1311
    f(hz) = [Q(L/m) * 1311] / 60 -> f(hz) = Q(L/m) * 21.85
    f(hz) = [Q(L/h) * 1311] / 60 / 60 -> f(hz) = Q(L/h) * 0.3642

    Q(L/s) = f(hz) / 1311
    Q(L/m) = [f(hz) / 1311] / 60 -> Q(L/m) = f(hz) / 21.85
    Q(L/h) = [f(hz) / 1311] / 60 / 60 -> Q(L/h) = f(hz) / 0.3642
*/
constexpr auto _YF_S302 = 21.85; // Q(L/m)

/*
    FLOW_CHARACTERISTICS_CODE: YF-S402-B
    1920 pulses = 1 liter

    f(hz) = Q(L/s) * 1920
    f(hz) = [Q(L/m) * 1920] / 60 -> f(hz) = Q(L/m) * 32
    f(hz) = [Q(L/h) * 1920] / 60 / 60 -> f(hz) = Q(L/h) * 0.5333

    Q(L/s) = f(hz) / 1920
    Q(L/m) = [f(hz) / 1920] * 60 -> Q(L/m) = f(hz) / 32
    Q(L/h) = [f(hz) / 1920] * 60 * 60 -> Q(L/h) = f(hz) / 0.5333
*/
constexpr auto _YFS402B = 32; // Q(L/m)

/*
    FLOW_CHARACTERISTICS_CODE:
    596 pulses = 1 liter

    f(hz) = Q(L/s) * 596
    f(hz) = [Q(L/m) * 596] / 60 -> f(hz) = Q(L/m) * 9.93
    f(hz) = [Q(L/h) * 596] / 60 / 60 -> f(hz) = Q(L/h) * 0.1655

    Q(L/s) = f(hz) / 596
    Q(L/m) = [f(hz) / 596] * 60 -> Q(L/m) = f(hz) / 9.93
    Q(L/h) = [f(hz) / 596] * 60 * 60 -> Q(L/h) = f(hz) / 0.1655
*/
constexpr auto _SS_FLOW1 = 9.93;

/*
    FLOW_CHARACTERISTICS_CODE: YF-B1-S
    1077 pulses = 1 liter

    f(hz) = Q(L/s) * 1077
    f(hz) = [Q(L/m) * 1077] / 60 -> f(hz) = Q(L/m) * 17.95
    f(hz) = [Q(L/h) * 1077] / 60 / 60 -> f(hz) = Q(L/h) * 0.2992

    Q(L/s) = f(hz) / 1077
    Q(L/m) = [f(hz) / 1077] * 60 -> Q(L/m) = f(hz) / 17.95
    Q(L/h) = [f(hz) / 1077] * 60 * 60 -> Q(L/h) = f(hz) / 0.2992
*/
constexpr auto _SS_FLOW2 = 17.95; // Q(L/m)

// Production: dev-kit
std::vector<flowmeter_cfg_t> FLOW_CFGS{
    {
            .id = "main",
            .sensor_pin = 34,
            .calibration_factor = _SS_FLOW1,
            .percent_correction_factor = 1.00,
    },
    {
            .id = "liqr",
            .sensor_pin = 35,
            .calibration_factor = _SS_FLOW1,
            .percent_correction_factor = 1.10,
    },
};

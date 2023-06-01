#pragma once

#include "secrets.h"

// Network setup
#define _SSID SECRET_SSID
#define _PASS SECRET_PASS
#define _MQTTHOST "192.168.1.2"
#define _MQTTPORT 1883
#define _CLIENTID "flow-meters"
#define _PUBTOPIC "brewcast/history/flow-meters"

// #define _MQTTHOST "192.168.1.6"
// #define _CLIENTID "flow-meters-TEST"
// #define _PUBTOPIC "brewcast/history/flow-meters-TEST"

// Flowmeter setup
// Names
#define _FLOW1 "flow-meter-1"
#define _FLOW2 "flow-meter-2"

// Sensor pins
#define _SPIN1 34
#define _SPIN2 35
// #define _SPIN1 7
// #define _SPIN2 8

// Calibration Factors
// [23*Q-3]
// f = 23*60-(23*3) = 1311 bits per liter
// 1311/60 = 21.85
#define _YF_S302 21.85

// f = Q*32 32*60 = 1920 bits per liter
// 1920 / 60 = 32.00
#define _YFS402B 32.00

// 596 bits per liter 596/60 = 9.93.
// correction_factor = .95 * 9.93 = 9.4335
// #define _SS_FLOW 9.4335
#define _SS_FLOW 9.93

// Correction Factors
#define _CORR1 1.00
#define _CORR2 1.00
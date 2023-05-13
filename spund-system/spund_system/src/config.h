#pragma once

#include <array>

#include "secrets.h"

#define _NUMBER_OF_SPUNDERS 4

// WiFi
#define _SSID SECRET_SSID
#define _PASS SECRET_PASS

// I2C Pins
#define _I2C_SCL 25
#define _I2C_SDA 26

// MQTT Setup
#define _MQTTHOST "192.168.1.2"
#define _MQTTPORT 1883
#define _CLIENTID "spund-system"
#define _SUBTOPIC "brewcast/history/spark-two"
#define _PUBTOPIC "brewcast/history/spund-system"

///////////////////////////// SPUNDER CONFIG ///////////////////////////////////

// Names
std::array<const String, _NUMBER_OF_SPUNDERS> _SPUNDER_NAMES = {
    "spunder-1",
    "spunder-2",
    "spunder-3",
    "spunder-4"};

// Desired volumes of carbonation
std::array<double, _NUMBER_OF_SPUNDERS> _DESIRED_VOLS = {
    2.20,
    2.20,
    2.20,
    2.20};

// ESP32 pins attached to relays for venting
std::array<const uint8_t, _NUMBER_OF_SPUNDERS> _RELAY_PINS = {
    14,
    27,
    16,
    17};

// Brewblox names of temps to request for each spunder mqtt_temp_field
std::array<String, _NUMBER_OF_SPUNDERS> _MQTT_FIELDS = {
    "TEMP_blueBeer",
    "TEMP_orangeBeer",
    "TEMP_ambientRoom",
    "TEMP_ambientRoom"};
////////////////////////////////////////////////////////////////////////////////

////////////////////////// PRESSURE_SENSOR CONFIG //////////////////////////////

std::array<const uint8_t, _NUMBER_OF_SPUNDERS> _ADS_CHANNELS = {
    0,
    1,
    2,
    3};

// Voltage at 0 PSI
std::array<const double, _NUMBER_OF_SPUNDERS> _OFFSET_VOLTS = {
    .44,
    .44,
    .46,
    .46};

// Max rated pressure in PSI
// std::array<const uint8_t, _NUMBER_OF_SPUNDERS> _UNIT_MAXS = {
//     60,
//     60,
//     60,
//     60};

// Max rated pressure in PSI
std::array<const double, _NUMBER_OF_SPUNDERS> _UNIT_MAXS = {
    60.0,
    60.0,
    60.0,
    60.0};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////// SERVER CONFIG ///////////////////////////////////

// Server setpoint message fields
std::array<String, _NUMBER_OF_SPUNDERS> _SETPOINT_MESSAGES = {
    String(_DESIRED_VOLS[0]),
    String(_DESIRED_VOLS[1]),
    String(_DESIRED_VOLS[2]),
    String(_DESIRED_VOLS[3])};

// Server mqtt temp message fields
std::array<String, _NUMBER_OF_SPUNDERS> _MQTT_MESSAGES = {
    _MQTT_FIELDS[0],
    _MQTT_FIELDS[1],
    _MQTT_FIELDS[2],
    _MQTT_FIELDS[3]};

// Server setpoint inputs
std::array<String, _NUMBER_OF_SPUNDERS> _SETPOINT_INPUTS = {
    "setpoint_input1",
    "setpoint_input2",
    "setpoint_input3",
    "setpoint_input4"};

// Server mqtt temp field inputs
std::array<String, _NUMBER_OF_SPUNDERS> _MQTT_INPUTS = {
    "mqtt_input1",
    "mqtt_input2",
    "mqtt_input3",
    "mqtt_input4"};
////////////////////////////////////////////////////////////////////////////////
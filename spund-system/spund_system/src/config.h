#pragma once

#include <array>

#include "secrets.h"

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// SPUNDER CONFIG ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define _NUMBER_OF_SPUNDERS 4

#define _NAME1 "spunder-1"
#define _NAME2 "spunder-2"
#define _NAME3 "spunder-3"
#define _NAME4 "spunder-4"

std::array<const String, _NUMBER_OF_SPUNDERS>
    _SPUNDER_NAMES = {_NAME1, _NAME2, _NAME3, _NAME4};

// Desired vols of CO2 for each spunder
float _VOLS1 = 2.20;
float _VOLS2 = 2.20;
float _VOLS3 = 2.20;
float _VOLS4 = 2.20;

std::array<float, _NUMBER_OF_SPUNDERS>
    _DESIRED_VOLS = {_VOLS1, _VOLS2, _VOLS3, _VOLS4};
////////////////////////////////////////////////////////////////////////////////
///////////////////////////// END OF SPUNDER ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// MQTT CONFIG /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define _SSID SECRET_SSID
#define _PASS SECRET_PASS

#define _MQTTHOST "192.168.1.2"
#define _MQTTPORT 1883
#define _CLIENTID "spund-system"
#define _SUBTOPIC "brewcast/history/spark-two"
#define _PUBTOPIC "brewcast/history/spund-system"

// Brewblox names of temps to request for each spunder mqtt_temp_field
String _TEMP1 = "TEMP_blueBeer";
String _TEMP2 = "TEMP_orangeBeer";
String _TEMP3 = "TEMP_ambientRoom";
String _TEMP4 = "TEMP_ambientRoom";

std::array<String, _NUMBER_OF_SPUNDERS>
    _MQTT_FIELDS = {_TEMP1, _TEMP2, _TEMP3, _TEMP4};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////// END OF MQTT /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// RELAY CONFIG ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// esp32 pins to activate relays
#define _RPIN1 14
#define _RPIN2 27
#define _RPIN3 16
#define _RPIN4 17

std::array<const uint8_t, _NUMBER_OF_SPUNDERS>
    _RELAY_PINS = {_RPIN1, _RPIN2, _RPIN3, _RPIN4};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////// END OF RELAY ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////// PRESSURE_SENSOR CONFIG //////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Voltage at 0 PSI
#define _OFFSET_VOLTS1 .55
#define _OFFSET_VOLTS2 .55
#define _OFFSET_VOLTS3 .55
#define _OFFSET_VOLTS4 .55

std::array<const float, _NUMBER_OF_SPUNDERS>
    _OFFSET_VOLTS = {_OFFSET_VOLTS1, _OFFSET_VOLTS2, _OFFSET_VOLTS3, _OFFSET_VOLTS4};

// Pressure in PSI at 5V
#define _UNIT_MAX1 60
#define _UNIT_MAX2 60
#define _UNIT_MAX3 60
#define _UNIT_MAX4 60

std::array<const uint8_t, _NUMBER_OF_SPUNDERS>
    _UNIT_MAXS = {_UNIT_MAX1, _UNIT_MAX2, _UNIT_MAX3, _UNIT_MAX4};

// ADS1115 Addresses
#define ADS1115_ADDRESS1 (0x48) // ADDR -> GND
#define ADS1115_ADDRESS2 (0x49) // ADDR -> VCC
#define ADS1115_ADDRESS3 (0x4a) // ADDR -> SDA
#define ADS1115_ADDRESS4 (0x4b) // ADDR -> SCL

#define I2C_SCL 25
#define I2C_SDA 26

// ADS1115 Gains reference table
/*
GAIN_TWOTHIRDS    = 0 - 6.144V
GAIN_ONE          = 0 - 4.096V
GAIN_TWO          = 0 - 2.048V
GAIN_FOUR         = 0 - 1.024V
GAIN_EIGHT        = 0 - 0.512V
GAIN_SIXTEEN      = 0 - 0.256V
*/
////////////////////////////////////////////////////////////////////////////////
////////////////////////// END OF PRESSURE_SENSOR //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// SERVER CONFIG ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Server setpoint message fields
String _SPMS1 = String(_DESIRED_VOLS[0]);
String _SPMS2 = String(_DESIRED_VOLS[1]);
String _SPMS3 = String(_DESIRED_VOLS[2]);
String _SPMS4 = String(_DESIRED_VOLS[3]);

std::array<String, _NUMBER_OF_SPUNDERS>
    _SETPOINT_MESSAGES = {_SPMS1, _SPMS2, _SPMS3, _SPMS4};

// Server mqtt temp message fields
String _MQMS1 = _MQTT_FIELDS[0];
String _MQMS2 = _MQTT_FIELDS[1];
String _MQMS3 = _MQTT_FIELDS[2];
String _MQMS4 = _MQTT_FIELDS[3];

std::array<String, _NUMBER_OF_SPUNDERS>
    _MQTT_MESSAGES = {_MQMS1, _MQMS2, _MQMS3, _MQMS4};

// Server setpoint inputs
#define _SPIN1 "setpoint_input1"
#define _SPIN2 "setpoint_input2"
#define _SPIN3 "setpoint_input3"
#define _SPIN4 "setpoint_input4"

std::array<const char *, _NUMBER_OF_SPUNDERS>
    _SETPOINT_INPUTS = {_SPIN1, _SPIN2, _SPIN3, _SPIN4};

// Server mqtt temp field inputs
#define _MQIN1 "mqtt_input1"
#define _MQIN2 "mqtt_input2"
#define _MQIN3 "mqtt_input3"
#define _MQIN4 "mqtt_input4"

std::array<const char *, _NUMBER_OF_SPUNDERS>
    _MQTT_INPUTS = {_MQIN1, _MQIN2, _MQIN3, _MQIN4};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////// END OF SERVER ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
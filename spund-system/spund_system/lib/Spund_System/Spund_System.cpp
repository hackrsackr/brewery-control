#include "Relay.h"
#include "Spund_System.h"

#include "config.h"

Spund_System::Spund_System(spund_system_cfg_t cfg)
{
    // .spunder
    spunder_id = cfg.spunder.spunder_id;
    desired_vols = cfg.spunder.desired_vols;
    _relay_pin = cfg.spunder.relay_pin;

    // .ads1115
    _i2c_addr = cfg.ads1115.i2c_addr;
    _ads_channel = cfg.ads1115.ads_channel;
    _ads_sensor_unit = cfg.ads1115.ads_sensor_unit;
    _input_low_val = cfg.ads1115.input_low_val;
    _input_high_val = cfg.ads1115.input_high_val;
    _output_low_val = cfg.ads1115.output_low_val;
    _output_high_val = cfg.ads1115.output_high_val;

    // .mqtt
    temp_sensor_id = cfg.mqtt.temp_sensor_id;
    server_setpoint_input = cfg.mqtt.server_setpoint_input;
    server_sensor_input = cfg.mqtt.server_sensor_input;

    // server variables
    server_setpoint = desired_vols;
    server_sensor = temp_sensor_id;
    _time_of_last_vent = millis();

    _p_ads = std::make_shared<Adafruit_ADS1115>();
    _p_ads->begin();

    _p_re = std::make_shared<Relay>();
    _p_re->begin(_relay_pin);
}

auto Spund_System::begin() -> bool
{
    return _p_ads->begin(_i2c_addr);
}

auto Spund_System::getSensorUnit() -> std::string
{
    return _ads_sensor_unit;
}

auto Spund_System::readADC() -> uint16_t
{
    return _p_ads->readADC_SingleEnded(_ads_channel);
}

auto Spund_System::readVolts() -> float
{
    return _p_ads->computeVolts(_p_ads->readADC_SingleEnded(_ads_channel));
}

auto Spund_System::readSensorUnits() -> float
{
    return (readVolts() - _input_low_val) / (_input_high_val - _input_low_val) * (_output_high_val - _output_low_val);
}

auto Spund_System::computePSISetpoint() -> float
{
    double a = -16.669 - (.0101059 * tempF) + (.00116512 * (tempF * tempF));
    double b = .173354 * tempF * desired_vols;
    double c = (4.24267 * desired_vols) - (.0684226 * (desired_vols * desired_vols));

    _psi_setpoint = a + b + c;

    return _psi_setpoint;
}

auto Spund_System::computeVols() -> float
{
    double a = -.0684226;
    double b = ((.173354 * tempF) + 4.24267);
    double c = (-readSensorUnits() + -16.669 + (-0.0101059 * tempF) + (0.00116512 * tempF * tempF));
    double d = ((b * b) - (4 * a * c));

    _vols = ((-b + (pow(d, .5))) / (2 * a));

    return _vols;
}

auto Spund_System::testForVent() -> uint8_t
{
    uint8_t vented;

    if (_vols > desired_vols)
    {
        _p_re->openRelay();
        delay(500);
        _time_of_last_vent = millis();
        _p_re->closeRelay();
        vented = 1;
    }
    else
    {
        vented = 0;
    }

    return vented;
}

auto Spund_System::getLastVent() -> float
{
    constexpr auto MILLISECONDS_PER_MINUTE = 60000;

    auto minutes_since_vent = (millis() - _time_of_last_vent) / MILLISECONDS_PER_MINUTE;

    return minutes_since_vent;
}
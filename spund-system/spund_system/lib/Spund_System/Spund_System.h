#pragma once

#include "ADS_Sensor.h"
#include "Relay.h"

typedef struct spund_system_cfg_t
{
    struct spund_cfg
    {
        String spunder_id;
        double desired_vols;
        uint8_t relay_pin;
    } spunder;

    struct ads_cfg
    {
        uint8_t i2c_addr;
        uint8_t ads_channel;
        adsGain_t ads_gain;
        std::string ads_sensor_unit;
        float input_low_val;
        float input_high_val;
        float output_low_val;
        float output_high_val;
    } ads1115;

    struct mqtt_cfg
    {
        String temp_sensor_id;
        String server_setpoint_input;
        String server_sensor_input;
    } mqtt;
} spund_system_cfg_t;

class Spund_System
{
public:
    String spunder_id;
    String temp_sensor_id;
    float desired_vols;
    String server_setpoint;
    String server_sensor;
    String server_setpoint_input;
    String server_sensor_input;
    float tempC;
    float tempF;

    Spund_System(spund_system_cfg_t);
    ~Spund_System();

    bool begin();

    // auto getSpunderId() -> String;
    // auto getTempSensorId() -> String;
    auto getSensorUnit() -> std::string;
    // auto getDesiredVols() -> float;

    void setDesiredVols(float);

    auto readADC() -> uint16_t;
    auto readVolts() -> float;
    auto readSensorUnits() -> float;

    auto computePSISetpoint() -> float;
    auto computeVols() -> float;

    auto testCarb() -> uint8_t;
    auto getLastVent() -> float;

private:
    // spund
    // String _spunder_id;
    // double _desired_vols;
    uint8_t _relay_pin;

    // ads
    uint8_t _i2c_addr;
    uint8_t _ads_channel;
    std::string _ads_sensor_unit;
    float _input_low_val;
    float _input_high_val;
    float _output_low_val;
    float _output_high_val;

    // mqtt
    // String _temp_sensor_id;

    // pointers
    std::shared_ptr<Adafruit_ADS1115> _p_ads;
    std::shared_ptr<Relay> _p_re;

    // data
    uint32_t _time_of_last_vent;
    double _psi_setpoint;
    double _vols;
};
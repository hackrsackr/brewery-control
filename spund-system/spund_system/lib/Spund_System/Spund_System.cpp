#include "ADS_Sensor.h"
#include "Relay.h"
#include "Spund_System.h"

Spund_System::Spund_System(spund_system_cfg_t cfg)
{
    spunder_id = cfg.spunder.spunder_id;
    desired_vols = cfg.spunder.desired_vols;
    relay_pin = cfg.spunder.relay_pin;

    ads_addr = cfg.ads1115.ads_addr;
    ads_gain = cfg.ads1115.ads_gain;
    i2c_sda = cfg.ads1115.i2c_sda;
    i2c_scl = cfg.ads1115.i2c_scl;
    ads_channel = cfg.ads1115.ads_channel;

    min_sensor_volts = cfg.sensor.min_sensor_volts;
    max_sensor_volts = cfg.sensor.max_sensor_volts;
    max_sensor_psi = cfg.sensor.max_sensor_psi;
    sensor_offset_volts = cfg.sensor.sensor_offset_volts;

    temp_sensor_id = cfg.mqtt.temp_sensor_id;
    server_setpoint_input = cfg.mqtt.server_setpoint_input;
    server_sensor_input = cfg.mqtt.server_sensor_input;

    server_setpoint = String(desired_vols);
    server_sensor = temp_sensor_id;
    time_of_last_vent = millis();

    s_ps_ = std::make_shared<ADS_Pressure_Sensor>();
    s_ps_->begin(
        ads_addr,
        ads_gain,
        i2c_sda,
        i2c_scl,
        ads_channel,
        min_sensor_volts,
        max_sensor_volts,
        max_sensor_psi,
        sensor_offset_volts);

    s_re_ = std::make_shared<Relay>();
    s_re_->begin(relay_pin);
}

double Spund_System::getVolts()
{
    return s_ps_->getADSVolts();
}

double Spund_System::getPSI()
{
    return s_ps_->computePSI();
}

double Spund_System::computePSISetpoint()
{
    double a = -16.669 - (.0101059 * tempF) + (.00116512 * (tempF * tempF));
    double b = .173354 * tempF * desired_vols;
    double c = (4.24267 * desired_vols) - (.0684226 * (desired_vols * desired_vols));

    psi_setpoint = a + b + c;

    return psi_setpoint;
}

double Spund_System::computeVols()
{
    double a = -.0684226;
    double b = ((.173354 * tempF) + 4.24267);
    double c = (-s_ps_->computePSI() + -16.669 + (-0.0101059 * tempF) + (0.00116512 * tempF * tempF));
    double d = ((b * b) - (4 * a * c));

    vols = ((-b + (pow(d, .5))) / (2 * a));

    return vols;
}

uint8_t Spund_System::testCarb()
{
    s_re_->relay_toggled = false;

    if (vols > desired_vols)
    {
        s_re_->openRelay();
        delay(500);
        time_of_last_vent = millis();
        s_re_->closeRelay();
    }

    return s_re_->relay_toggled;
}

double Spund_System::getLastVent()
{
    double minutes_since_vent = (millis() - time_of_last_vent) / 60000.0;

    return minutes_since_vent;
}
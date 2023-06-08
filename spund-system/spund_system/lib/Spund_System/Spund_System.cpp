#include "ADS_Sensor.h"
#include "Relay.h"
#include "Spund_System.h"

Spund_System::Spund_System() {}

Spund_System::Spund_System(Spund_Settings config)
{
    settings = config;

    spunder_id = settings.spunder_id;
    ads_addr = settings.ads_addr;
    ads_gain = settings.ads_gain;
    desired_vols = settings.desired_vols;
    relay_pin = settings.relay_pin;
    temp_sensor_id = settings.temp_sensor_id;
    ads_channel = settings.ads_channel;
    min_sensor_volts = settings.min_sensor_volts;
    max_sensor_volts = settings.max_sensor_volts;
    max_sensor_psi = settings.max_sensor_psi;
    server_setpoint_input = settings.server_setpoint_input;
    server_sensor_input = settings.server_sensor_input;
    i2c_sda = settings.i2c_sda;
    i2c_scl = settings.i2c_scl;

    server_setpoint = String(desired_vols);
    server_sensor = temp_sensor_id;
    time_of_last_vent = millis();

    s_ps = std::make_shared<ADS_Pressure_Sensor>();
    s_ps->begin(
        ads_addr,
        ads_gain,
        i2c_sda,
        i2c_scl,
        ads_channel,
        min_sensor_volts,
        max_sensor_volts,
        max_sensor_psi);

    s_re = std::make_shared<Relay>();
    s_re->begin(relay_pin);
}

Spund_System::Spund_System(
    uint8_t ads_addr,
    adsGain_t ads_gain,
    uint8_t i2c_sda,
    uint8_t i2c_scl,
    uint8_t ads_chan,
    double min_vs,
    double max_vs,
    double max_unit,
    uint8_t vent_pin)
{
    time_of_last_vent = millis();
    s_ps = std::make_shared<ADS_Pressure_Sensor>();
    s_ps->begin(ads_addr, ads_gain, i2c_sda, i2c_scl, ads_chan, min_vs, max_vs, max_unit);

    s_re = std::make_shared<Relay>();
    s_re->begin(vent_pin);
}

void Spund_System::begin(
    uint8_t ads_addr,
    adsGain_t ads_gain,
    uint8_t i2c_sda,
    uint8_t i2c_scl,
    uint8_t ads_chan,
    double min_vs,
    double max_vs,
    double max_unit,
    uint8_t vent_pin)
{
    time_of_last_vent = millis();

    s_ps = std::make_shared<ADS_Pressure_Sensor>();
    s_ps->begin(ads_addr, ads_gain, i2c_sda, i2c_scl, ads_chan, min_vs, max_vs, max_unit);

    s_re = std::make_shared<Relay>();
    s_re->begin(vent_pin);
}

double Spund_System::getVolts()
{
    return s_ps->getADSVolts();
}

double Spund_System::getPSI()
{
    return s_ps->computePSI();
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
    double c = (-s_ps->computePSI() + -16.669 + (-0.0101059 * tempF) + (0.00116512 * tempF * tempF));
    double d = ((b * b) - (4 * a * c));

    vols = ((-b + (pow(d, .5))) / (2 * a));

    return vols;
}

double Spund_System::test_carb()
{
    // if (vols > desired_vols && psi > psi_setpoint)
    if (vols > desired_vols)
    {
        s_re->openRelay();
        delay(500);
        time_of_last_vent = millis();
        s_re->closeRelay();
    }
    double minutes_since_vent = (millis() - time_of_last_vent) / 60000.0;

    return minutes_since_vent;
}
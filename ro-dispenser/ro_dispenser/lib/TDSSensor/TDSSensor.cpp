#include "Arduino.h"

#include "TDSSensor.hpp"

TDSSensor::TDSSensor(tds_sensor_cfg_t cfg)
{
    id = cfg.id;
    sensor_pin = cfg.sensor_pin;
    tds_factor = cfg.tds_factor;
}

TDSSensor::~TDSSensor()
{
}

void TDSSensor::readTDSSensor()
{
    static unsigned long tdsTimer = millis();

    // if (millis() - tdsTimer > 40U)
    if (tdsTimer)
    { // Sample every 40ms
        // tdsTimer = millis();

        // Read incoming water TDS sensor
        adc = analogRead(sensor_pin);
        voltage = adc * reference_voltage / 4096.0;
        EC = 133.42 * voltage * voltage * voltage - 255.86 * voltage * voltage + 857.39 * voltage;
        TDS = EC * tds_factor;
    }
}
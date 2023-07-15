#include <Arduino.h>
#include <FunctionalInterrupt.h>

#include "FlowMeter.hpp"

const unsigned long MIN_INTERVAL = 1000; // Minimum interval between flow events (in microseconds)

FlowMeter::FlowMeter(flowmeter_cfg_t cfg)
{
    id = cfg.flow.id;
    sensor_pin = cfg.flow.sensor_pin;
    calibration_factor = cfg.flow.calibration_factor;
    percent_correction_factor = cfg.flow.percent_correction_factor;
    total_pulse_count = 0;
    pulse_count = 0;

    pinMode(sensor_pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(sensor_pin), std::bind(&FlowMeter::pulseCounter, this), RISING);
}

FlowMeter::~FlowMeter()
{
    detachInterrupt(sensor_pin);
}

void FlowMeter::begin()
{
    total_pulse_count = 0;
    pulse_count = 0;
    pinMode(sensor_pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(sensor_pin), std::bind(&FlowMeter::pulseCounter, this), RISING);
}

void FlowMeter::attachISR()
{
    attachInterrupt(digitalPinToInterrupt(sensor_pin), std::bind(&FlowMeter::pulseCounter, this), RISING);
}
void FlowMeter::run()
{
    if ((millis() - old_time) > MIN_INTERVAL)
    {
        detachInterrupt(sensor_pin);
        get_flow_rate();
        old_time = millis();
        total_pulse_count += pulse_count;
        total_liters = total_pulse_count / (calibration_factor * 60);
        total_mLs = total_liters * 1000;
        pulse_count = 0;
    }
}

void FlowMeter::pulseCounter()
{
    pulse_count++;
}

double FlowMeter::get_frequency()
{
    return frequency = (float(MIN_INTERVAL) / (millis() - old_time)) * pulse_count;
}

double FlowMeter::get_flow_rate()
{
    return flow_rate = (get_frequency() / calibration_factor) * percent_correction_factor;
}

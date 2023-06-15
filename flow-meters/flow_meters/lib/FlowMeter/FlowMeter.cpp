#include <Arduino.h>

#include "FlowMeter.hpp"

const unsigned long MIN_INTERVAL = 1000000; // Minimum interval between flow events (in microseconds)

FlowMeter::FlowMeter(flowmeter_cfg_t cfg)
{
    flowmeter_id = cfg.flow.flowmeter_id;
    sensor_pin = cfg.flow.sensor_pin;
    calibration_factor = cfg.flow.calibration_factor;
    percent_correction_factor = cfg.flow.percent_correction_factor;
}

double FlowMeter::get_frequency()
{
    frequency = (float(MIN_INTERVAL) / (micros() - old_time)) * pulse_count;

    return frequency;
}

double FlowMeter::get_flow_rate()
{
    flow_rate = (get_frequency() / calibration_factor) * percent_correction_factor;

    return flow_rate;
}

void FlowMeter::run()
{

    if ((micros() - old_time) > MIN_INTERVAL)
    {
        detachInterrupt(sensor_pin);
        get_flow_rate();
        old_time = micros();
        total_pulse_count += pulse_count;
        total_liters = total_pulse_count / (calibration_factor * 60);
        total_mLs = total_liters * 1000;
        pulse_count = 0;
    }
}
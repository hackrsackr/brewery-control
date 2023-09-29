#include <Arduino.h>
#include <FunctionalInterrupt.h>
#include "FlowMeter.hpp"

const unsigned long MIN_INTERVAL = 1000000.0; // Minimum interval between flow events (in microseconds)

FlowMeter::FlowMeter(flowmeter_cfg_t cfg)
{
    id = cfg.flow.id;
    sensor_pin = cfg.flow.sensor_pin;
    calibration_factor = cfg.flow.calibration_factor;
    percent_correction_factor = cfg.flow.percent_correction_factor;

    pulse_count = 0;
    flow_rate = 0.0;
    flow_milliliters = 0;
    total_milliliters = 0;
    old_time = 0;

    pinMode(sensor_pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(sensor_pin), std::bind(&FlowMeter::pulseCounter, this), RISING);
}

FlowMeter::~FlowMeter()
{
    detachInterrupt(sensor_pin);
}

void FlowMeter::attachPinInt()
{
    attachInterrupt(digitalPinToInterrupt(sensor_pin), std::bind(&FlowMeter::pulseCounter, this), RISING);
}

void FlowMeter::run()
{
    if ((micros() - old_time) > MIN_INTERVAL)
    {
        detachInterrupt(sensor_pin);
        getFlowRate();
        old_time = micros();
        flow_milliliters = (flow_rate / 60) * 1000;
        total_milliliters += flow_milliliters;
        total_liters = total_milliliters / 1000.0;
        // total_pulse_count += pulse_count;
        // total_liters = total_pulse_count / (calibration_factor * 60);
        // total_mLs = total_liters * 1000;
        pulse_count = 0;
        attachPinInt();
    }
}

void FlowMeter::pulseCounter()
{
    pulse_count++;
}

double FlowMeter::getFrequency()
{
    return frequency = (float(MIN_INTERVAL) / (micros() - old_time)) * pulse_count;
}

double FlowMeter::getFlowRate()
{
    return flow_rate = (getFrequency() / calibration_factor) * percent_correction_factor;
}

#include <Arduino.h>

#include "flow_config.hpp"
#include "FlowMeter/FlowMeter.hpp"

FlowMeter::FlowMeter(int sp, std::string nm, float cf)
{
    sensor_pin = sp;
    name = nm;
    cal_factor = cf;
}

FlowMeter::FlowMeter(int sp, std::string nm, float cf, float cr)
{
    sensor_pin = sp;
    name = nm;
    cal_factor = cf;
    cor_factor = cr;
}

void FlowMeter::reset_total() { total_mLs = 0; }

void FlowMeter::set_sensor_pin(uint8_t pin) { sensor_pin = pin; }

void FlowMeter::set_calibration_factor(float cal) { cal_factor = cal; }
// void FlowMeter::set_correction_factor(float cor) { cor_factor = cor; }

float FlowMeter::get_flow_rate()
{
    flow_rate = 1000.0 / (millis() - old_time) * pulse_count / cal_factor;

    return flow_rate;
}

void FlowMeter::flowmeter_run()
{
    if ((millis() - old_time) > 1000) // Only process counters once per second
    {
        detachInterrupt(sensor_pin);
        // frequency = 1000.0 / (millis() - old_time) * pulse_count;
        flow_rate = 1000.0 / (millis() - old_time) * pulse_count / (cal_factor * cor_factor);
        // flow_rate = 1000.0 / (millis() - old_time) * pulse_count / cal_factor;
        old_time = millis();
        total_pulse_count += pulse_count;
        total_liters = total_pulse_count / (cal_factor * 60);
        total_mLs = total_liters * 1000;
        pulse_count = 0;
    }
}
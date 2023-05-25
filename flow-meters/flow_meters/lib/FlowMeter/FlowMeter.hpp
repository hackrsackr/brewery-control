#pragma once

#include "flow_config.hpp"

class FlowMeter
{
public:
  uint8_t sensor_pin;
  volatile int pulse_count;
  uint16_t total_pulse_count;

  double total_liters;
  double flow_rate;
  double cal_factor; // listed calibration factor
  double cor_factor; // flow meter output/actual output

  unsigned long total_mLs;
  unsigned long old_time;

  std::string name;

  FlowMeter();
  FlowMeter(int, std::string, double);
  FlowMeter(int, std::string, double, double);

  void set_sensor_pin(uint8_t);
  void set_calibration_factor(double);
  void set_correction_factor(double);
  void reset_total();
  double get_flow_rate();
  void flowmeter_run();
};
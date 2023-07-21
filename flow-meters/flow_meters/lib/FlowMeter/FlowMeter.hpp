#pragma once

typedef struct flowmeter_cfg_t
{
  struct flow_cfg
  {
    std::string id;
    uint8_t sensor_pin;
    double calibration_factor;
    double percent_correction_factor;

  } flow;

} flowmeter_cfg_t;

class FlowMeter
{
public:
  FlowMeter(flowmeter_cfg_t _cfg);
  ~FlowMeter();

  std::string id;
  double flow_rate;
  double total_liters;
  unsigned long total_mLs;

  void run();

private:
  uint8_t sensor_pin;
  double calibration_factor;
  double percent_correction_factor;
  uint16_t total_pulse_count;
  double frequency;
  volatile int pulse_count;
  unsigned long old_time;

  void pulseCounter();
  void attachPinInt();
  double getFrequency();
  double getFlowRate();
};

#pragma once

#include <string>

typedef struct tds_sensor_cfg_t
{
  std::string id;
  uint8_t sensor_pin;
  float tds_factor;

} tds_sensor_cfg_t;

class TDSSensor
{
public:
  TDSSensor(tds_sensor_cfg_t _cfg);
  ~TDSSensor();

  std::string id;
  float tds_factor;
  float TDS;
  float EC;
  float voltage;
  uint16_t adc;

  void readTDSSensor();

private:
  uint8_t sensor_pin;
  float reference_voltage = 5.0;
};
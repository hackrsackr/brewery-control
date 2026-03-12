#include "Arduino.h"
#include "ArduinoJson.h"
// #include "EspMQTTClient.h"

#include <vector>

#include "FlowMeter.hpp"
#include "flow_config.h"

std::vector<FlowMeter *> _FLOWMETERS;

void setup()
{
  Serial.begin(115200);
  for (auto &cfg : FLOW_CFGS)
  {
    FlowMeter *f = new FlowMeter(cfg);
    _FLOWMETERS.push_back(f);
  }
}

void loop()
{
  StaticJsonDocument<400> message;
  message["key"] = _CLIENTID;

  for (auto &flowmeter : _FLOWMETERS)
  {
    flowmeter->run();

    message["data"][flowmeter->id]["Flow_rate[LPM]"] = flowmeter->flow_rate;
    message["data"][flowmeter->id]["Total[mL]"] = flowmeter->total_milliliters;
    message["data"][flowmeter->id]["Total[L]"] = flowmeter->total_liters;
  }
  message["data"]["memory"]["Output_memory_size"] = message.memoryUsage();

  // serializeJsonPretty(message["data"], Serial);
  serializeJson(message["data"], Serial);
  Serial.println("");

  delay(1000);
}

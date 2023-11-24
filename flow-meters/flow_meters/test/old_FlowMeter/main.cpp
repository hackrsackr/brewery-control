#include "Arduino.h"
#include "ArduinoJson.h"
#include "EspMQTTClient.h"

#include <vector>

#include "FlowMeter.hpp"
#include "flow_config.hpp"

EspMQTTClient client(_SSID, _PASS, _MQTTHOST, _CLIENTID, _MQTTPORT);

std::vector<FlowMeter *> _FLOWMETERS;

void setup()
{
  Serial.begin(115200);
  client.setMaxPacketSize(4096);
  client.enableOTA();
  client.enableDebuggingMessages();

  WiFi.begin(_SSID, _PASS);
  uint8_t failed_connections = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("connecting..");
    failed_connections++;
    if (failed_connections > 10)
    {
      Serial.println("restarting..");
      ESP.restart();
    }
  }
  Serial.print("Connected to ");
  Serial.println(WiFi.localIP());

  for (auto &cfg : FLOW_CFGS)
  {
    FlowMeter *f = new FlowMeter(cfg);
    _FLOWMETERS.push_back(f);
  }
}

void onConnectionEstablished()
{
  while (client.isConnected())
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

    if (_PUBLISHMQTT)
    {
      client.publish(_PUBTOPIC, message.as<String>());
      serializeJsonPretty(message, Serial);
    }

    if (!_PUBLISHMQTT)
    {
      serializeJson(message["data"], Serial);
      Serial.println("");
      // serializeJsonPretty(message["data"], Serial);
    }
    delay(5000);
  }
}

void loop()
{
  client.loop();
}

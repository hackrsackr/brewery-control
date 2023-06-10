#include "Arduino.h"
#include "ArduinoJson.h"
#include "EspMQTTClient.h"

#include "FlowMeter.hpp"
#include "flow_config.hpp"

EspMQTTClient client(_SSID, _PASS, _MQTTHOST, _CLIENTID, _MQTTPORT);

FlowMeter f1(FLOW_CFGS[0]);
FlowMeter f2(FLOW_CFGS[1]);
FlowMeter f3(FLOW_CFGS[2]);

void pulseCounter1() { f1.pulse_count++; }
void pulseCounter2() { f2.pulse_count++; }
void pulseCounter3() { f3.pulse_count++; }

void onConnectionEstablished(void);
void publishData(void);

void setup()
{
  Serial.begin(115200);
  client.setMaxPacketSize(4096);
  client.enableOTA();

  WiFi.disconnect(true);
  delay(1000);
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

  pinMode(f1.sensor_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(f1.sensor_pin), pulseCounter1, FALLING);

  pinMode(f2.sensor_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(f2.sensor_pin), pulseCounter2, FALLING);

  pinMode(f3.sensor_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(f3.sensor_pin), pulseCounter3, FALLING);
}

void loop()
{
  client.loop();
  publishData();
}

void onConnectionEstablished()
{
  publishData();
}

void publishData()
{
  StaticJsonDocument<400> message;
  message["key"] = _CLIENTID;

  f1.run();
  message["data"][f1.flowmeter_id]["Flow_rate[LPM]"] = f1.flow_rate;
  message["data"][f1.flowmeter_id]["Total[mL]"] = f1.total_mLs;
  message["data"][f1.flowmeter_id]["Total[L]"] = f1.total_liters;
  attachInterrupt(f1.sensor_pin, pulseCounter1, FALLING);

  f2.run();
  message["data"][f2.flowmeter_id]["Flow_rate[LPM]"] = f2.flow_rate;
  message["data"][f2.flowmeter_id]["Total[mL]"] = f2.total_mLs;
  message["data"][f2.flowmeter_id]["Total[L]"] = f2.total_liters;
  attachInterrupt(f2.sensor_pin, pulseCounter2, FALLING);

  f3.run();
  message["data"][f3.flowmeter_id]["Flow_rate[LPM]"] = f3.flow_rate;
  message["data"][f3.flowmeter_id]["Total[mL]"] = f3.total_mLs;
  message["data"][f3.flowmeter_id]["Total[L]"] = f3.total_liters;
  attachInterrupt(f3.sensor_pin, pulseCounter3, FALLING);

  client.publish(_PUBTOPIC, message.as<String>());
  serializeJsonPretty(message, Serial);
  delay(5000);
}

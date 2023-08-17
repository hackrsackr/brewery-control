#include "ArduinoJson.h"
#include "EspMQTTClient.h"

#include "Flowmeter.hpp"
#include "flow_config.hpp"

EspMQTTClient client(_SSID, _PASS, _MQTTHOST, _CLIENTID, _MQTTPORT);

FlowMeter f1(_SPIN1, _FLOW1, _YF_S302);
FlowMeter f2(_SPIN2, _FLOW2, _YF_S302);

void pulseCounter1() { f1.pulse_count++; }
void pulseCounter2() { f2.pulse_count++; }

void initWifi(void);
void onConnectionEstablished(void);
void publishData(void);

void setup()
{
  // Initialize a serial connection for reporting values to the host
  Serial.begin(115200);

  client.setMaxPacketSize(4096);
  client.enableOTA();

  // WiFi
  initWifi();

  pinMode(f1.sensor_pin, INPUT_PULLUP);
  attachInterrupt(f1.sensor_pin, pulseCounter1, FALLING);

  pinMode(f2.sensor_pin, INPUT_PULLUP);
  attachInterrupt(f2.sensor_pin, pulseCounter2, FALLING);
}

void loop()
{
  client.loop();
  publishData();
}

void initWifi()
{
  WiFi.disconnect(true);
  delay(1000);
  WiFi.begin(_SSID, _PASS);
  uint8_t failed_connections = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("connecting..");
    failed_connections++;
    if (failed_connections > 20)
    {
      Serial.println("restarting..");
      ESP.restart();
    }
  }
  Serial.print("Connected to ");
  Serial.println(WiFi.localIP());
}

void onConnectionEstablished()
{
  publishData();
}

void publishData()
{
  if (client.isConnected())
  {
    StaticJsonDocument<400> message;
    message["key"] = _CLIENTID;

    f1.flowmeter_run();
    // message["data"][f1.name]["Frequency"] = f1.frequency;
    // message["data"][f1.name]["Total_pulse_count"] = f1.total_pulse_count;
    message["data"][f1.name]["Flow_rate[LPM]"] = f1.flow_rate;
    message["data"][f1.name]["Total[mL]"] = f1.total_mLs;
    message["data"][f1.name]["Total[L]"] = f1.total_liters;
    attachInterrupt(f1.sensor_pin, pulseCounter1, FALLING);

    f2.flowmeter_run();
    // message["data"][f2.name]["Frequency"] = f2.frequency;
    // message["data"][f2.name]["Total_pulse_count"] = f2.total_pulse_count;
    message["data"][f2.name]["Flow_rate[LPM]"] = f2.flow_rate;
    message["data"][f2.name]["Total[mL]"] = f2.total_mLs;
    message["data"][f2.name]["Total[L]"] = f2.total_liters;
    attachInterrupt(f2.sensor_pin, pulseCounter2, FALLING);

    client.publish(_PUBTOPIC, message.as<String>());
    serializeJsonPretty(message, Serial);
    delay(5000);
  }
}
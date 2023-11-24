#include "Arduino.h"
#include "ArduinoJson.h"
// #include "EspMQTTClient.h"

// #include <vector>

// #include "FlowMeter.hpp"
// #include "flow_config.hpp"

// std::vector<FlowMeter *> _FLOWMETERS;

// void setup()
// {
//   Serial.begin(115200);
//   for (auto &cfg : FLOW_CFGS)
//   {
//     FlowMeter *f = new FlowMeter(cfg);
//     _FLOWMETERS.push_back(f);
//   }
// }

// void loop()
// {
//   StaticJsonDocument<400> message;
//   message["key"] = _CLIENTID;

//   for (auto &flowmeter : _FLOWMETERS)
//   {
//     flowmeter->run();

//     message["data"][flowmeter->id]["Flow_rate[LPM]"] = flowmeter->flow_rate;
//     message["data"][flowmeter->id]["Total[mL]"] = flowmeter->total_milliliters;
//     message["data"][flowmeter->id]["Total[L]"] = flowmeter->total_liters;
//   }
//   message["data"]["memory"]["Output_memory_size"] = message.memoryUsage();

//   serializeJsonPretty(message["data"], Serial);
//   serializeJson(message["data"], Serial);
//   Serial.println("");

//   delay(5000);
//   // client.loop();
// }

#define sensor_pin 4

long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
float calibrationFactor = 32.0;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}

void setup()
{
  Serial.begin(115200);

  pinMode(sensor_pin, INPUT_PULLUP);

  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;

  attachInterrupt(digitalPinToInterrupt(sensor_pin), pulseCounter, FALLING);
}

void loop()
{
  StaticJsonDocument<400> message;
  message["key"] = "test";

  currentMillis = millis();
  if (currentMillis - previousMillis > interval)
  {

    pulse1Sec = pulseCount;
    pulseCount = 0;

    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();

    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;

    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;

    message["data"]["PulseCount"] = pulse1Sec;
    message["data"]["Flow_rate[LPM]"] = flowRate;
    message["data"]["Total[mL]"] = totalMilliLitres;
    message["data"]["Total[L]"] = totalMilliLitres / 1000;

    serializeJsonPretty(message["data"], Serial);
    // serializeJson(message["data"], Serial);
    Serial.println("");

    delay(2500);
  }
}
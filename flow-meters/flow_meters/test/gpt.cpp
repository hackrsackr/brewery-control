#include "HallEffectFlowMeter.h"
#include <ArduinoJson.h>
#include <Arduino.h>
#include <EspMQTTClient.h>

// std::vector<HallEffectFlowMeter> flowMeters;

// const unsigned long MIN_INTERVAL = 1000000; // Minimum interval between flow events (in microseconds)

// HallEffectFlowMeter::HallEffectFlowMeter(int pin, const std::string &name, float calibrationFactor)
//     : flowPin(pin), meterName(name), pulseCount(0), lastFlowTime(0), calibrationFactor(calibrationFactor)
// {
//   // Initialize the flow meter
// }

// HallEffectFlowMeter::~HallEffectFlowMeter()
// {
//   // Cleanup resources
// }

// void HallEffectFlowMeter::startMeasurement()
// {
//   attachInterrupt(digitalPinToInterrupt(flowPin), flowEventCallback, RISING);
// }

// void HallEffectFlowMeter::stopMeasurement()
// {
//   detachInterrupt(digitalPinToInterrupt(flowPin));
// }

// void IRAM_ATTR HallEffectFlowMeter::flowEventCallback()
// {
//   // Increment pulse count
//   flowMeters.back().pulseCount++;

//   // Compute frequency
//   unsigned long currentTime = micros();
//   unsigned long elapsedTime = currentTime - flowMeters.back().lastFlowTime;
//   if (elapsedTime >= MIN_INTERVAL)
//   {
//     float frequency = 1000000.0 / elapsedTime;
//     flowMeters.back().lastFlowTime = currentTime;

//     // Compute flow rate
//     float flowRate = (flowMeters.back().pulseCount * flowMeters.back().calibrationFactor) / (elapsedTime / 60000000.0);

//     // Publish flow meter data to MQTT broker
//     publishFlowData(frequency, flowRate);
//   }
// }

// void HallEffectFlowMeter::publishFlowData(float frequency, float flowRate)
// {
//   StaticJsonDocument<256> jsonDoc;
//   jsonDoc["meter_name"] = flowMeters.back().meterName.c_str();
//   jsonDoc["pulse_count"] = flowMeters.back().pulseCount;
//   jsonDoc["frequency"] = frequency;
//   jsonDoc["flow_rate"] = flowRate;

//   char jsonBuffer[256];
//   serializeJson(jsonDoc, jsonBuffer);

//   EspMQTTClient.publish("flow_meter_data", jsonBuffer);
// }

// #include <Arduino.h>
// #include <vector>
// #include <WiFi.h>
// #include <EspMQTTClient.h>
// #include "HallEffectFlowMeter.h"

// std::vector<HallEffectFlowMeter> flowMeters;

// WiFiClient wifiClient;
// EspMQTTClient EspMQTTClient(
//     wifiClient,
//     "mqtt_broker_ip",
//     "mqtt_broker_port",
//     "mqtt_broker_username",
//     "mqtt_broker_password");

// void setup()
// {
//   Serial.begin(115200);

//   WiFi.begin("Your_SSID", "Your_Password");
//   while (WiFi.status() != WL_CONNECTED)
//   {
//     delay(1000);
//     Serial.println("Connecting to WiFi...");
//   }
//   Serial.println("Connected to WiFi");

//   EspMQTTClient.enableDebuggingMessages();
//   EspMQTTClient.enableLastWillMessage("lastwill/topic", "Last Will Message", false, 0);

//   // Connect to MQTT broker
//   EspMQTTClient.connect();

//   // Create and initialize flow meters with calibration factors
//   float calibrationFactor1 = 0.5; // Example calibration factor for Flow Meter 1
//   float calibrationFactor2 = 0.3; // Example calibration factor for Flow Meter 2
//   float calibrationFactor3 = 0.2; // Example calibration factor for Flow Meter 3

//   flowMeters.emplace_back(HallEffectFlowMeter(2, "Flow Meter 1", calibrationFactor1));
//   flowMeters.emplace_back(HallEffectFlowMeter(3, "Flow Meter 2", calibrationFactor2));
//   flowMeters.emplace_back(HallEffectFlowMeter(4, "Flow Meter 3", calibrationFactor3));

//   // Start flow measurement for each flow meter
//   for (auto &meter : flowMeters)
//   {
//     meter.startMeasurement();
//   }
// }

// void loop()
// {
//   EspMQTTClient.loop();

//   // Maintain MQTT connection
//   if (!EspMQTTClient.connected())
//   {
//     EspMQTTClient.connect();
//   }

//   delay(1000);
// }
class FanSpeed
{

public:
  void
  setupInterruptHandler(uint8_t irq_pin, void (*ISR)(void), int value),
      handleInterrupt(void);

  getSpeed();

private:
  uint32_t
      _lastMicros = 0UL,
      _interval = 60000000UL;
};

void FanSpeed::setupInterruptHandler(uint8_t irq_pin, void (*ISR)(void), int value)
{
  attachInterrupt(digitalPinToInterrupt(irq_pin), ISR, value);
}

inline void FanSpeed::handleInterrupt(void)
{
  uint32_t nowMicros = micros();
  _interval = nowMicros - _lastMicros;
  _lastMicros = nowMicros;
}

double FanSpeed::getSpeed()
{
  if (micros() - _lastMicros < 4294967296UL) // has rotated in the last ~70 seconds
  {
    return 60000000.0 / _interval;
  }
  else
  {
    return 0;
  }
}

//
// PROGRAM START
//

FanSpeed *fan1;
uint8_t fan1pin = 2;
FanSpeed *fan2;
uint8_t fan2pin = 3;

void fan1InterruptHandler(void)
{
  fan1->handleInterrupt();
}

void fan2InterruptHandler(void)
{
  fan2->handleInterrupt();
}

void setup()
{
  Serial.begin(115200);
  pinMode(fan1pin, INPUT_PULLUP);
  pinMode(fan2pin, INPUT_PULLUP);
  fan1 = new FanSpeed();
  fan1->setupInterruptHandler(fan1pin, fan1InterruptHandler, FALLING);
  fan2 = new FanSpeed();
  fan2->setupInterruptHandler(fan2pin, fan2InterruptHandler, FALLING);
}

void loop()
{
  static uint32_t lastMillis = 0;
  if (millis() - lastMillis > 1000UL)
  {
    Serial.print(F("Fan one speed = "));
    Serial.print(floor(fan1->getSpeed() + 0.5), 0);
    Serial.println(F(" RPM"));
    Serial.print(F("Fan two speed = "));
    Serial.print(floor(fan2->getSpeed() + 0.5), 0);
    Serial.println(F(" RPM"));
    Serial.println('\r');
    lastMillis = millis();
  }
}
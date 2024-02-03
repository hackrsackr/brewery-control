#include "Arduino.h"
#include "ArduinoJson.h"
#include "EspMQTTClient.h"
#include <array>

#include "Spund_System.h"
#include "ADS_Sensor.h"
#include "Relay.h"
#include "config.h"

EspMQTTClient client(_SSID, _PASS, _MQTTHOST, _CLIENTID, _MQTTPORT);
StaticJsonDocument<4096> input;

void onConnectionEstablished();

void setup(void)
{
    Serial.begin(115200);
    client.setMaxPacketSize(4096);
}

void loop(void)
{
    client.loop();
}

void onConnectionEstablished()
{
    client.subscribe(_SUBTOPIC, [](const String &payload)
                     { Serial.println(payload); });
}
#include <ArduinoJson.h>
#include <EspMQTTClient.h>

#include "config.h"

EspMQTTClient client(_SSID, _PASS, _MQTTHOST, _CLIENTID, _MQTTPORT);

void onConnectionEstablished()
{
    uint8_t temp = 25;

    StaticJsonDocument<1000> message;
    message["key"] = _CLIENTID;

    while (client.isConnected())
    {
        message["data"]["temp-1"] = temp;
        ++temp;
        message["data"]["temp-2"] = temp;
        temp++;
        message["data"]["temp-3"] = temp;

        message["data"]["memory"]["Output_memory_size"] = message.memoryUsage();

        if (!client.publish(_PUBTOPIC, message.as<String>()))
        {
            Serial.println("restarting due to failed MQTT publish");
            ESP.restart();
        }

        serializeJsonPretty(message, Serial);
        delay(5000);
    }
}

void setup(void)
{
    Serial.begin(115200);
    client.setMaxPacketSize(4096);
    // client.enableDebuggingMessages();
}

void loop(void)
{
    client.loop();
}

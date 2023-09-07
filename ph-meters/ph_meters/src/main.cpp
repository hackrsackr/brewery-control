#include "Arduino.h"
#include "ArduinoJson.h"
#include "Adafruit_ADS1X15.h"
#include "EspMQTTClient.h"

#include "MA_Meter.h"
#include "config.h"

EspMQTTClient client(_SSID, _PASS, _MQTTHOST, _CLIENTID, _MQTTPORT);
std::vector<MA_Meter *> _METERS;

void setup()
{
    Serial.begin(115200);

    Wire.end();
    Wire.setPins(_I2C_SDA, _I2C_SCL);
    Wire.begin();

    // client.enableDebuggingMessages();
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

    for (auto &meter_cfg : meter_cfgs)
    {
        MA_Meter *m = new MA_Meter(meter_cfg);
        _METERS.push_back(m);
    }
}

void onConnectionEstablished()
{
    while (client.isConnected())
    {
        StaticJsonDocument<1000> message;
        message["key"] = _CLIENTID;

        for (auto &meter : _METERS)
        {
            meter->read();
            
            message["data"][meter->description]["volts"] = meter->volts;
            message["data"][meter->description]["ma"] = meter->ma;
            
            if (meter->meter_type == 0)
            {
                message["data"][meter->description]["ph"] = meter->ph;
            }

            if (meter->meter_type == 1)
            {
                message["data"][meter->description]["ppm"] = meter->ppm;
            }
        }

        message["data"]["memory"]["Output memory size"] = message.memoryUsage();

        if (_PUBLISHMQTT)
        {
            client.publish(_PUBTOPIC, message.as<String>());
            serializeJsonPretty(message, Serial);
        }

        if (!_PUBLISHMQTT)
        {
            serializeJson(message["data"], Serial);
        }

        delay(5000);
    }
}

void loop()
{
    client.loop();
}
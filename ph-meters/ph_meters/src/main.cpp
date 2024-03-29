#include "Arduino.h"
#include "ArduinoJson.h"
#include "Adafruit_ADS1X15.h"
#include "EspMQTTClient.h"

#include "MA_Meter.h"
#include "config.h"

EspMQTTClient client(_SSID, _PASS, _MQTTHOST, _CLIENTID, _MQTTPORT);
std::vector<MA_Meter *> _METERS;
void onConnectionEstablished();

void setup()
{
    // client.enableDebuggingMessages();
    client.setMaxPacketSize(4096);
    // client.enableOTA();
    // client.enableDrasticResetOnConnectionFailures();
    // client.enableMQTTPersistence();

    Serial.begin(115200);

    Wire.begin(_I2C_SDA, _I2C_SCL);

    for (auto &meter_cfg : meter_cfgs)
    {
        MA_Meter *m = new MA_Meter(meter_cfg);
        _METERS.push_back(m);
    }
}

void loop()
{
    client.loop();
}

void onConnectionEstablished()
{
    while (client.isConnected())
    {
        StaticJsonDocument<1000> message;
        message["key"] = _CLIENTID;

        for (auto &meter : _METERS)
        {
            if (!meter->adsReady())
            {
                break;
            }

            meter->read();

            // message["data"][meter->id]["volts"] = meter->volts;                 // DEBUG
            message["data"][meter->id]["ma"] = meter->ma; // Calibration
            message["data"][meter->id][meter->measurement] = meter->output;
        }

        message["data"]["memory"]["Output memory size"] = message.memoryUsage();

        if (_PUBLISHMQTT)
        {
            client.publish(_PUBTOPIC, message.as<String>());

            if (!client.publish(_PUBTOPIC, message.as<String>()))
            {
                ESP.restart();
            }

            serializeJsonPretty(message, Serial);

            // Execute delayed instructions
            // client.executeDelayed(5000, [&message]()
            //                       {
            //     client.publish(_PUBTOPIC, message.as<String>());
            //     serializeJsonPretty(message, Serial); });
        }

        if (!_PUBLISHMQTT)
        {
            // serializeJson(message["data"], Serial);
            serializeJsonPretty(message, Serial);
        }

        delay(5000);
    }
}

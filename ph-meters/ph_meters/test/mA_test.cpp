#include "Arduino.h"
#include "ArduinoJson.h"
#include "Adafruit_ADS1X15.h"

#include "MA_Meter.h"
#include "config.h"

std::vector<MA_Meter *> _METERS;

void setup()
{
    Serial.begin(115200);

    Wire.end();
    Wire.setPins(_I2C_SDA, _I2C_SCL);
    Wire.begin();

    for (auto &meter_cfg : meter_cfgs)
    {
        MA_Meter *m = new MA_Meter(meter_cfg);
        _METERS.push_back(m);
    }
}

void loop()
{
    StaticJsonDocument<1000> message;
    message["key"] = _CLIENTID;

    for (auto &meter : _METERS)
    {
        meter->read();
        // message["data"][meter->id]["volts"] = meter->volts;
        // message["data"][meter->id]["ma"] = meter->ma;
        message["data"][meter->id][meter->measurement] = meter->output;
    }

    message["data"]["memory"]["Output memory size"] = message.memoryUsage();

    // serializeJsonPretty(message["data"], Serial);
    serializeJson(message["data"], Serial);
    Serial.println("");

    delay(5000);
}

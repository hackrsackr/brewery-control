#include "Arduino.h"
#include "config.h"
#include <Adafruit_ADS1X15.h>

#include "Spund_System.h"
#include <string>

std::vector<Spund_System *> _SPUNDERS;

void setup(void)
{
    Serial.begin(115200);
    Wire.begin(_I2C_SDA, _I2C_SCL);

    for (auto &spund_cfg : spund_cfgs)
    {
        Spund_System *s = new Spund_System(spund_cfg);
        _SPUNDERS.push_back(s);
    }
}

void loop(void)
{
    for (auto &spunder : _SPUNDERS)
    {
        Serial.printf("%s \t", spunder->spunder_id);
        Serial.printf("VOLTS: %1.2f \t", spunder->readVolts());
        Serial.printf("%s: %2.2f \n", spunder->getSensorUnit().c_str(), spunder->readSensorUnits());
    }
    Serial.println("------------------------------------------");
    delay(1000);
}
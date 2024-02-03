#include "ADS_Sensor.h"
#include "config.h"

std::vector<Spund_System *> _SPUNDERS;

void setup(void)
{
    Serial.begin(115200);

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
        Serial.printf("VOLTS%d: %1.2f \t", spunder->ads_channel, spunder->getVolts());
        Serial.printf("PSI%d: %2.2f \n", spunder->ads_channel, spunder->getPSI());
    }
    Serial.println("------------------------------------------");
    delay(1000);
}
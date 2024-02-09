#include "config.h"
#include "Relay.h"

std::vector<Relay *> _RELAYS;

void setup(void)
{
    Serial.begin(115200);

    for (auto &spund_cfg : spund_cfgs)
    {
        Relay *r = new Relay();
        r->begin(spund_cfg.spunder.relay_pin);
        _RELAYS.push_back(r);
    }
}

void loop(void)
{
    for (auto &relay : _RELAYS)
    {
        relay->openRelay();
        Serial.printf("relay pin %i opened \n", relay->getRelayPin());
        delay(1000);
        relay->closeRelay();
        Serial.printf("relay pin %i closed \n", relay->getRelayPin());
        delay(1000);
    }
}
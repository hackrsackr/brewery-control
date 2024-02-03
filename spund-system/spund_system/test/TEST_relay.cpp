#include "config.h"
#include "Relay.h"

std::vector<Relay *> _RELAYS;

void setup(void)
{
    Serial.begin(115200);

    for (auto &spund_cfg : spund_cfgs)
    {
        Relay *r = new Relay(spund_cfg.spunder.relay_pin);
        _RELAYS.push_back(r);
    }
}

void loop(void)
{
    for (auto &relay : _RELAYS)
    {
        Serial.printf("PIN%d: %6d \t", relay->relay_pin);
        Serial.printf("STATE%d: %6d \t", relay->relay_state);
        Serial.printf("Toggled%d: %6d \t", relay->relay_toggled);
    }
    Serial.println("------------------------------------------");
    delay(1000);
}
#include "Arduino.h"
#include "config.h"

#include "ADS_Sensor.h"

uint8_t ads_chan = 0;

// ADS_Sensor ads(spund_cfgs[0].ads1115.ads_cfg);
ADS_Sensor ads;

void setup(void)
{
    Serial.begin(115200);
    ads.begin(
        ADS1115_ADDRESS1,
        GAIN_TWOTHIRDS,
        _I2C_SDA,
        _I2C_SCL,
        ads_chan);
}

void loop(void)
{
    for (auto &spund_cfg : spund_cfgs)
    {
        ads.ads_channel = spund_cfg.ads1115.ads_cfg.ads_channel;
        Serial.printf("ADC%d: %6d \t", ads.ads_channel, ads.readADC());
        Serial.printf("VOLTS%d: %1.2f \n", ads.ads_channel, ads.readVolts());
    }

    Serial.println("------------------------------------------");
    delay(1000);
}

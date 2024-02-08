#include "ADS_Sensor.h"
#include "config.h"

constexpr auto ads_channel = 0;
constexpr auto input_low_range = 0.5;
constexpr auto input_high_range = 4.5;
constexpr auto output_low_range = 0.0;
constexpr auto output_high_range = 30.0;
constexpr auto offset = 0.0;

ADS_Pressure_Sensor ads_ps;

void setup(void)
{
    Serial.begin(115200);

    ads_ps.begin(
        ADS1115_ADDRESS1,
        GAIN_TWOTHIRDS,
        _I2C_SDA,
        _I2C_SCL,
        ads_channel,
        input_low_range,
        input_high_range,
        output_high_range,
        offset);
}

void loop(void)
{
    for (auto &spund_cfg : spund_cfgs)
    {
        ads_ps.ads_channel = spund_cfg.ads1115.ads_cfg.ads_channel;
        Serial.printf("ADC%d: %6d \t", ads_ps.ads_channel, ads_ps.readADC());
        Serial.printf("VOLTS%d: %1.2f \t", ads_ps.ads_channel, ads_ps.readVolts());
        Serial.printf("PSI%d: %2.2f \n", ads_ps.ads_channel, ads_ps.computePSI());
    }
    Serial.println("------------------------------------------");
    delay(1000);
}

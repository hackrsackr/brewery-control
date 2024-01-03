#include "Arduino.h"
#include <Adafruit_ADS1X15.h>
#include "config.h"

Adafruit_ADS1115 ads;

void setup(void)
{
    Serial.begin(115200);

    Wire.end();
    Wire.setPins(_I2C_SDA, _I2C_SCL);
    Wire.begin();

    ads.begin();

    if (!ads.begin())
    {
        Serial.println("Failed to initialize ADS.");
        while (1)
            ;
    }

    // Start the first conversion.
    // ads.startADCReading(ADS1X15_REG_CONFIG_MUX_DIFF_0_1, /*continuous=*/false);
}

// void loop(void)
// {
//     // If we don't have new data, skip this iteration.
//     if (!ads.conversionComplete())
//     {
//         return;
//     }

//     int16_t results = ads.getLastConversionResults();

//     Serial.print("Differential: ");
//     Serial.print(results);
//     Serial.print("(");
//     Serial.print(ads.computeVolts(results));
//     Serial.println("mV)");

//     // Start another conversion.
//     ads.startADCReading(ADS1X15_REG_CONFIG_MUX_DIFF_0_1, /*continuous=*/false);

//     delay(1000);
// }

// void loop(void)
// {
//     int16_t adc0, adc1, adc2, adc3;
//     float volts0, volts1, volts2, volts3;

//     adc0 = ads.readADC_SingleEnded(0);
//     adc1 = ads.readADC_SingleEnded(1);
//     adc2 = ads.readADC_SingleEnded(2);
//     adc3 = ads.readADC_SingleEnded(3);

//     volts0 = ads.computeVolts(adc0);
//     volts1 = ads.computeVolts(adc1);
//     volts2 = ads.computeVolts(adc2);
//     volts3 = ads.computeVolts(adc3);

//     Serial.println("-----------------------------------------------------------");
//     Serial.print("AIN0: ");
//     Serial.print(adc0);
//     Serial.print("  ");
//     Serial.print(volts0);
//     Serial.println("V");
//     Serial.print("AIN1: ");
//     Serial.print(adc1);
//     Serial.print("  ");
//     Serial.print(volts1);
//     Serial.println("V");
//     Serial.print("AIN2: ");
//     Serial.print(adc2);
//     Serial.print("  ");
//     Serial.print(volts2);
//     Serial.println("V");
//     Serial.print("AIN3: ");
//     Serial.print(adc3);
//     Serial.print("  ");
//     Serial.print(volts3);
//     Serial.println("V");

//     delay(1000);
// }

void loop(void)
{
    int16_t results;

    /* Be sure to update this value based on the IC and the gain settings! */
    // float   multiplier = 3.0F;    /* ADS1015 @ +/- 6.144V gain (12-bit results) */
    float multiplier = 0.0001875F; /* ADS1115  @ +/- 6.144V gain (16-bit results) */

    results = ads.readADC_Differential_0_1();

    // Serial.print("Differential: ");
    // Serial.print(results);
    // Serial.print("(");
    Serial.print(results * multiplier);
    Serial.println("V");

    delay(1000);
}
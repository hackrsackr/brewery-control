#include "Arduino.h"
#include <Adafruit_ADS1X15.h>
#include "config.h"

#define I2C_SDA 18
#define I2C_SCL 17

Adafruit_ADS1115 ads1; /* Use this for the 16-bit version */
Adafruit_ADS1115 ads2; /* Use this for the 16-bit version */
Adafruit_ADS1115 ads3; /* Use this for the 16-bit version */
Adafruit_ADS1115 ads4; /* Use this for the 16-bit version */

std::vector<Adafruit_ADS1115> _ADS{ads1, ads2, ads3, ads4};

void setup(void)
{
    Wire.begin(I2C_SDA, I2C_SCL);
    Serial.begin(115200);

    ads1.begin(_I2C_ADDRESS1);
    ads2.begin(_I2C_ADDRESS2);
    ads3.begin(_I2C_ADDRESS3);
    ads4.begin(_I2C_ADDRESS4);

    if (!ads1.begin(0x48))
    {
        Serial.println("Failed to initialize ADS1.");
    }

    if (!ads2.begin(0x49))
    {
        Serial.println("Failed to initialize ADS2.");
    }
    if (!ads3.begin(0x4a))
    {
        Serial.println("Failed to initialize ADS3.");
    }

    if (!ads4.begin(0x4b))
    {
        Serial.println("Failed to initialize ADS4.");
    }
}

void loop(void)
{
    int16_t adc1_0, adc1_1, adc1_2, adc1_3;
    int16_t adc2_0, adc2_1, adc2_2, adc2_3;
    int16_t adc3_0, adc3_1, adc3_2, adc3_3;
    int16_t adc4_0, adc4_1, adc4_2, adc4_3;

    float volts1_0, volts1_1, volts1_2, volts1_3;
    float volts2_0, volts2_1, volts2_2, volts2_3;
    float volts3_0, volts3_1, volts3_2, volts3_3;
    float volts4_0, volts4_1, volts4_2, volts4_3;

    adc1_0 = ads1.readADC_SingleEnded(0);
    adc1_1 = ads1.readADC_SingleEnded(1);
    adc1_2 = ads1.readADC_SingleEnded(2);
    adc1_3 = ads1.readADC_SingleEnded(3);

    adc2_0 = ads2.readADC_SingleEnded(0);
    adc2_1 = ads2.readADC_SingleEnded(1);
    adc2_2 = ads2.readADC_SingleEnded(2);
    adc2_3 = ads2.readADC_SingleEnded(3);

    adc3_0 = ads3.readADC_SingleEnded(0);
    adc3_1 = ads3.readADC_SingleEnded(1);
    adc3_2 = ads3.readADC_SingleEnded(2);
    adc3_3 = ads3.readADC_SingleEnded(3);

    adc4_0 = ads4.readADC_SingleEnded(0);
    adc4_1 = ads4.readADC_SingleEnded(1);
    adc4_2 = ads4.readADC_SingleEnded(2);
    adc4_3 = ads4.readADC_SingleEnded(3);

    volts1_0 = ads1.computeVolts(adc1_0);
    volts1_1 = ads1.computeVolts(adc1_1);
    volts1_2 = ads1.computeVolts(adc1_2);
    volts1_3 = ads1.computeVolts(adc1_3);

    volts2_0 = ads2.computeVolts(adc2_0);
    volts2_1 = ads2.computeVolts(adc2_1);
    volts2_2 = ads2.computeVolts(adc2_2);
    volts2_3 = ads2.computeVolts(adc2_3);

    volts3_0 = ads3.computeVolts(adc3_0);
    volts3_1 = ads3.computeVolts(adc3_1);
    volts3_2 = ads3.computeVolts(adc3_2);
    volts3_3 = ads3.computeVolts(adc3_3);

    volts4_0 = ads4.computeVolts(adc4_0);
    volts4_1 = ads4.computeVolts(adc4_1);
    volts4_2 = ads4.computeVolts(adc4_2);
    volts4_3 = ads4.computeVolts(adc4_3);

    Serial.println("-----------------------------------------------------------");
    Serial.print("AIN1-0: ");
    Serial.print(adc1_0);
    Serial.print("  ");
    Serial.print(volts1_0);
    Serial.println("V");
    Serial.print("AIN1-1: ");
    Serial.print(adc1_1);
    Serial.print("  ");
    Serial.print(volts1_1);
    Serial.println("V");
    Serial.print("AIN1-2: ");
    Serial.print(adc1_2);
    Serial.print("  ");
    Serial.print(volts1_2);
    Serial.println("V");
    Serial.print("AIN1-3: ");
    Serial.print(adc1_3);
    Serial.print("  ");
    Serial.print(volts1_3);
    Serial.println("V");

    Serial.println("-----------------------------------------------------------");
    Serial.print("AIN2-0: ");
    Serial.print(adc2_0);
    Serial.print("  ");
    Serial.print(volts2_0);
    Serial.println("V");
    Serial.print("AIN2-1: ");
    Serial.print(adc2_1);
    Serial.print("  ");
    Serial.print(volts2_1);
    Serial.println("V");
    Serial.print("AIN2-2: ");
    Serial.print(adc2_2);
    Serial.print("  ");
    Serial.print(volts2_2);
    Serial.println("V");
    Serial.print("AIN2-3: ");
    Serial.print(adc2_3);
    Serial.print("  ");
    Serial.print(volts2_3);
    Serial.println("V");

    Serial.println("-----------------------------------------------------------");
    Serial.print("AIN3-0: ");
    Serial.print(adc3_0);
    Serial.print("  ");
    Serial.print(volts3_0);
    Serial.println("V");
    Serial.print("AIN3-1: ");
    Serial.print(adc3_1);
    Serial.print("  ");
    Serial.print(volts3_1);
    Serial.println("V");
    Serial.print("AIN3-2: ");
    Serial.print(adc3_2);
    Serial.print("  ");
    Serial.print(volts3_2);
    Serial.println("V");
    Serial.print("AIN3-3: ");
    Serial.print(adc3_3);
    Serial.print("  ");
    Serial.print(volts3_3);
    Serial.println("V");

    Serial.println("-----------------------------------------------------------");
    Serial.print("AIN4-0: ");
    Serial.print(adc4_0);
    Serial.print("  ");
    Serial.print(volts4_0);
    Serial.println("V");
    Serial.print("AIN4-1: ");
    Serial.print(adc4_1);
    Serial.print("  ");
    Serial.print(volts4_1);
    Serial.println("V");
    Serial.print("AIN4-2: ");
    Serial.print(adc4_2);
    Serial.print("  ");
    Serial.print(volts4_2);
    Serial.println("V");
    Serial.print("AIN4-3: ");
    Serial.print(adc4_3);
    Serial.print("  ");
    Serial.print(volts4_3);
    Serial.println("V");

    delay(1000);
}

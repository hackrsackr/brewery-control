from time import sleep

from ads1115 import ADS1115

ADS_FULLSCALE = 32767
GAIN = 2/3
ADS_MAX_V = 4.096 / GAIN

ads1 = ADS1115(address=0x48)
ads2 = ADS1115(address=0x49)
ads3 = ADS1115(address=0x4a)
ads4 = ADS1115(address=0x4b)

while True:
    adc1_1 = ads3.readADC(0, gain=GAIN)
    adc1_2 = ads3.readADC(1, gain=GAIN)
    adc1_3 = ads3.readADC(2, gain=GAIN)
    adc1_4 = ads3.readADC(3, gain=GAIN)

    v1 = ads3.readVolts(0, ADS_MAX_V, ADS_FULLSCALE)
    v2 = ads3.readVolts(1, ADS_MAX_V, ADS_FULLSCALE)
    v3 = ads3.readVolts(2, ADS_MAX_V, ADS_FULLSCALE)
    v4 = ads3.readVolts(3, ADS_MAX_V, ADS_FULLSCALE)

    print(f'adc1: {adc1_1} volts1: {v1}')
    print(f'adc2: {adc1_2} volts1: {v2}')
    print(f'adc3: {adc1_3} volts1: {v3}')
    print(f'adc1: {adc1_4} volts1: {v4}')

    sleep(3)
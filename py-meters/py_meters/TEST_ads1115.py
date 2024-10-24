from time import sleep

from ads1115 import ADS1115

ADS_FULLSCALE = 32767
GAIN = 2/3
ADS_MAX_V = 4.096 / GAIN

ads1 = ADS1115(address=0x48)
ads2 = ADS1115(address=0x49)
ads3 = ADS1115(address=0x4a)
ads4 = ADS1115(address=0x4b)
    
p = 0

while True:
    adc1_1 = ads3.readADC(0, gain=GAIN)
    adc1_2 = ads3.readADC(1, gain=GAIN)
    adc1_3 = ads3.readADC(2, gain=GAIN)
    # adc1_4 = ads3.readADC(3, gain=GAIN)

    # v1 = ads3.readVolts(0, ADS_MAX_V, ADS_FULLSCALE)
    # v2 = ads3.readVolts(1, ADS_MAX_V, ADS_FULLSCALE)
    # v3 = ads3.readVolts(2, ADS_MAX_V, ADS_FULLSCALE)
    # v4 = ads3.readVolts(3, ADS_MAX_V, ADS_FULLSCALE)
    
    # LITERS TEST 
    offset = 6000
    Offsets = [7500, 5824, 6960]
    bitsPerLiter = 442

    l1 = ( adc1_1 - Offsets[0] ) / bitsPerLiter 
    l2 = ( adc1_2 - Offsets[1] ) / bitsPerLiter 
    l3 = ( adc1_3 - Offsets[2] ) / bitsPerLiter 
    # l4 = ( adc1_4 - offset ) / bitsPerLiter 

    print(f'adc1: {adc1_1} liters1: {l1:.2f}')
    print(f'adc2: {adc1_2} liters2: {l2:.2f}')
    print(f'adc3: {adc1_3} liters3: {l3:.2f}')

    # print(f'adc1: {adc1_1} volts1: {v1:.2f} liters1: {l1:.2f}')
    # print(f'adc2: {adc1_2} volts2: {v2:.2f} liters2: {l2:.2f}')
    # print(f'adc3: {adc1_3} volts3: {v3:.2f} liters3: {l3:.2f}')
    # print(f'adc4: {adc1_4} volts4: {v4:.2f} liters4: {l4:.2f}')

    p += 1
    print(p)    

    sleep(1)
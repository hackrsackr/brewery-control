"""
main script
reads 4 ads1115 ADC boards outputs into one dictionary
publishes output to brewblox over mqtt
"""
import json

from time import sleep

from paho.mqtt import client as mqtt

from ads1115 import ADS1115
from Meter import Meter
from VolumeSensor import VolumeSensor

with open('config.json', 'r') as f:
    cfg = json.load(f)
    # print(cfg)

# Brewblox Host ip address
HOST = cfg['_HOST']

# Brewblox Host ip address
HOSTNAME = cfg['_HOSTNAME']

# Brewblox Port
PORT = cfg['_PORT']

# The history service is subscribed to all topic starting with this
HISTORY_TOPIC = cfg['_HISTORY_TOPIC']

# Specific topic for this script
TOPIC = HISTORY_TOPIC + cfg['_SERVICE_TOPIC']

# This is a constant value. You never need to change it.
API_TOPIC = cfg['_API_TOPIC']

# We'll be using the 'patch' command
# 'create', 'write', and 'delete' are also available
# https://www.brewblox.com/dev/reference/blocks_api.html
PATCH_TOPIC = API_TOPIC + '/patch'

# Ads Addresses
# ADS1 = ADS1115(address=0x48)  # ADDRESS -> GND
# ADS2 = ADS1115(address=0x49)  # ADDRESS -> VDD
# ADS3 = ADS1115(address=0x4a)  # ADDRESS -> SDA
# ADS4 = ADS1115(address=0x4b)  # ADDRESS -> SDL

# Max positive bits of ADS1115's 16 bit signed integer
ADS_FULLSCALE = cfg['_ADS_FULLSCALE']
GAIN = 2/3
ADS_MAX_V = 4.096 / GAIN
ADS1115_ADDRESS_1 = 0x48
ADS1115_ADDRESS_2 = 0x49
ADS1115_ADDRESS_3 = 0x4a
ADS1115_ADDRESS_4 = 0x4b

# Create a websocket MQTT client
client = mqtt.Client()

def main():
    try:
        # Create a websocket MQTT client
        client.connect_async(host=HOST, port=PORT)
        client.loop_start()

        while True:
            # Iterate through ads1 channels, populate dict d1
            d1 = {}
            for input in cfg['_METERS']['meter-1']:
                m1 = Meter()
                m1.ads = ADS1115(ADS1115_ADDRESS_1)
                m1.meter_id = cfg['_METERS']['meter-1'][input]['meter_id']
                m1.name = cfg['_METERS']['meter-1'][input]['name']
                m1.unit = cfg['_METERS']['meter-1'][input]['unit']
                m1.ads_channel = cfg['_METERS']['meter-1'][input]['ads_channel']
                m1.ilrv = cfg['_METERS']['meter-1'][input]['input_LRV']
                m1.iurv = cfg['_METERS']['meter-1'][input]['input_URV']
                m1.olrv = cfg['_METERS']['meter-1'][input]['output_LRV']
                m1.ourv = cfg['_METERS']['meter-1'][input]['output_URV']
                m1.offset = cfg['_METERS']['meter-1'][input]['offset']

                d1[m1.name] = {
                    # 'mA': round(m1.readMa(), 2),
                    # 'volts': round(m1.volts, 2),
                    m1.unit: round(m1.maToUnit(), 2)
                }

            # Iterate through ads2 channels, populate dict d2
            d2 = {}
            for input in cfg['_METERS']['meter-2']:
                m2 = Meter()
                m2.ads = ADS1115(ADS1115_ADDRESS_2)
                m2.meter_id = cfg['_METERS']['meter-2'][input]['meter_id']
                m2.name = cfg['_METERS']['meter-2'][input]['name']
                m2.unit = cfg['_METERS']['meter-2'][input]['unit']
                m2.ads_channel = cfg['_METERS']['meter-2'][input]['ads_channel']
                m2.ilrv = cfg['_METERS']['meter-2'][input]['input_LRV']
                m2.iurv = cfg['_METERS']['meter-2'][input]['input_URV']
                m2.olrv = cfg['_METERS']['meter-2'][input]['output_LRV']
                m2.ourv = cfg['_METERS']['meter-2'][input]['output_URV']
                m2.offset = cfg['_METERS']['meter-2'][input]['offset']

                d2[m2.name] = {
                    # 'mA': round(m2.readMa(), 2),
                    # 'volts': round(m2.volts, 2),
                    m2.unit: round(m2.maToUnit(), 2)
                }

            # # Iterate through ads3 channels, populate dict d3
            d3 = {}
            patch_list = [0]*3

            for index, input in enumerate(cfg['_METERS']['meter-3']):
                if input == 'input-4':
                    m3 = Meter()
                    m3.ads = ADS1115(ADS1115_ADDRESS_3)
                    m3.meter_id = cfg['_METERS']['meter-3'][input]['meter_id']
                    m3.name = cfg['_METERS']['meter-3'][input]['name']
                    m3.unit = cfg['_METERS']['meter-3'][input]['unit']
                    m3.ads_channel = cfg['_METERS']['meter-3'][input]['ads_channel']
                    m3.ilrv = cfg['_METERS']['meter-3'][input]['input_LRV']
                    m3.iurv = cfg['_METERS']['meter-3'][input]['input_URV']
                    m3.olrv = cfg['_METERS']['meter-3'][input]['output_LRV']
                    m3.ourv = cfg['_METERS']['meter-3'][input]['output_URV']
                    m3.offset = cfg['_METERS']['meter-3'][input]['offset']

                    d3[m3.name] = {
                            # 'mA': round(m3.readMa(), 2),
                            # 'volts': round(m3.volts, 2),
                            m3.unit: round(m3.maToUnit(), 2)}
                else: 
                    m3 = VolumeSensor()
                    m3.ads = ADS1115(ADS1115_ADDRESS_3)
                    m3.meter_id = cfg['_METERS']['meter-3'][input]['meter_id']
                    m3.name = cfg['_METERS']['meter-3'][input]['name']
                    m3.unit = cfg['_METERS']['meter-3'][input]['unit']
                    m3.ads_channel = cfg['_METERS']['meter-3'][input]['ads_channel']
                    m3.ads_offset = cfg['_METERS']['meter-3'][input]['ads_offset']
                    m3.adc = m3.ads.readADC(m3.ads_channel, gain=GAIN)
                    m3.value = round(m3.readLiters(), 2)

                    d3[m3.name] = {
                            'adc': m3.ads.readADC(m3.ads_channel, gain=GAIN),
                            'trimmed_adc': m3.adc - m3.ads_offset,
                            m3.unit: m3.value
                            }
                    patch_list[index] = d3[m3.name]['liters']

            d4 = {}
            for input in cfg['_METERS']['meter-4']:
                m4 = Meter()
                m4.ads = ADS1115(ADS1115_ADDRESS_4)
                m4.meter_id = cfg['_METERS']['meter-4'][input]['meter_id']
                m4.name = cfg['_METERS']['meter-4'][input]['name']
                m4.unit = cfg['_METERS']['meter-4'][input]['unit']
                m4.ads_channel = cfg['_METERS']['meter-4'][input]['ads_channel']
                m4.ilrv = cfg['_METERS']['meter-4'][input]['input_LRV']
                m4.iurv = cfg['_METERS']['meter-4'][input]['input_URV']
                m4.olrv = cfg['_METERS']['meter-4'][input]['output_LRV']
                m4.ourv = cfg['_METERS']['meter-4'][input]['output_URV']
                m4.offset = cfg['_METERS']['meter-4'][input]['offset']

                d4[m4.name] = {
                    # 'mA': round(m4.readMa(), 2),
                    # 'volts': round(m4.volts, 2),
                    m4.unit: round(m4.maToUnit(), 3)
                }

            # Output
            message = {
                'key': 'meters',
                'data': {'pH': d1,
                         'DO': d2,
                         'volume': d3,
                         'flow': d4}
            }

            liqr_liters = {
                'id': 'EXT_liqrLiters',
                'serviceId': HOSTNAME,
                'type': 'TempSensorExternal',
                'data': {'setting[degC]': patch_list[0]},
            }

            mash_liters = {
                'id': 'EXT_mashLiters',
                'serviceId': HOSTNAME,
                'type': 'TempSensorExternal',
                'data': {'setting[degC]': patch_list[1]},
            }

            boil_liters = {
                'id': 'EXT_boilLiters',
                'serviceId': HOSTNAME,
                'type': 'TempSensorExternal',
                'data': {'setting[degC]': patch_list[2]},
            }

            client.publish(PATCH_TOPIC, json.dumps(liqr_liters))
            client.publish(PATCH_TOPIC, json.dumps(mash_liters))
            client.publish(PATCH_TOPIC, json.dumps(boil_liters))

            client.publish(TOPIC, json.dumps(message))


            # print(json.dumps(message, sort_keys=False, indent=4))

            print(f'liqr_liters: {patch_list[0]}\n'
                  f'mash_liters: {patch_list[1]}\n'
                  f'boil_liters: {patch_list[2]}\n')
            
            sleep(2.5)

    finally:
        client.loop_stop()


if __name__ == '__main__':
    main()

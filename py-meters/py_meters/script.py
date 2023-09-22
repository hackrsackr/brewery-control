"""
main script
reads up to 4 ads1115 ADC boards outputs into one dictionary
publishes output to brewblox over mqtt
"""
import json
import serial

from time import sleep

from paho.mqtt import client as mqtt

from ads1115 import ADS1115
from Meter import Meter
from VolumeSensor import VolumeSensor

# Brewblox Host ip address
HOST = '10.0.0.101'

# Brewblox Port
PORT = 1883

# The history service is subscribed to all topic starting with this
HISTORY_TOPIC = 'brewcast/history'

# Specific topic for this script
TOPIC = HISTORY_TOPIC + '/meters'

# This is a constant value. You never need to change it.
API_TOPIC = 'brewcast/spark/blocks'

# We'll be using the 'patch' command
# 'create', 'write', and 'delete' are also available
# https://www.brewblox.com/dev/reference/blocks_api.html
PATCH_TOPIC = API_TOPIC + '/patch'

# Create a websocket MQTT client
client = mqtt.Client()

# ADS1115 names and addresses
ads1 = ADS1115(address=0x48)  # ADDRESS -> GND
ads2 = ADS1115(address=0x49)  # ADDRESS -> VDD
ads3 = ADS1115(address=0x4a)  # ADDRESS -> SDA

# Max positive bits of ADS1115's 16 bit signed integer
ADS_FULLSCALE = 32767
GAIN = 2/3
ADS_MAX_V = 4.096 / GAIN

# Names of each input
ads1_keys = ['mash_mV', 'boil_mV', 'mash', 'boil']
ads2_keys = ['liqr_nA', 'wort_nA', 'liqr', 'wort']
ads3_keys = ['liqr', 'mash', 'boil']

def main():
    try:
        # Create a websocket MQTT client
        client.connect_async(host=HOST, port=PORT)
        client.loop_start()

        while True:
            # Iterate through ads1 channels, populate dict d1
            d1 = {}
            for index, key in enumerate(ads1_keys):
                m1 = Meter()
                m1.name = key
                m1.ads = ADS1115(address=0x48)  # ADDRESS -> GND

                d1[m1.name] = {
                    'mA': round(m1.read_mA(index), 2),
                    'mV': round(m1.mA_to_mV(m1.mA), 2),
                    'pH': round(m1.mA_to_pH(m1.mA), 2)
                }

            # Iterate through ads2 channels, populate dict d2
            d2 = {}
            for index, key in enumerate(ads2_keys):
                m2 = Meter()
                m2.name = key
                m2.ads = ADS1115(address=0x49)  # ADDRESS -> VCC

                d2[m2.name] = {
                    'mA': round(m2.read_mA(index), 2),
                    'nA': round(m2.mA_to_nA(m2.mA), 2),
                    'DO': round(m2.mA_to_DO(m2.mA), 2)
                }

            # Iterate through ads3 channels, populate dict d3
            d3 = {}

            volume_sensor_offsets = [8000, 5824, 6960]
            patch_list = [0]*3

            for index, key in enumerate(ads3_keys):
                v = VolumeSensor()
                v.name = key
                v.ads = ADS1115(address=0x4a)  # ADDRESS -> SDA
                v.offset = volume_sensor_offsets[index]

                d3[v.name] = {
                    'calibration': {
                        'adc': v.read_ads(index),
                        'trimmed-adc': v.trim_adc(v.adc, v.offset),
                        'volts': round(v.read_volts(index), 2)
                    },
                    'liters': round(v.adc_to_liters(), 2),
                    'gallons': round(v.adc_to_gallons(), 2)
                }

                patch_list[index] = d3[v.name]['liters']

            # Output
            message = {
                'key': 'meters',
                'data': {'pH': d1,
                         'DO': d2,
                         'volume': d3}
            }

            client.publish(TOPIC, json.dumps(message))
            print(json.dumps(message, sort_keys=False, indent=4))

            # IMPORTANT !!
            # Don't forget to first create the 'Tutorial Sensor' block
            # Patch writes to non-existent blocks will be ignored
            # The block ID
            # The unique service name
            # https://www.brewblox.com/dev/reference/block_types.html#tempsensorexternal
            # We only write the field we want to change
            # Because we're using patch, the other settings will remain unchanged
            liqr_liters = {
                'id': 'EXT_liqrLiters',
                'serviceId': 'spark-three',
                'type': 'TempSensorExternal',
                'data': {'setting[degC]': patch_list[0]},
            }

            mash_liters = {
                'id': 'EXT_mashLiters',
                'serviceId': 'spark-three',
                'type': 'TempSensorExternal',
                'data': {'setting[degC]': patch_list[1]},
            }

            boil_liters = {
                'id': 'EXT_boilLiters',
                'serviceId': 'spark-three',
                'type': 'TempSensorExternal',
                'data': {'setting[degC]': patch_list[2]},
            }

            client.publish(PATCH_TOPIC, json.dumps(liqr_liters))
            client.publish(PATCH_TOPIC, json.dumps(mash_liters))
            client.publish(PATCH_TOPIC, json.dumps(boil_liters))

            print(f'sent {liqr_liters, mash_liters, boil_liters}')

            sleep(5)

    finally:
        client.loop_stop


if __name__ == '__main__':
    main()

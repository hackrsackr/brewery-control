"""
main script

reads 3 level sensors and publishes output to brewblox over mqtt
patches 3 external temp sensors with level sensor data in liters

"""
import json

from time import sleep

from paho.mqtt import client as mqtt

from ads1115 import ADS1115
from VolumeSensor import VolumeSensor

# Brewblox Host ip address
HOST = '192.168.1.2'

# Brewblox Port
PORT = 80

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
client = mqtt.Client(transport='websockets')
client.ws_set_options(path='/eventbus')

# Create an ADS1115 instance
ads3 = ADS1115(address=0x4a)  # ADDRESS -> SDA

# Names of each input
ads3_keys = ['liqr', 'mash', 'boil', 'unused']


def main():
    try:
        # Create a websocket MQTT client
        client.connect_async(host=HOST, port=PORT)
        client.loop_start()

        while True:
            # Iterate through ads3 channels, populate dict d3
            d3 = {}
            adc3_offsets = [8000, 5824, 6960, 6960]
            patch_list = [0, 0, 0, 0]
            for index, ads3_key in enumerate(ads3_keys):
                v = VolumeSensor()
                v.name = ads3_key
                v.ads = ads3

                d3[v.name] = {
                    'adc': v.read_ads(index),
                    'trimmed-adc': v.trim_adc(v.adc, adc3_offsets[index]),
                    'volts': round(v.read_volts(index), 2),
                    'liters': round(v.adc_to_liters(), 2),
                    'gallons': round(v.adc_to_gallons(), 2)
                }

                patch_list[index] = d3[v.name]['liters']

            message = {
                'key': 'volume-sensors',
                'data': d3
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
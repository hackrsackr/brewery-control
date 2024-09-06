"""
Code example for publishing data to the Brewblox eventbus

Dependencies:
- paho-mqtt
"""

import json
from time import sleep

from paho.mqtt import client as mqtt

from ads1115 import ADS1115

with open('config.json', 'r') as f:
    cfg = json.load(f)

# 172.17.0.1 is the default IP address for the host running the Docker container
# Change this value if Brewblox is installed on a different computer
HOST = cfg['_HOST']

# 80 is the default port for HTTP, but this can be changed in brewblox env settings.
PORT = cfg['_PORT']

# This is a constant value. You never need to change it.
HISTORY_TOPIC = cfg['_HISTORY_TOPIC']
TOPIC = HISTORY_TOPIC + cfg['_SERVICE_TOPIC']

# Ads setup
ADS_FULLSCALE = cfg['_ADS_FULLSCALE']
GAIN = 2/3
ADS_MAX_V = 4.096 / GAIN


# Create a websocket MQTT client
client = mqtt.Client(transport='websockets')
client.ws_set_options(path='/eventbus')

Sensors = ['liqr', 'mash', 'boil']
Offsets = [8000, 5824, 6960]

class VolumeSensor:
    def __init__(self) -> None:
        # Create a websocket MQTT client
        # self.client = mqtt.Client(transport='websockets')
        # self.client.ws_set_options(path='/eventbus')

        self.bit_max = ADS_FULLSCALE
        self.adsMaxV = ADS_MAX_V

        self.bitsPerGallon = 1675
        self.bitsPerLiter = 442.54

    def read_ads(self) -> int:
        self.adc = self.ads.readADC(self.ads_channel, gain=GAIN)
        return self.adc

    def trim_adc(self) -> int:
        self.trimmed_adc = self.adc - self.ads_offset
        return self.trimmed_adc

    def read_volts(self) -> float:
        self.volts = self.read_ads() * ADS_MAX_V / ADS_FULLSCALE
        return self.volts

    def adc_to_volts(self) -> float:
        return self.adc * self.adsMaxV / self.bit_max

    def adc_to_gallons(self) -> float:
        self.gallons = self.trimmed_adc / self.bitsPerGallon
        return self.gallons if self.gallons > 0 else 0

    def readLiters(self) -> float:
        self.trim_adc()
        self.liters = self.trimmed_adc / self.bitsPerLiter

        return self.liters if self.liters > 0 else 0

    def adc_to_liters(self) -> float:
        self.liters = self.trimmed_adc / self.bitsPerLiter
        return self.liters if self.liters > 0 else 0

    def run(self):
        try:
            client.connect_async(host=HOST, port=PORT)
            client.loop_start()

            while True:
                data = {}

                for index, sensor in enumerate(Sensors):
                    self.__init__()
                    self.ads = ADS1115(address=0x4a)
                    self.meter_id = index
                    self.name = sensor
                    self.measurement = 'liters'
                    self.ads_channel = index
                    self.ads_offset = Offsets[index]

                    #data[self.name] = {
                    #    'volts': round(self.read_volts(), 2),
                    #    self.measurement: round(self.readLiters(), 2),
                    #}
                    data[self.name] = {
                        'adc': self.read_ads(),
                        'trimmed-adc': self.trim_adc(),
                        'volts': round(self.read_volts(), 2),
                        'liters': round(self.adc_to_liters(), 2),
                        'gallons': round(self.adc_to_gallons(), 2)
                    }

                # MQTT message to send to brewblox
                message = {
                    'key': 'VolumeSensors',
                    'data': data
                }

                # Publish message
                #client.publish(TOPIC, json.dumps(message))
                print(json.dumps(message, sort_keys=False, indent=4))
                sleep(5)

        finally:
            client.loop_stop()


if __name__ == '__main__':
    VolumeSensor().run()

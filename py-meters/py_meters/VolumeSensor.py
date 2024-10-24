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

ADS1 = ADS1115(address=0x48)  # ADDRESS -> GND
ADS2 = ADS1115(address=0x49)  # ADDRESS -> VDD
ADS3 = ADS1115(address=0x4a)  # ADDRESS -> SDA
ADS4 = ADS1115(address=0x4b)  # ADDRESS -> SDL


# Create a websocket MQTT client
client = mqtt.Client(transport='websockets')
client.ws_set_options(path='/eventbus')

Sensors = ['liqr', 'mash', 'boil']
Offsets = [8000, 5824, 6960]

class VolumeSensor:
    def __init__(self) -> None:
        self.bit_max = ADS_FULLSCALE
        self.adsMaxV = ADS_MAX_V
        self.bitsPerGallon = 1675
        self.bitsPerLiter = 442
    
    def read(self) -> None:
        self.adc = self.ads.readADC(self.ads_channel, gain=GAIN)
        self.trimmed_adc = self.adc - self.ads_offset
        self.volts = self.trimmed_adc * ADS_MAX_V / ADS_FULLSCALE
        self.liters = ( self.adc - self.ads_offset ) / self.bitsPerLiter
        self.gallons = ( self.adc - self.ads_offset ) / self.bitsPerGallon

    def readADS(self) -> int:
        self.adc = self.ads.readADC(self.ads_channel, gain=GAIN)
        return self.adc

    def trimADC(self) -> int:
        self.trimmed_adc = self.adc - self.ads_offset
        return self.trimmed_adc

    def readVolts(self) -> float:
        self.volts = self.read_ads() * ADS_MAX_V / ADS_FULLSCALE
        return self.volts

    #def adc_to_volts(self) -> float:
     #   return self.adc * self.adsMaxV / self.bit_max

    def readGallons(self) -> float:
        self.gallons = ( self.adc - self.ads_offset ) / self.bitsPerGallon
        return self.gallons if self.gallons > 0 else 0

    def readLiters(self) -> float:
        self.liters = ( self.adc - self.ads_offset ) / self.bitsPerLiter

        return self.liters if self.liters > 0 else 0

    def run(self):
        try:
            client.connect_async(host=HOST, port=PORT)
            client.loop_start()

            while True:
                data = {}

                for index, sensor in enumerate(Sensors):
                    self.__init__()
                    self.meter_id = index
                    self.name = sensor
                    self.measurement = 'liters'
                    self.ads_channel = index
                    self.ads_offset = Offsets[index]

                    #data[self.name] = {
                    #    'volts': round(self.readVolts(), 2),
                    #    sel.measurement: round(self.readLiters(), 2),
                    #}
                    data[self.name] = {
                        'adc': self.ads.readADC(),
                        'trimmed-adc': self.adc - self.ads_offset,
                        'volts': round(self.readVolts(), 2),
                        'liters': round(self.readliters(), 2)
                        # ,
                        # 'gallons': round(self.readGallons(), 2)
                    }

                # MQTT message to send to brewblox
                message = {
                    'key': 'VolumeSensors',
                    'data': data
                }

                # Publish message
                client.publish(TOPIC, json.dumps(message))
                # print(json.dumps(message, sort_keys=False, indent=4))
                sleep(5)

        finally:
            client.loop_stop()


if __name__ == '__main__':
    VolumeSensor().run()

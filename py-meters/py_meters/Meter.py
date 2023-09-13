import json
from time import sleep

from paho.mqtt import client as mqtt

from ads1115 import ADS1115

# Brewblox Host ip address
HOST = '192.168.1.2'

# Brewblox Port
PORT = 80

# The history service is subscribed to all topic starting with this
HISTORY_TOPIC = 'brewcast/history'
TOPIC = HISTORY_TOPIC + '/meters'

"""
ADS1115 names and addresses
ads1 = ADS1115(address=0x48)  # ADDRESS -> GND
ads2 = ADS1115(address=0x49)  # ADDRESS -> VDD
ads3 = ADS1115(address=0x4a)  # ADDRESS -> SDA
ads4 = ADS1115(address=0x4b)  # ADDRESS -> SDL
"""
ads1 = ADS1115(address=0x48)
ads2 = ADS1115(address=0x49)

# Max positive bits of ADS1115's 16 bit signed integer
ADS_FULLSCALE = 32767
GAIN = 2/3
ADS_MAX_V = 4.096 / GAIN

ads1_keys = ['m-1_output-1', 'm-1_output-2', 'm-1_output-3', 'm-1_output-4']
ads2_keys = ['m-2_output-1', 'm-2_output-2', 'm-2_output-3', 'm-2_output-4']


class Meter:
    def __init__(self) -> None:
        # Create a websocket MQTT client
        self.client = mqtt.Client(transport='websockets')
        self.client.ws_set_options(path='/eventbus')

        self.bit_max = ADS_FULLSCALE
        self.adsMaxV = ADS_MAX_V

    def read_ads(self, channel, offset=0) -> int:
        self.adc = self.ads.read_adc(channel, gain=GAIN) + offset
        return self.adc

    def read_mA(self, channel) -> float:
        self.mA = self.read_ads(channel) * ADS_MAX_V / ADS_FULLSCALE * 4
        return self.mA

    def mA_to_Volts(self, mA) -> float:
        return mA / 4

    def mA_to_pH(self, mA) -> float:
        return mA / 2

    def mA_to_mV(self, mA) -> float:
        return mA * 50 - 500

    def mA_to_DO(self, mA) -> float:
        return mA / 2

    def mA_to_nA(self, mA) -> float:
        return mA * 10

    def run(self):
        try:
            self.client.connect_async(host=HOST, port=PORT)
            self.client.loop_start()

            while True:
                """ Iterate through ads1 channels and compile data """
                d1 = {}
                for index, ads1_key in enumerate(ads1_keys):
                    self.name = ads1_key
                    self.ads = ads1

                    d1[self.name] = {
                        'adc': self.read_ads(index),
                        'mA': round(self.read_mA(index), 2),
                        'pH': round(self.mA_to_pH(self.mA), 2),
                        'ORP': round(self.mA_to_mV(self.mA), 2),
                        'DO': round(self.mA_to_DO(self.mA), 2),
                        'nA': round(self.mA_to_nA(self.mA), 2)
                    }

                """ Iterate through ads1 channels and compile data """
                d2 = {}
                for index, ads2_key in enumerate(ads2_keys):
                    self.name = ads2_key
                    self.ads = ads2

                    d2[self.name] = {
                        'adc': self.read_ads(index),
                        'mA': round(self.read_mA(index), 2),
                        'pH': round(self.mA_to_pH(self.mA), 2),
                        'ORP': round(self.mA_to_mV(self.mA), 2),
                        'DO': round(self.mA_to_DO(self.mA), 2),
                        'nA': round(self.mA_to_nA(self.mA), 2)
                    }

                """ Output """
                message = {
                    'key': 'meters',
                    'data': {'meter-1': d1, 'meter-2': d2}
                }

                self.client.publish(TOPIC, json.dumps(message))
                print(json.dumps(message, sort_keys=False, indent=4))
                sleep(5)

        finally:
            self.client.loop_stop()


if __name__ == '__main__':
    Meter().run()

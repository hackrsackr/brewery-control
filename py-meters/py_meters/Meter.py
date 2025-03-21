import json
from time import sleep

from paho.mqtt import client as mqtt

from ads1115 import ADS1115

with open('config.json', 'r') as f:
    cfg = json.load(f)

# Brewblox Host ip address
HOST = cfg['_HOST']

# Brewblox Port
PORT = cfg['_PORT']

# The history service is subscribed to all topic starting with this
HISTORY_TOPIC = cfg['_HISTORY_TOPIC']
TOPIC = HISTORY_TOPIC + cfg['_SERVICE_TOPIC']

# Ads Addresses
ADS1 = ADS1115(address=0x48)  # ADDRESS -> GND
ADS2 = ADS1115(address=0x49)  # ADDRESS -> VDD
ADS3 = ADS1115(address=0x4a)  # ADDRESS -> SDA
ADS4 = ADS1115(address=0x4b)  # ADDRESS -> SDL

# Max positive bits of ADS1115's 16 bit signed integer
ADS_FULLSCALE = cfg['_ADS_FULLSCALE']
GAIN = 2/3
ADS_MAX_V = 4.096 / GAIN

# Create a websocket MQTT client
client = mqtt.Client()


class Meter:
    def __init__(self) -> None:
        self.bit_max = ADS_FULLSCALE
        self.adsMaxV = ADS_MAX_V

    def read_ads(self, channel, offset=0) -> int:
        self.adc = self.ads.readADC(channel, gain=GAIN) + offset
        return self.adc

    def readVolts(self) -> float:
        self.volts = self.read_ads(self.ads_channel) * ADS_MAX_V / ADS_FULLSCALE
        return self.volts

    def readMa(self) -> float:
        self.mA = self.readVolts() * 4
        return self.mA

    def maToUnit(self) -> float:
        self.readMa()
        if self.mA > self.ilrv and self.mA < self.iurv:
            self.measurement = (self.mA - self.ilrv) / (self.iurv - self.ilrv) * (self.ourv - self.olrv) + self.offset
        else:
            self.measurement = 0.0 + self.offset
        return self.measurement


    def run(self):
        try:
            client.connect_async(host=HOST, port=PORT)
            client.loop_start()

            while True:
                """ Iterate through ads1 channels and compile data """
                d1 = {}
                for input in cfg['_METERS']['meter-1']:
                    self.__init__()
                    self.ads = ADS1
                    self.meter_id = cfg['_METERS']['meter-1'][input]['meter_id']
                    self.name = cfg['_METERS']['meter-1'][input]['name']
                    self.measurement = cfg['_METERS']['meter-1'][input]['measurement']
                    self.ads_channel = cfg['_METERS']['meter-1'][input]['ads_channel']
                    self.ilrv = cfg['_METERS']['meter-1'][input]['input_LRV']
                    self.iurv = cfg['_METERS']['meter-1'][input]['input_URV']
                    self.olrv = cfg['_METERS']['meter-1'][input]['output_LRV']
                    self.ourv = cfg['_METERS']['meter-1'][input]['output_URV']
                    self.offset = cfg['_METERS']['meter-1'][input]['offset']

                    d1[self.name] = {
                        'mA': round(self.readMa(), 2),
                        'volts': round(self.volts, 2),
                        self.measurement: round(self.maToUnit(), 2),
                    }

                """ Iterate through ads1 channels and compile data """
                d2 = {}
                for input in cfg['_METERS']['meter-2']:
                    self.__init__()
                    self.ads = ADS2
                    self.meter_id = cfg['_METERS']['meter-2'][input]['meter_id']
                    self.name = cfg['_METERS']['meter-2'][input]['name']
                    self.measurement = cfg['_METERS']['meter-2'][input]['measurement']
                    self.ads_channel = cfg['_METERS']['meter-2'][input]['ads_channel']
                    self.ilrv = cfg['_METERS']['meter-2'][input]['input_LRV']
                    self.iurv = cfg['_METERS']['meter-2'][input]['input_URV']
                    self.olrv = cfg['_METERS']['meter-2'][input]['output_LRV']
                    self.ourv = cfg['_METERS']['meter-2'][input]['output_URV']
                    self.offset = cfg['_METERS']['meter-2'][input]['offset']

                    d2[self.name] = {
                        # 'mA': round(self.readMa(), 2),
                        'volts': round(self.volts, 2),
                        self.measurement: round(self.maToUnit(), 2),
                    }

                """ Output """
                message = {
                    'key': 'meters',
                    #'data': {'pH': d1, 'DO': d2}
                    'data': {'DO': d2}
                }

                # client.publish(TOPIC, json.dumps(message))
                print(json.dumps(message, sort_keys=False, indent=4))
                sleep(5)

        finally:
            client.loop_stop()


if __name__ == '__main__':
    Meter().run()

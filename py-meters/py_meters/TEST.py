"""
main script
reads 4 ads1115 ADC boards outputs into one dictionary
publishes output to brewblox over mqtt
"""
import json

from time import sleep

# from paho.mqtt import client as mqtt

from ads1115 import ADS1115
# from Meter import Meter
# from VolumeSensor import VolumeSensor

with open('config.json', 'r') as f:
    cfg = json.load(f)
    # print(cfg)

# # Brewblox Host ip address
# HOST = cfg['_HOST']

# # Brewblox Host ip address
# HOSTNAME = cfg['_HOSTNAME']

# # Brewblox Port
# PORT = cfg['_PORT']

# # The history service is subscribed to all topic starting with this
# HISTORY_TOPIC = cfg['_HISTORY_TOPIC']

# # Specific topic for this script
# TOPIC = HISTORY_TOPIC + cfg['_SERVICE_TOPIC']

# # This is a constant value. You never need to change it.
# API_TOPIC = cfg['_API_TOPIC']

# # We'll be using the 'patch' command
# # 'create', 'write', and 'delete' are also available
# # https://www.brewblox.com/dev/reference/blocks_api.html
# PATCH_TOPIC = API_TOPIC + '/patch'

# Ads Addresses
# ADS1 = ADS1115(address=0x48)  # ADDRESS -> GND
# ADS2 = ADS1115(address=0x49)  # ADDRESS -> VDD
ADS3 = ADS1115(address=0x4a)  # ADDRESS -> SDA
ADS3 = ADS1115(address=0x4a)  # ADDRESS -> SDA
# ADS4 = ADS1115(address=0x4b)  # ADDRESS -> SDL

# Max positive bits of ADS1115's 16 bit signed integer
ADS_FULLSCALE = cfg['_ADS_FULLSCALE']
GAIN = 2/3
ADS_MAX_V = 4.096 / GAIN

# Create a websocket MQTT client
# client = mqtt.Client()

def main():
    a = "0x48"
    print(type(a))
      





if __name__ == '__main__':
    main()

"""
Example for reading from a serial port inside a container

Dependencies:
- pyserial
"""

import serial
import json
from time import sleep
from paho.mqtt import client as mqtt

HOST = '192.168.1.2'

PORT = 1883

# This is the default serial port
USB_PORT = '/dev/ttyUSB0'

HISTORY_TOPIC = 'brewcast/history'

TOPIC = HISTORY_TOPIC + '/spund-system'

client = mqtt.Client()

# You may need to further configure settings
# See the pyserial documentation for more info
# https://pythonhosted.org/pyserial/pyserial_api.html#classes
ser = serial.Serial(port=USB_PORT,
                    baudrate=115200,
                    timeout=1)

def main():
    try:
        # Create a websocket MQTT client
        client.connect_async(host=HOST, port=PORT)
        client.loop_start()

        while True:
            # Read raw data from the stream
            # Convert the binary string to a normal string
            # Remove the trailing newline character
            spund_data = ser.readline().decode().rstrip()
            
            try:
                spund_data = json.loads(spund_data)
            except json.JSONDecodeError:
                continue

            message = {
                'key': 'spund-system',
                'data': spund_data
            }
            
            client.publish(TOPIC, json.dumps(message))
            print(json.dumps(message, indent=4))
            sleep(5)
    finally:
        ser.close()
        client.loop_stop()

if __name__ == '__main__':
    main()

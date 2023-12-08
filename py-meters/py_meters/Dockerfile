FROM python:3.9 as base

COPY config.json /config.json
COPY ads1115.py /app/ads1115.py
COPY Meter.py /app/Meter.py
COPY VolumeSensor.py /app/VolumeSensor.py
COPY script.py /app/script.py

# install the required packages
RUN apt-get update && apt-get -y install build-essential
<<<<<<< HEAD
# RUN pip3 install pyserial
=======
>>>>>>> c2874f2f5efdf2e886bc14315d1a8f17918d5ccb
RUN pip3 install paho.mqtt
RUN pip3 install adafruit-gpio

CMD ["python3", "-u", "/app/script.py"]

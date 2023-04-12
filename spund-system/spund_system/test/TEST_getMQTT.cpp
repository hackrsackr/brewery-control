#include "Arduino.h"
#include "ArduinoJson.h"
#include "EspMQTTClient.h"
#include <array>

#include "ADS_Sensor.h"
#include "Spund_Sytem.h"
#include "Pressure_Sensor.h"
#include "relay.h"
#include "config.h"

std::array<Spund_System, _NUMBER_OF_SPUNDERS> spund_arr;
EspMQTTClient client(_SSID, _PASS, _MQTTHOST, _CLIENTID, _MQTTPORT);
StaticJsonDocument<4096> input;

void initWiFi();
void onConnectionEstablished();
void publishData();

void setup(void)
{
    Serial.begin(115200);
    client.setMaxPacketSize(4096);
    client.enableOTA();

    initWiFi();

    for (uint8_t i = 0; i < _NUMBER_OF_SPUNDERS; ++i)
    {
        spund_arr[i].begin(ADS1115_ADDRESS1, GAIN_TWOTHIRDS, OFFSET_VOLTS[i], UNIT_MAXS[i], RELAY_PINS[i]);
        spund_arr[i].name = std::to_string(i + 1);
        spund_arr[i].mqtt_field = _MQTT_FIELDS[i];
    }
}

void loop(void)
{
    client.loop();
}

void initWiFi()
{
    WiFi.disconnect(true);
    delay(1000);
    WiFi.begin(_SSID, _PASS);
    uint8_t failed_connections = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("connecting..");
        failed_connections++;
        if (failed_connections > 20)
        {
            Serial.println("restarting..");
            ESP.restart();
        }
    }
    Serial.print("Connected to ");
    Serial.println(WiFi.localIP());
}

void onConnectionEstablished()
{
    client.subscribe(_SUBTOPIC, [](const String &payload)
                     {
    // Serial.println(payload);
    deserializeJson(input, payload);
    for (uint8_t i = 0; i < _NUMBER_OF_SPUNDERS; ++i)
    {
        spund_arr[i].tempC = input["data"][spund_arr[i].mqtt_field]["value[degC]"];
    }
    publishData(); });
}

void publishData()
{
    StaticJsonDocument<768> message;
    message["key"] = _CLIENTID;

    if (!WiFi.isConnected())
    {
        initWiFi();
    }

    if (!client.isConnected())
    {
        ESP.restart();
    }

    for (uint8_t i = 0; i < _NUMBER_OF_SPUNDERS; ++i)
    {
        if (!spund_arr[i].tempC)
        {
            Serial.println(" no temp reading");
            continue;
        }
        else
        {
            // spund_arr[spunder].spunder_run();

            message["data"][spund_arr[i].name]["tempC"] = spund_arr[i].tempC;
            message["data"][spund_arr[i].name]["volts"] = spund_arr[i].getVolts(i);
            message["data"][spund_arr[i].name]["volts"] = spund_arr[i].getNetVolts(i);
            // message["data"][spund_arr[spunder].name]["psi_setpoint"] = spund_arr[spunder].psi_setpoint;
            // message["data"][spund_arr[spunder].name]["psi"] = spund_arr[spunder].psi_value;
            // message["data"][spund_arr[spunder].name]["vols_target"] = spund_arr[spunder].vols_setpoint;
            // message["data"][spund_arr[spunder].name]["volumes[co2]"] = spund_arr[spunder].vols_value;
            // message["data"][spund_arr[spunder].name]["since_vent"] = spund_arr[spunder].time_since_vent;
            // message["data"][spund_arr[spunder].name]["vent_state"] = spund_arr[spunder].vent_state;
        }
    }
    serializeJsonPretty(message, Serial);
    // client.publish(_PUBTOPIC, message.as<String>());
    delay(5000);
}
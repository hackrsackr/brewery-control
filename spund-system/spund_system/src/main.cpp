#include "Arduino.h"

#include "ArduinoJson.h"
#include "EspMQTTClient.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <array>

#include "ADS_Sensor.h"
#include "Spund_System.h"
#include "Pressure_Sensor.h"
#include "Relay.h"
#include "config.h"
#include "server.h"

std::array<Spund_System, _NUMBER_OF_SPUNDERS> spund_arr;
EspMQTTClient client(_SSID, _PASS, _MQTTHOST, _CLIENTID, _MQTTPORT);
AsyncWebServer server(80);
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
        spund_arr[i].begin(
            ADS1115_ADDRESS1,
            GAIN_TWOTHIRDS,
            _OFFSET_VOLTS[i],
            _UNIT_MAXS[i],
            _RELAY_PINS[i],
            _DESIRED_VOLS[i]);
        spund_arr[i].id = _SPUNDER_NAMES[i];
        spund_arr[i].mqtt_field = _MQTT_FIELDS[i];
    }
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", index_html, processor); });
    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    String inputMessage;
    String inputParam;

    for (uint8_t i = 0; i < _NUMBER_OF_SPUNDERS; ++i)
    {
        if (request->hasParam(_SETPOINT_INPUTS[i])) {
            _SETPOINT_MESSAGES[i] = request->getParam(_SETPOINT_INPUTS[i])->value();
            spund_arr[i].vols_setpoint = _SETPOINT_MESSAGES[i].toFloat();
            inputMessage = _SETPOINT_MESSAGES[i];
            inputParam = _SETPOINT_INPUTS[i];
        }
        if (request->hasParam(_MQTT_INPUTS[i])) {
            _MQTT_MESSAGES[i] = request->getParam(_MQTT_INPUTS[i])->value();
            spund_arr[i].mqtt_field =  _MQTT_MESSAGES[i];
            inputMessage = _MQTT_MESSAGES[i];
            inputParam = _MQTT_INPUTS[i];
        }

    }

    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field ("
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>"); });
    server.onNotFound(notFound);
    server.begin();
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
    // message["key"] = _CLIENTID;

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
            // message["data"][spund_arr[i].id]["TempC"] = spund_arr[i].tempC;
            // message["data"][spund_arr[i].id]["TempF"] = spund_arr[i].getTempF(spund_arr[i].tempC);
            // message["data"][spund_arr[i].id]["Volts"] = spund_arr[i].getVolts(i);
            // message["data"][spund_arr[i].id]["PSI"] = spund_arr[i].getPSI(i);
            // message["data"][spund_arr[i].id]["PSI_setpoint"] = spund_arr[i].getPSISetpoint();
            // message["data"][spund_arr[i].id]["Vols_setpoint"] = spund_arr[i].vols_setpoint;
            // message["data"][spund_arr[i].id]["Vols"] = spund_arr[i].getVols();
            // message["data"][spund_arr[i].id]["Minutes_since_vent"] = spund_arr[i].test_carb();
            // message["data"][spund_arr[i].id]["Relay_state"] = spund_arr[i].relay_state;

            message[spund_arr[i].id]["TempC"] = spund_arr[i].tempC;
            message[spund_arr[i].id]["TempF"] = spund_arr[i].getTempF(spund_arr[i].tempC);
            message[spund_arr[i].id]["Volts"] = spund_arr[i].getVolts(i);
            message[spund_arr[i].id]["PSI"] = spund_arr[i].getPSI(i);
            message[spund_arr[i].id]["PSI_setpoint"] = spund_arr[i].getPSISetpoint();
            message[spund_arr[i].id]["Vols_setpoint"] = spund_arr[i].vols_setpoint;
            message[spund_arr[i].id]["Vols"] = spund_arr[i].getVols();
            message[spund_arr[i].id]["Minutes_since_vent"] = spund_arr[i].test_carb();
            message[spund_arr[i].id]["Relay_state"] = spund_arr[i].relay_state;
        }
    }

    // message["data"]["memory"]["Input_memory_size"] = input.memoryUsage();
    // message["data"]["memory"]["Output_memory_size"] = message.memoryUsage();
    serializeJson(message, Serial);
    Serial.println("");
    // client.publish(_PUBTOPIC, message.as<String>());
    delay(5000);
}
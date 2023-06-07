#include <Arduino.h>

#include <ArduinoJson.h>
#include <EspMQTTClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <array>

#include "ADS_Sensor.h"
#include "Spund_System.h"
#include "Relay.h"

#include "config.h"
#include "server.h"

std::array<Spund_System, _NUMBER_OF_SPUNDERS> spund_arr;
EspMQTTClient client(_SSID, _PASS, _MQTTHOST, _CLIENTID, _MQTTPORT);
AsyncWebServer server(80);

void setup(void)
{
    Serial.begin(115200);
    client.setMaxPacketSize(4096);
    client.enableOTA();

    WiFi.begin(_SSID, _PASS);
    uint8_t failed_connections = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("connecting..");
        failed_connections++;
        if (failed_connections > 10)
        {
            Serial.println("restarting..");
            ESP.restart();
        }
    }
    Serial.print("Connected to ");
    Serial.println(WiFi.localIP());

    // Initialize each Spunder in spund_arr
    for (uint8_t i = 0; i < _NUMBER_OF_SPUNDERS; ++i)
    {
        spund_arr[i].begin(
            ADS1115_ADDRESS1,     // ads_address
            GAIN_TWOTHIRDS,       // ads_gain
            _I2C_SDA,             // i2c_sda
            _I2C_SCL,             // i2c_scl
            i,                    // ads_channel
            _MIN_SENSOR_VOLTS[i], // volts at ZERO PSI
            _MAX_SENSOR_VOLTS[i], // volts at MAX PSI
            _MAX_PSIS[i],         // MAX PSI sensor can read
            _RELAY_PINS[i]);      // relay pin
    }

    // Webserver
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
            _DESIRED_VOLS[i] = _SETPOINT_MESSAGES[i].toDouble();
            inputMessage = _SETPOINT_MESSAGES[i];
            inputParam = _SETPOINT_INPUTS[i];
        }
        if (request->hasParam(_MQTT_INPUTS[i])) {
            _MQTT_MESSAGES[i] = request->getParam(_MQTT_INPUTS[i])->value();
            _MQTT_FIELDS[i] =  _MQTT_MESSAGES[i];
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

void onConnectionEstablished()
{
    client.subscribe(_SUBTOPIC, [](const String &payload)
                     {
        // Serial.println(payload);
        StaticJsonDocument<4000> input;
        deserializeJson(input, payload);

        StaticJsonDocument<1000> message;
        message["key"] = _CLIENTID;

        for (uint8_t i = 0; i < _NUMBER_OF_SPUNDERS; ++i)
        {
            spund_arr[i].tempC = input["data"][_MQTT_FIELDS[i]]["value[degC]"];
            spund_arr[i].tempF = spund_arr[i].tempC * 1.8 + 32;
            spund_arr[i].vols_setpoint = _DESIRED_VOLS[i];

            message["data"][_SPUNDER_NAMES[i]]["TempC"] = spund_arr[i].tempC;
            message["data"][_SPUNDER_NAMES[i]]["Volts"] = spund_arr[i].getVolts();
            message["data"][_SPUNDER_NAMES[i]]["PSI"] = spund_arr[i].getPSI();
            message["data"][_SPUNDER_NAMES[i]]["PSI_setpoint"] = spund_arr[i].computePSISetpoint();
            message["data"][_SPUNDER_NAMES[i]]["Vols_setpoint"] = spund_arr[i].vols_setpoint;
            message["data"][_SPUNDER_NAMES[i]]["Vols"] = spund_arr[i].computeVols();
            message["data"][_SPUNDER_NAMES[i]]["Minutes_since_vent"] = spund_arr[i].test_carb();
        }

        message["data"]["memory"]["Input_memory_size"] = input.memoryUsage();
        message["data"]["memory"]["Output_memory_size"] = message.memoryUsage();

        serializeJsonPretty(message, Serial);
        Serial.println("");

        client.executeDelayed(5000, [&message]()
                              { 
                                client.publish(_PUBTOPIC, message.as<String>()); 
                                // serializeJson(message, Serial);
                              }); });
}

void loop(void)
{
    client.loop();
}
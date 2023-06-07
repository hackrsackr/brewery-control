#include <Arduino.h>

#include <ArduinoJson.h>
#include <EspMQTTClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <array>
#include <vector>

#include "ADS_Sensor.h"
#include "Spund_System.h"
#include "Relay.h"

#include "new_config.h"
#include "server.h"

std::array<Spund_System, _NUMBER_OF_SPUNDERS> spund_arr;
// std::vector<Spund_System> spund_vec;
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
    for (auto &settings : _SETTINGS)
    {
        uint8_t i = 0;

        // Serial.println("pre");
        // Serial.println(settings.name);
        // Serial.println(settings.desired_vols);
        // Serial.println(settings.relay_pin);
        // Serial.println(settings.temp_sensor_name);
        // Serial.println(settings.ads_channel);

        // Spund_System s();
        // s.begin(
        // spund_arr[i].settings = settings;

        spund_arr[i].begin(
            // spund_vec[i].begin(
            ADS1115_ADDRESS1,          // ads_address
            GAIN_TWOTHIRDS,            // ads_gain
            _I2C_SDA,                  // i2c_sda
            _I2C_SCL,                  // i2c_scl
            settings.ads_channel,      // ads_channel
            settings.min_sensor_volts, // volts at ZERO PSI
            settings.max_sensor_volts, // volts at MAX PSI
            settings.max_psi,          // MAX PSI sensor can read
            settings.relay_pin);       // relay pin

        // spund_vec.push_back(s);
        // Serial.println("");
        Serial.println("post");
        Serial.println(spund_arr[i].settings.name);
        Serial.println(spund_arr[i].settings.desired_vols);
        Serial.println(spund_arr[i].settings.relay_pin);
        Serial.println(spund_arr[i].settings.temp_sensor_name);
        Serial.println(spund_arr[i].settings.ads_channel);
        Serial.println(spund_arr[i].settings.server_sensor);
        Serial.println(spund_arr[i].settings.server_sensor);
    }

    // Webserver
    //     server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    //               { request->send_P(200, "text/html", index_html, processor); });
    //     server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
    //               {
    //     String inputMessage;
    //     String inputParam;

    //     for (auto& spunder : _SPUNDERS) {
    //         if (request->hasParam(String(spunder.desired_vols))) {
    //             spunder.desired_vols = request->getParam(spunder.server_setpoint_input)->value().toDouble();
    //             // spunder.desired_vols = spunder.server_setpoint.toDouble();
    //             inputMessage = spunder.desired_vols;
    //             inputParam = spunder.server_setpoint_input;
    //         }
    //         if (request->hasParam(spunder.temp_sensor_name)) {
    //             spunder.temp_sensor_name = request->getParam(spunder.server_sensor_input)->value();
    //             // _MQTT_FIELDS[i] =  _MQTT_MESSAGES[i];
    //             inputMessage = spunder.temp_sensor_name;
    //             inputParam = spunder.server_sensor_input;
    //         }

    //     }

    //     request->send(200, "text/html", "HTTP GET request sent to your ESP on input field ("
    //                                      + inputParam + ") with value: " + inputMessage +
    //                                      "<br><a href=\"/\">Return to Home Page</a>"); });
    //     server.onNotFound(notFound);
    //     server.begin();
}

void onConnectionEstablished()
{
    client.subscribe(
        _SUBTOPIC, [](const String &payload)
        {
            //  Serial.println(payload);
            StaticJsonDocument<4000> input;
            deserializeJson(input, payload);

            StaticJsonDocument<1000> message;
            message["key"] = _CLIENTID;

            for (uint8_t i = 0; i < _NUMBER_OF_SPUNDERS; ++i)
            //  for (auto &spunder : spund_arr)
            {
            // if (!spund_arr[i].tempC)
            // {
            //     Serial.println(" no temp reading");
            //     continue;
            // }
            // else {
            // spund_arr[i].tempC = input["data"][spund_arr[i].settings.temp_sensor_name]["value[degC]"];
                spund_arr[i].tempC = input["data"][spund_arr[i].settings.server_sensor]["value[degC]"];
                Serial.println(spund_arr[i].tempC);
            //  Serial.println(spunder.settings.temp_sensor_name);
                spund_arr[i].tempF = spund_arr[i].tempC * 1.8 + 32;
                spund_arr[i].vols_setpoint = spund_arr[i].settings.desired_vols;
                message["data"][spund_arr[i].settings.name]["TempC"] = spund_arr[i].tempC;
            //  message["data"][spunder.settings.name]["Volts"] = spunder.getVolts();
            //  message["data"][spunder.settings.name]["PSI"] = spunder.getPSI();
            //  message["data"][spunder.settings.name]["PSI_setpoint"] = spunder.computePSISetpoint();
            //  message["data"][spunder.settings.name]["Vols_setpoint"] = spunder.vols_setpoint;
            //  message["data"][spunder.settings.name]["Vols"] = spunder.computeVols();
            //  message["data"][spunder.settings.name]["Minutes_since_vent"] = spunder.test_carb();

            // i++;
            // }
            }

            message["data"]["memory"]["Input_memory_size"] = input.memoryUsage();
            message["data"]["memory"]["Output_memory_size"] = message.memoryUsage();

            serializeJsonPretty(message, Serial);
            Serial.println("");

            client.executeDelayed(5000, [&message]()
                                {
                                    client.publish(_PUBTOPIC, message.as<String>());
                                //  serializeJson(message, Serial);
                                }); });
}

void loop(void)
{
    client.loop();
}
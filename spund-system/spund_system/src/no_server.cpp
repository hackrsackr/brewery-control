// #include <Arduino.h>

#include <ArduinoJson.h>
#include <EspMQTTClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <vector>

#include "ADS_Sensor.h"
#include "Spund_System.h"
#include "Relay.h"
#include "secrets.h"
#include "server.h"
#include "new_config.h"

std::vector<Spund_System *> _SPUNDERS;
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

    for (auto &settings : _SETTINGS)
    {
        Spund_System *s = new Spund_System(
            ADS1115_ADDRESS1,          // ads_address
            GAIN_TWOTHIRDS,            // ads_gain
            _I2C_SDA,                  // i2c_sda
            _I2C_SCL,                  // i2c_scl
            settings.ads_channel,      // ads_channel
            settings.min_sensor_volts, // volts at ZERO PSI
            settings.max_sensor_volts, // volts at MAX PSI
            settings.max_psi,          // MAX PSI sensor can read
            settings.relay_pin);       // relay pin

        s->settings.name = settings.name;
        s->vols_setpoint = settings.desired_vols;
        s->settings.desired_vols = settings.desired_vols;
        s->settings.temp_sensor_name = settings.temp_sensor_name;

        s->settings.server_setpoint = String(s->vols_setpoint);
        s->settings.server_setpoint_input = settings.server_setpoint_input;
        s->settings.server_sensor = settings.server_sensor;
        s->settings.server_sensor_input = settings.server_sensor_input;
        _SPUNDERS.push_back(s);
    }

    // // Webserver
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", index_html, processor); });
    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    String inputMessage;
    String inputParam;

    for (auto& s : _SPUNDERS) {
        if (request->hasParam(s->settings.server_setpoint_input)) {
            s->settings.server_setpoint = request->getParam(s->settings.server_setpoint_input)->value();
            s->settings.desired_vols = s->settings.server_setpoint.toDouble();
            s->vols_setpoint = s->settings.server_setpoint.toDouble();
            inputMessage = s->settings.server_setpoint;
            inputParam = s->settings.server_setpoint_input;
        }
        if (request->hasParam(s->settings.server_sensor_input)) {
            // s->settings.server_sensor = request->getParam(s->settings.server_sensor_input)->value();
            s->settings.temp_sensor_name = request->getParam(s->settings.server_sensor_input)->value();
            s->settings.server_sensor = s->settings.temp_sensor_name;
            // inputMessage = s->settings.server_sensor;
            inputMessage = s->settings.temp_sensor_name;
            inputParam = s->settings.server_sensor_input;
        }

    }
        // _SPUNDERS.push_back(s);

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
        //  Serial.println(payload);
        StaticJsonDocument<4000> input;
        deserializeJson(input, payload);

        StaticJsonDocument<1000> message;
        message["key"] = _CLIENTID;

        for (auto &spunder : _SPUNDERS)
        {
            spunder->tempC = input["data"][spunder->settings.temp_sensor_name]["value[degC]"];
            spunder->tempF = spunder->tempC * 1.8 + 32;
            message["data"][spunder->settings.name]["TempC"] = spunder->tempC;
            message["data"][spunder->settings.name]["Volts"] = spunder->getVolts();
            message["data"][spunder->settings.name]["PSI"] = spunder->getPSI();
            message["data"][spunder->settings.name]["PSI_setpoint"] = spunder->computePSISetpoint();
            message["data"][spunder->settings.name]["Vols_setpoint"] = spunder->vols_setpoint;
            message["data"][spunder->settings.name]["Desired_vols"] = spunder->settings.desired_vols;
            message["data"][spunder->settings.name]["Temp_Sensor"] = spunder->settings.temp_sensor_name;
            message["data"][spunder->settings.name]["Server_Sensor"] = spunder->settings.server_sensor;
            message["data"][spunder->settings.name]["Vols"] = spunder->computeVols();
            message["data"][spunder->settings.name]["Minutes_since_vent"] = spunder->test_carb();
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
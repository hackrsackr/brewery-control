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
#include "config.h"

std::vector<Spund_System *> _SPUNDERS;

EspMQTTClient client(SECRET_SSID, SECRET_PASS, _MQTTHOST, _CLIENTID, _MQTTPORT);
void onConnectionEstablished();

AsyncWebServer server(80);
void notFound(AsyncWebServerRequest *request);
String processor(const String &var);

void setup(void)
{
    Serial.begin(115200);
    client.setMaxPacketSize(4096);
    client.enableOTA();

    WiFi.begin(SECRET_SSID, SECRET_PASS);
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
        Spund_System *s = new Spund_System(settings);
        _SPUNDERS.push_back(s);
    }

    // // Webserver
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", index_html, processor); });
    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    String inputMessage;
    String inputParam;

    for (auto& spunder : _SPUNDERS) {
        if (request->hasParam(spunder->server_setpoint_input)) {
            spunder->server_setpoint = request->getParam(spunder->server_setpoint_input)->value();
            spunder->desired_vols = spunder->server_setpoint.toDouble();
            inputMessage = spunder->server_setpoint;
            inputParam = spunder->server_setpoint_input;
        }
        if (request->hasParam(spunder->server_sensor_input)) {
            spunder->temp_sensor_id = request->getParam(spunder->server_sensor_input)->value();
            spunder->server_sensor = spunder->temp_sensor_id;
            inputMessage = spunder->server_sensor;
            inputParam = spunder->server_sensor_input;
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

void onConnectionEstablished()
{
    client.subscribe(_SUBTOPIC, [](const String &payload)
                     {
        //  Serial.println(payload);
        StaticJsonDocument<4000> input;
        deserializeJson(input, payload);

        StaticJsonDocument<900> data;

        for (auto &spunder : _SPUNDERS)
        {
            spunder->tempC = input["data"][spunder->temp_sensor_id]["value[degC]"];
            spunder->tempF = spunder->tempC * 1.8 + 32;
            data[spunder->spunder_id]["TempC"] = spunder->tempC;
            data[spunder->spunder_id]["Temp_Sensor"] = spunder->temp_sensor_id;
            data[spunder->spunder_id]["PSI"] = spunder->getPSI();
            data[spunder->spunder_id]["PSI_setpoint"] = spunder->computePSISetpoint();
            data[spunder->spunder_id]["Desired_vols"] = spunder->desired_vols;
            data[spunder->spunder_id]["Server_Sensor"] = spunder->server_sensor;
            data[spunder->spunder_id]["Vols"] = spunder->computeVols();
            data[spunder->spunder_id]["Minutes_since_vent"] = spunder->test_carb();
        }
        serializeJson(data, Serial);
        Serial.println("");

        StaticJsonDocument<1000> message;
        message["key"] = _CLIENTID;
        message["data"] = data;
        message["data"]["memory"]["Input_memory_size"] = input.memoryUsage();
        message["data"]["memory"]["Output_memory_size"] = message.memoryUsage();

        serializeJsonPretty(message["data"], Serial);
        Serial.println("");

        client.executeDelayed(5000, [&message]()
        {
            client.publish(_PUBTOPIC, message.as<String>());
            //  serializeJson(message, Serial);
        }); });
}

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

String processor(const String &var)
{
    if (var == "SETPOINT1")
    {
        return _SPUNDERS[0]->server_setpoint;
    }
    if (var == "SETPOINT2")
    {
        return _SPUNDERS[1]->server_setpoint;
    }
    if (var == "SETPOINT3")
    {
        return _SPUNDERS[2]->server_setpoint;
    }
    if (var == "SETPOINT4")
    {
        return _SPUNDERS[3]->server_setpoint;
    }
    if (var == "MQTT1")
    {
        return _SPUNDERS[0]->server_sensor;
    }
    if (var == "MQTT2")
    {
        return _SPUNDERS[1]->server_sensor;
    }
    if (var == "MQTT3")
    {
        return _SPUNDERS[2]->server_sensor;
    }
    if (var == "MQTT4")
    {
        return _SPUNDERS[3]->server_sensor;
    }
    return String();
}